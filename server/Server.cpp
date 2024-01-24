/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamazzal <mamazzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 22:48:52 by mamazzal          #+#    #+#             */
/*   Updated: 2024/01/24 15:35:25 by mamazzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../main.h"

std::string get_response_message(std::string fhtml, const t_config & data) {
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

char *current_date() {
    time_t now = time(0);
    char *dt = std::ctime(&now);
    dt[strlen(dt) - 1] = '\0';
    return dt;
}

Server::Server(const t_config & data) {
    std::string htmlData = get_response_message("index.html", data);
    this->httpRes = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(htmlData.length()) + "\n\n" + htmlData + "\n";
}
int Server::setup_server(const t_config & data,struct sockaddr_in & address) {
    int server_fd;
    int opt = 1;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(data.port);
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 20);
    return server_fd;
}

void response_errors(int client_fd, int code, const t_config & data) {
    std::string htmlData = "";
    std::string res_status = "";
    switch(code) {
        case 404 :
            htmlData = get_response_message(data.error404, data);
            res_status = "404 Not Found";   
            break;
        case 500 :
            htmlData = get_response_message(data.error500, data);
            res_status = "500 Internal Server Error";
            break;
        case 400 :
            htmlData = get_response_message(data.error400, data);
            res_status = "400 Bad Request";
            break;
        case 408 :
            htmlData = get_response_message(data.error408, data);
            res_status = "408 Request Timeout";
            break;
        case 413 :
            htmlData = get_response_message(data.error413, data);
            res_status = "413 Payload Too Large";
            break;
        case 403 :
            htmlData = get_response_message(data.error403, data);
            res_status = "403 Forbidden";
            break;
        case  405 :
            htmlData = get_response_message(data.error405, data);
            res_status = "405 Method Not Allowed";
            break;
        case 501 :
            htmlData = get_response_message(data.error501, data);
            res_status = "501 Not Implemented";
            break;
        case 409 :
            htmlData = get_response_message(data.error409, data);
            res_status = "409 Conflict";
            break;
        default :
            break;
    }
    std::string httpResq = "HTTP/1.1 " + res_status + "\nContent-Type: text/html\nContent-Length: " + std::to_string(htmlData.length()) + "\n\n" + htmlData + "\n";
    send(client_fd, httpResq.c_str(), httpResq.length(), 0);
    std::cout << RED << "[RESPONSE - " << current_date() << "] " <<  BG_WHITE << BLACK << res_status << RESET << std::endl;
}

void Server::serve(const t_config & data) {
    int server_fd;

    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    server_fd = setup_server(data, address);
    fd_set fds;
    std::cout << GREEN << "[SERVER STARTED - " << current_date() << "] " << RESET << data.host_name << ":" << data.port << std::endl;
    listen(server_fd, 3);
    timeval timeout;
    timeout.tv_sec = 15;
    timeout.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(server_fd, &fds);
    for (;;) {
        signal(SIGPIPE, SIG_IGN);
        int ret = select(server_fd + 1, &fds, NULL, NULL, &timeout);
        
        int client_fd = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        if (ret == 0) {
           response_errors(client_fd, 408, data);
            continue;
        } else if (ret < 0) {
            response_errors(client_fd, 500, data);
            continue;
        }
        else {
            char buffer[3000] = {0};
            ssize_t valread = recv(client_fd, buffer, sizeof(buffer), 0);
            if (valread <= 0)
                response_errors(client_fd, 500, data);
            else {
                std::string requestBody(buffer, valread);
                HttpRequest req = parseHttpRequest(requestBody, data);
                if (!req.is_valid) {
                    response_errors(client_fd, req.ifnotvalid_code_status, data);
                    continue;
                }
                if (buffer[0] == '\0')
                    continue;
                int reded_value = req.content_length;
                if (req.method == "POST") {
                    while (1) {
                        int content_length = req.content_length;
                        if (reded_value >= content_length && req.if_post_form_type != FORM_DATA)
                            break;
                        valread = recv(client_fd, buffer, sizeof(buffer), 0);
                        reded_value += valread;
                        if (valread <= 0)
                            response_errors(client_fd, 500, data);
                        else {
                            std::string newBuffer(buffer, valread);
                            requestBody += newBuffer;
                            std::string gg(buffer, valread);
                            if (reded_value >= content_length && requestBody.find(req.boundary_end) != SIZE_T_MAX)
                                break;
                        }
                    }
                }
                req = parseHttpRequest(requestBody, data);
                handle_request(req, client_fd, data);
            }
            close(client_fd);
        }
    }
    close(server_fd);
}

int check_file_exist(std::string path) {
    //check permission
    if (access(path.c_str(), F_OK) == -1)
        return 404;
    else if (access(path.c_str(), R_OK) == -1)
        return 403;
    return 0;
}

void Server::handle_request(HttpRequest & req, int & client_fd, const t_config & data) {
    if (req.path.find(".php") != SIZE_T_MAX && req.method != "DELETE") {
        std::string path = data.root + req.path;
        if (check_file_exist(path) != 0) {
            response_errors(client_fd, check_file_exist(path), data);
            return;
        }
        std::string cgi_path = run_cgi(req, data,std::string("text/html"), path);
        std::cout << path << std::endl;
        send(client_fd, cgi_path.c_str(), cgi_path.length(), 0);
        std::cout << GREEN << "[RESPONSE - " << current_date() << "] " << RESET << data.host_name << ":" << data.port << " "  << RESET << "  " << BG_WHITE << req.method << " " << req.path << RESET << std::endl;
        return;
    }
    else if (req.method == "POST") {
        handle_post_requst(req, data);
        // std::string cgi_path = run_cgi(req, data, std::string("text/html"), data.root + std::string("/php/post.php"));
        // send(client_fd, cgi_path.c_str(), cgi_path.length(), 0);
        send(client_fd, this->httpRes.c_str(), this->httpRes.length(), 0);
        std::cout << GREEN << "[RESPONSE - " << current_date() << "] " << RESET << data.host_name << ":" << data.port << " "  << RESET << " "  << req.method << " " << req.path << RESET << std::endl;
    } else if (req.method == "GET") {
        handle_get_requst(req, client_fd, data);
        std::cout << GREEN << "[RESPONSE - " << current_date() << "] " << RESET << data.host_name << ":" << data.port << " "  << RESET << " "  << req.method << " " << req.path << RESET << std::endl;   
    }
    else if (req.method == "DELETE")
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

void save_file(HttpRequest & req, const t_config & __unused data) {
    std::string path = data.root + "/uploads/" +  req.file_name[0];
    std::ofstream ofs;
    ofs.open(path, std::ofstream::out | std::ofstream::trunc);
    if (!ofs)
        throw std::runtime_error("Could not open file for writing");
    ofs << req.form_data[0];
    std::cout << YELLOW << "[FILE SAVED - " << current_date() << "] " << RESET << path << std::endl;
    ofs.close();
}

void Server::handle_post_requst(HttpRequest &  req, const t_config & data) {
    for (size_t i = 0; i < req.form_data.size(); i++) {
       if (req.content_type[i] == "file_upload")
            save_file(req, data);
        else {
            std::cout << req.content_names[i] << " : " << req.form_data[i] << std::endl;
        }
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

void file_response(HttpRequest & __unused req, int & client_fd, const t_config & __unused data, char * resolvedPath) {
    std::ifstream file(resolvedPath, std::ios::binary);

    if (!file.is_open()) {
        response_errors(client_fd, 404, data);
        return;
    }

    //file size
    file.seekg(0, std::ios::end);
    //check if file size is bigger than 10MB
    if (file.tellg() > 10000000) {
        response_errors(client_fd, 413, data);
        return;
    }
    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(fileSize);
    file.read(buffer.data(), fileSize);

    std::string extension = resolvedPath;
    size_t dotPos = extension.find_last_of('.');
    if (dotPos != std::string::npos)
        extension = extension.substr(dotPos + 1);

    std::string contentType;
    if (extension == "html")
        contentType = "text/html";
    else if (extension == "jpg" || extension == "jpeg")
        contentType = "image/jpeg";
    else if (extension == "png")
        contentType = "image/png";
    else
        contentType = "application/octet-stream";
    std::ostringstream httpRes;
        httpRes << "HTTP/1.1 200 OK\n"
        << "Content-Type: " << contentType << "\n"
        << "Content-Length: " << fileSize << "\n\n";

    send(client_fd, httpRes.str().c_str(), httpRes.str().length(), 0);
    send(client_fd, buffer.data(), fileSize, 0);
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
    req.method = "";
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
