 /* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamazzal <mamazzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 22:48:52 by mamazzal          #+#    #+#             */
/*   Updated: 2024/02/13 17:05:44 by mamazzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../main.h"

std::string read_html_file(std::string fhtml, const t_config & data) {
    fhtml = data.root + "/" + fhtml;
    char resolvedPath[PATH_MAX];
    realpath(fhtml.c_str(), resolvedPath);
    std::fstream file(resolvedPath);
    if (!file.is_open())
        return "<h1 >ROOT NOT FOUND</h1>";
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return buffer.str();
}

Server::Server(const t_config & data) {
    std::string htmlData = read_html_file("index.html", data);
    this->httpRes = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(htmlData.length()) + "\n\n" + htmlData + "\n";
}

void set_nonblock(int socket) {
    int flags = fcntl(socket, F_GETFL);
    flags |= O_NONBLOCK;
    fcntl(socket, F_SETFL, flags);
}

int Server::setup_server(const t_config & data,struct sockaddr_in & address) {
    int server_fd;
    int opt = 1;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    set_nonblock(server_fd);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(data.port);
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, BACKLOG);
    return server_fd;
}


bool is_server_fd(int fd, int server_fd) {
    return fd == server_fd;
}

typedef struct t_request {
    std::string request_str;
    t_config conf;
    HttpRequest req_parsed_data;
    int server_fd;
    int client_fd;
} t_request;

#include <sys/ioctl.h>

void Server::serve(std::vector<t_config> http_config) {
    std::vector<int> server_fds;
    std::map<int, t_request> server_config;
    std::map<int, t_config> configs;
    struct sockaddr_in address;
    socklen_t addresslen = sizeof(address);
    for (size_t i = 0; i != http_config.size(); i++) {
        int fd = setup_server(http_config[i], address);
        server_fds.push_back(fd);
        configs[fd] = http_config[i];
    }
    int server_fd_ = setup_server(http_config[0], address);
    std::cout << GREEN << "[SERVER STARTED - " << current_date() << "] " << RESET << http_config[0].host_name << ":" << http_config[0].port << std::endl;
    timeval timeout;
    std::vector<int> client_sockets;
    int max_fd = server_fd_ + 1;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    const int MAX_CLIENTS = 200;
    int client_socket[MAX_CLIENTS];
    for (int i = 0; i < MAX_CLIENTS; i++)
        client_socket[i] = 0;
    int new_socket;
    while (true) {
        signal(SIGPIPE, SIG_IGN);
        fd_set read_fds, write_fds;
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        for (size_t i = 0; i != server_fds.size(); i++) {
            server_fd_ = server_fds[i];
            FD_SET(server_fd_, &read_fds);
        }
        int activity = select(max_fd + 1, &read_fds, &write_fds, nullptr, &timeout);
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (client_socket[i] > 0) {
                FD_SET(client_socket[i], &read_fds);
                if (client_socket[i] > max_fd)
                    max_fd = client_socket[i];
            }
        }
        if (activity < 0) {
            perror("select");
            exit(EXIT_FAILURE);
        }
        for(size_t i = 0; i != server_fds.size(); i++) {
            server_fd_ = server_fds[i];
            if (FD_ISSET(server_fd_, &read_fds)) {
                new_socket = accept(server_fd_, (struct sockaddr *)&address, &addresslen);
                if (new_socket < 0) {
                    perror("accept");
                    exit(EXIT_FAILURE);
                }
                for (int i = 0; i < MAX_CLIENTS; i++) {
                    if (client_socket[i] == 0) {
                        client_socket[i] = new_socket;
                        server_config[new_socket].server_fd = server_fd_;
                        server_config[new_socket].conf = configs[server_fd_];
                        if (client_socket[i] > max_fd)
                            max_fd = client_socket[i];
                        break;
                    }
                }
        }
        }
        // Check each client socket for data to read
        for (int i = 0; i < MAX_CLIENTS; ++i) {
            int sd = client_socket[i];
            if (FD_ISSET(sd, &read_fds) && sd != server_fd_) {
                char buffer[BUFFER_SIZE] = {0};
                int valread = recv(sd, buffer, sizeof(buffer) - 1, 0);
                if (valread == 0) {
                    close(sd);
                    client_socket[i] = 0;
                }else if (valread > 0) {
                    buffer[valread] = '\0';
                    std::string str(buffer, valread);
                    FD_SET(client_socket[i], &write_fds);
                    server_config[sd].request_str += str;
                }
            }
            if (FD_ISSET(sd, &write_fds) && sd != server_fd_) {
                server_config[sd].req_parsed_data = parseHttpRequest(server_config[sd].request_str, server_config[sd].conf);
                if (server_config[sd].req_parsed_data.method == POST) {
                    if (server_config[sd].req_parsed_data.content_length > static_cast<int>(server_config[sd].req_parsed_data.full_body.length())) {
                        FD_CLR(sd, &write_fds);
                        continue;
                    }
                }
                handle_request(server_config[sd].req_parsed_data, sd, server_config[sd].conf);
                close(sd);
                client_socket[i] = 0;
                clear_httprequest(server_config[sd].req_parsed_data);
                server_config[sd].request_str = "";
                FD_CLR(sd, &write_fds);
                FD_CLR(sd, &read_fds);
                server_config.erase(sd);
            }
        }
    }
    close(server_fd_);
    for (size_t i = 0; i < server_fds.size(); i++)
        close(server_fds[i]);
}

void Server::request_(int & __unused client_fd, const t_config & __unused data) {

}

void handle_delete_request(HttpRequest & __unused req, int & __unused client_fd, const t_config  & __unused data); 

int check_file_exist(std::string path) {
    //check permission
    if (access(path.c_str(), F_OK) == -1)
        return 404;
    else if (access(path.c_str(), R_OK) == -1)
        return 403;
    return 0;
}

void Server::handle_request(HttpRequest & req, int & client_fd, const t_config & data) {
    if (req.path.find(".php") != SIZE_T_MAX && req.method != DELETE) {
        std::string path = data.root + req.path;
        if (check_file_exist(path) != 0) {
            response_errors(client_fd, check_file_exist(path), data);
            return;
        }
        std::string cgi_path = run_cgi(req, data,std::string("text/html"), path);
        std::cout << path << std::endl;
        send(client_fd, cgi_path.c_str(), cgi_path.length(), 0);
        std::cout << GREEN << "[RESPONSE - " << current_date() << "] " << RESET << data.host_name << ":" << data.port << " "  << RESET << "  " << BG_WHITE << enum_to_string(req.method) << " " << req.path << RESET << std::endl;
        return;
    }
    else if (req.method == POST) {
        handle_post_requst(req, data);
        send(client_fd, this->httpRes.c_str(), this->httpRes.length(), 0);
        std::cout << GREEN << "[RESPONSE - " << current_date() << "] " << RESET << data.host_name << ":" << data.port << " "  << RESET << " "  << enum_to_string(req.method) << " " << req.path << RESET << std::endl;
    } else if (req.method == GET) {
        handle_get_requst(req, client_fd, data);
        std::cout << GREEN << "[RESPONSE - " << current_date() << "] " << RESET << data.host_name << ":" << data.port << " "  << RESET << " "  << enum_to_string(req.method) << " " << req.path << RESET << std::endl;   
    }
    else if (req.method == DELETE)
        handle_delete_request(req, client_fd, data);
    else
        response_errors(client_fd, 405, data);
    clear_httprequest(req);
}

void handle_delete_request(HttpRequest & __unused req, int & __unused client_fd, const t_config  & __unused data) {
    std::string path = data.root + req.path;
    if (check_file_exist(path) != 0) {
        response_errors(client_fd, check_file_exist(data.root + req.path), data);
        return;
    }
    if (isDirectory(path.c_str())) {
        //check if directory not empty 
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir(path.c_str())) != NULL) {
            while ((ent = readdir(dir)) != NULL) {
                if (std::string(ent->d_name) != "." && std::string(ent->d_name) != "..") {
                    response_errors(client_fd, 409, data);
                    return;
                }
            }
            closedir (dir);
        }
    }
    //check if it has permission to delete
    if (access(path.c_str(), W_OK) == -1) {
        response_errors(client_fd, 403, data);
        return;
    }
    if (std::remove(path.c_str()) != 0) {
        std::cout << "PATH : " << path << std::endl;
        response_errors(client_fd, 500, data);
    }
    else {
        std::string html_data = "<html><head><title>Index of " + req.path + "</title></head><body><h1>File : " + req.path + " deleted</h1><hr><pre>";
        std::string httpRes = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(html_data.length()) + "\n\n" + html_data + "\n";
        send(client_fd, html_data.c_str(), html_data.length(), 0);
    }
}

void save_file(HttpRequest & req, const t_config & __unused data, size_t & index) {
    std::string path = data.root + "/uploads/" +  req.file_name[index];
    std::ofstream ofs;
    ofs.open(path, std::ofstream::out | std::ofstream::trunc);
    if (!ofs)
        throw std::runtime_error("Could not open file for writing");
    ofs << req.form_data[index];
    std::cout << YELLOW << "[FILE SAVED - " << current_date() << "] " << RESET << path << std::endl;
    ofs.close();
}

void Server::handle_post_requst(HttpRequest &  req, const t_config & data) {
    for (size_t i = 0; i < req.form_data.size(); i++) {
       if (req.content_type[i] == "file_upload")
            save_file(req, data, i);
        else
            std::cout << req.content_names[i] << " : " << req.form_data[i] << std::endl;
    }
}

bool isDirectory(const char* path) {
    struct stat statResult;
    if (stat(path, &statResult) != 0)
        return false;
    return S_ISDIR(statResult.st_mode);
}

void directory_response(HttpRequest & req, int & client_fd, const t_config & data) {
    std::string full_path = data.root + req.path;
    char resolvedPath[PATH_MAX];
    realpath(full_path.c_str(), resolvedPath);
    std::string htmlData = "";
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(resolvedPath)) != NULL) {
        htmlData += "<html><head><title>Index of " + req.path + "</title></head><body><h1>Directory : " + req.path + "</h1><hr><pre>";
        while ((ent = readdir(dir)) != NULL) {
            if (std::string(ent->d_name) != "." && std::string(ent->d_name) != "..")
                htmlData += "<a href=\"" + req.path + "/" + ent->d_name + "\">" + ent->d_name + "</a><br>";
        }
        htmlData += "</pre>";
        closedir (dir);
    } else {
        response_errors(client_fd, 404, data);
        return;
    }
    std::string httpRes = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(htmlData.length()) + "\n\n" + htmlData + "\n";
    send(client_fd, httpRes.c_str(), httpRes.length(), 0);
}

std::vector<std::string> split_string(std::string s, std::string delimiter) {
    std::vector<std::string> extensions;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        extensions.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    extensions.push_back(s);
    return extensions;
}

std::string get_file_extention(char * resolvedPath, HttpRequest & req) {
    std::string extension = resolvedPath;
    size_t dotPos = extension.find_last_of('.');
    if (dotPos != std::string::npos)
        extension = extension.substr(dotPos + 1);
    std::vector<std::string> content_types = split_string(req.headers["Accept"], ",");
    std::string contentType = "";
    for (size_t i = 0; i < content_types.size(); i++) {
        if (content_types[i].find(extension) != std::string::npos) {
            contentType = content_types[i];
            break;
        }
    }
    if (contentType.empty())
        contentType = "text/plain";
    return contentType;
}
void file_response(HttpRequest &req, int &client_fd, const t_config & __unused data, char *resolvedPath) {
    int fd = open(resolvedPath, O_RDONLY);
    if (fd == -1) {
        response_errors(client_fd, 500, data);
        return;
    }
    std::string contentType = get_file_extention(resolvedPath, req);
    struct stat stat_buf;
    fstat(fd, &stat_buf);
    std::string httpRes = "HTTP/1.1 200 OK\nContent-Type: " + contentType + "\nContent-Length: " + std::to_string(stat_buf.st_size) + "\n\n";
    send(client_fd, httpRes.c_str(), httpRes.length(), 0);
    char buffer[BUFFER_SIZE_BIG];
    int bytes_read;
    while ((bytes_read = read(fd, buffer, BUFFER_SIZE_BIG)) != 0) {
        ssize_t bytes_sent = send(client_fd, buffer, bytes_read, 0);
        if (bytes_sent == -1)
            break;
        usleep(1500);
    }
    close(fd);
}

void get_response(HttpRequest & req, int & client_fd, const t_config & data) {
    std::string full_path = data.root + req.path;
    char resolvedPath[PATH_MAX];
    realpath(full_path.c_str(), resolvedPath);
    //check if file exist
    if (check_file_exist(resolvedPath) != 0) {
        response_errors(client_fd, check_file_exist(resolvedPath), data);
        return;
    } else if (isDirectory(resolvedPath))
        if (access((std::string(resolvedPath) + "/index.html").c_str(), F_OK) != -1)
            file_response(req, client_fd, data, (char *)(std::string(resolvedPath) + "/index.html").c_str());
        else {
            std::cout << "autoindex : " << data.autoindex << std::endl;
            if (data.autoindex == "off")
                response_errors(client_fd, 403, data);
            else if (data.autoindex == "on")
                directory_response(req, client_fd, data);
        }
    else
        file_response(req, client_fd, data, resolvedPath);
}

void Server::handle_get_requst(HttpRequest &  req, int & client_fd, const t_config & data) {
    if (req.path == "/" && data.autoindex == "on" && !req.has_query) 
        send(client_fd, this->httpRes.c_str(), this->httpRes.length(), 0);
    else if (!req.has_body)
        get_response(req, client_fd, data);
}

void clear_httprequest(HttpRequest & req) {
    req.method = NULL_METHOD;
    req.path = "";
    req.version = "";
    req.is_valid = false;
    req.ifnotvalid_code_status = false;
    req.is_chunked = false;
    req.content_length = 0;
    req.has_body = false;
    req.has_query = false;
    req.query = "";
    req.headers.clear();
    req.full_body.clear();
}


Server::~Server() {}
