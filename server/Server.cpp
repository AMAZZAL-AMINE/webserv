/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamazzal <mamazzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 22:48:52 by mamazzal          #+#    #+#             */
/*   Updated: 2024/01/17 17:27:49 by mamazzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../main.h"

std::string get_response_message(std::string fhtml) {
    std::string root = ROOT;
    fhtml = root + "/" + fhtml;
    char resolvedPath[PATH_MAX];
    realpath(fhtml.c_str(), resolvedPath);
    std::fstream file(resolvedPath);
    if (!file.is_open())
        return "error";
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return buffer.str();
}

Server::Server() {
    std::string htmlData = get_response_message("index.html");
    this->httpRes = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(htmlData.length()) + "\n\n" + htmlData + "\n";
}
int setup_server(const t_config & data,struct sockaddr_in & address) {
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
    if (code == 404) {
        htmlData = get_response_message(data.error404);
        res_status = "404 Not Found";   
    } else if (code == 500) {
        htmlData = get_response_message(data.error500);
        res_status = "500 Internal Server Error";
    } else if (code == 400) {
        htmlData = get_response_message(data.error400);
        res_status = "400 Bad Request";
    } else if (code == 408) {
        htmlData = get_response_message(data.error408);
        res_status = "408 Request Timeout";
    } else if (code == 413) {
        htmlData = get_response_message(data.error413);
        res_status = "413 Payload Too Large";
    }else if (code == 403) {
        htmlData = get_response_message(data.error403);
        res_status = "403 Forbidden";
    }else if (code ==  405) {
        htmlData = get_response_message(data.error405);
        res_status = "405 Method Not Allowed";
    }else if (code ==  501) {
        htmlData = get_response_message(data.error501);
        res_status = "501 Not Implemented";
    }
    std::string httpResq = "HTTP/1.1 " + res_status + "\nContent-Type: text/html\nContent-Length: " + std::to_string(htmlData.length()) + "\n\n" + htmlData + "\n";
    send(client_fd, httpResq.c_str(), httpResq.length(), 0);
}

void prinHttpRequest(HttpRequest & req) {
    std::cout << "\033[1;32m----------- Start Request -------\033[0m\n" << std::endl;
    std::cout << "Method : " << req.method << std::endl;
    std::cout << "Path : " << req.path << std::endl;
    std::cout << "Version : " << req.version << std::endl;
    std::cout << "HEADERS : " << std::endl;
    std::map<std::string, std::string>::iterator it = req.headers.begin();
    for (; it != req.headers.end(); it++) {
        std::cout << it->first << " : " << it->second << std::endl;
    }
    if (req.has_body == true) {   
        std::cout << "BODY : " << std::endl;
            std::cout << req.body << std::endl;
    }else if (req.has_query == true) {
        std::cout << "QUERY : " << req.query << std::endl;
    }
}

void Server::serve(const t_config & data) {
    int server_fd;

    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    server_fd = setup_server(data, address);
    fd_set fds;
    std::cout << "server runing on "<< std::endl;
    std::cout << "      local : localhost:" << data.port << std::endl;
    std::cout << "      network : 10.11.3.5:" << data.port << std::endl;
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
        if (ret == 0)
           response_errors(client_fd, 408, data);
        else if (ret < 0)
            response_errors(client_fd, 500, data);
        else if (ret == -1)
            response_errors(client_fd, 500, data);
        else {
            char buffer[3000] = {0};
            ssize_t valread = recv(client_fd, buffer, sizeof(buffer), 0);
            if (valread <= 0)
                response_errors(client_fd, 500, data);
            else {
                std::string requestBody(buffer, valread);
                HttpRequest req = parseHttpRequest(requestBody);
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
                req = parseHttpRequest(requestBody);
                std::cout << requestBody + "\n";
                handle_request(req, client_fd, data);
            }
            close(client_fd);
        }
    }
    close(server_fd);
}

void Server::handle_request(HttpRequest & req, int & client_fd, const t_config & data) {
    if (req.method == "POST") {   
        handle_post_requst(req);
        send(client_fd, this->httpRes.c_str(), this->httpRes.length(), 0);
    } else if (req.method == "GET")
        handle_get_requst(req, client_fd, data);
    else if (req.method == "DELETE")
        response_errors(client_fd, 400, data);
    else  {
        std::cout << "METHOD : " << req.method << std::endl;
        response_errors(client_fd, 405, data);
    }
    clear_httprequest(req);
}

void save_file(HttpRequest & req) {
    std::string root = ROOT;
    std::string path = root + "/uploads/" +  req.file_name[0];
    std::ofstream ofs;
    ofs.open(path, std::ofstream::out | std::ofstream::trunc);
    if (!ofs)
        throw std::runtime_error("Could not open file for writing");
    ofs << req.form_data[0];
    std::cout << "file uploaded" << std::endl;
    ofs.close();
}

void Server::handle_post_requst(HttpRequest &  req) {
    for (size_t i = 0; i < req.form_data.size(); i++) {
       if (req.content_type[i] == "file_upload")
            save_file(req);
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
    std::string full_path = std::string(ROOT) + req.path;
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

void get_data_response(HttpRequest & req, int & client_fd, const t_config & data) {
    std::string full_path = std::string(ROOT) + req.path;
    char resolvedPath[PATH_MAX];
    realpath(full_path.c_str(), resolvedPath);
    //chck permission
    if (access(resolvedPath, R_OK) == -1) {
        response_errors(client_fd, 403, data);
        return;
    }
    //check if file exist
    if (access(resolvedPath, F_OK) == -1)
        response_errors(client_fd, 404, data);
    else if (isDirectory(resolvedPath))
        directory_response(req, client_fd, data);
    else
        file_response(req, client_fd, data, resolvedPath);
}

void Server::handle_get_requst(HttpRequest &  req, int & client_fd, const t_config & data) {
    if (req.path == "/" && !req.has_query)
        send(client_fd, this->httpRes.c_str(), this->httpRes.length(), 0);
    else if (!req.has_body)
        get_data_response(req, client_fd, data);
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
    req.body.clear();
}

Server::~Server() {}
