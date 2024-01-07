/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouali <rouali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 22:48:52 by mamazzal          #+#    #+#             */
/*   Updated: 2024/01/07 13:06:47 by rouali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../main.h"
#include <sstream> 
std::string get_response_message(std::string fhtml) {
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
    std::string htmlData = get_response_message("html_root/index.html");
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

void image_response(HttpRequest & req, int client_fd) {
    std::string relativePath = "assets" + req.path;
    char resolvedPath[PATH_MAX];
    std::string htmlData = "";
    realpath(relativePath.c_str(), resolvedPath);
    std::ifstream file(resolvedPath, std::ios::binary);
    htmlData.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::string httpRes = "HTTP/1.1 200 OK\nContent-Type: image/png\nContent-Length: " + std::to_string(htmlData.length()) + "\n\n" + htmlData + "\n";
    file.close();
    send(client_fd, httpRes.c_str(), httpRes.length(), 0);
}

void response_errors(int client_fd, int code, const t_config & data) {
    std::string htmlData = "";
    if (code == 404)
        htmlData = get_response_message(data.error404);
    else if (code == 500)
        htmlData = get_response_message(data.error500);
    else if (code == 400)
        htmlData = get_response_message(data.error400);
    else if (code == 408)
        htmlData = get_response_message(data.error408);
    std::string httpResq = "HTTP/1.1 404 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(htmlData.length()) + "\n\n" + htmlData + "\n";
    send(client_fd, httpResq.c_str(), httpResq.length(), 0);
}

void documents_respons(int client_fd, const HttpRequest & req, const t_config & data) {
    std::string htmlData = get_response_message(std::string("html_root") + req.path);
    if (htmlData == "error")
        response_errors(client_fd, 404, data);
    else {
        std::string httpRes = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(htmlData.length()) + "\n\n" + htmlData + "\n";
        send(client_fd, httpRes.c_str(), httpRes.length(), 0);
    }
}

void prinHttpRequest(HttpRequest & req) {
    std::cout << "\033[1;32m----------- Start Request -------\033[0m\n" << std::endl;
    std::cout << "Method : " << req.method << std::endl;
    std::cout << "Path : " << req.path << std::endl;
    std::cout << "Version : " << req.version << std::endl;
    std::cout << "HEADERS : " << std::endl;
    for (size_t i = 0; i < req.headers.size(); i++)
        std::cout << req.headers[i] << std::endl;
    if (req.has_body == true) {   
        std::cout << "BODY : " << std::endl;
        for (size_t i = 0; i < req.body.size(); i++)
            std::cout << req.body[i] << std::endl;
    }else if (req.has_query == true) {
        std::cout << "QUERY : " << req.query << std::endl;
    }
}

bool is_request_img(HttpRequest & req) {
    std::string imgType[] = {".png", ".jpg", ".jpeg", ".gif", ".ico"};
    for (size_t i = 0; i < 5; i++) {
        if (req.path.find(imgType[i]) != SIZE_T_MAX)
            return true;
    }
    return false;
}

void Server::serve(const t_config & data) {
    int server_fd;
    char buffer[BUFSIZ] = {0};
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    server_fd = setup_server(data, address);
    fd_set fds;
    std::cout << "server runing on "<< std::endl;
    std::cout << "      local : localhost:" << data.port << std::endl;
    std::cout << "      network : 10.11.3.5:" << data.port << std::endl;
    listen(server_fd, 3);
    timeval timeout;
        FD_ZERO(&fds);
        FD_SET(server_fd, &fds);
    timeout.tv_sec = 15;
    timeout.tv_usec = 0;
    for (;;) {
        int ret = select(server_fd + 1, &fds, NULL, NULL, &timeout);
        int client_fd = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        if (ret == 0)
           response_errors(client_fd, 408, data);
        else if (ret == -1)
            response_errors(client_fd, 500, data);
        else {
            int valread = read(client_fd, buffer, BUFSIZ);
            if (valread == -1)
                response_errors(client_fd, 500, data);
            else {
                HttpRequest req = parseHttpRequest(std::string(buffer));
                // prinHttpRequest(req);
                if (req.path.find(".php") != SIZE_T_MAX)
                    send(client_fd, run_cgi(req).c_str(), run_cgi(req).length(), 0);
                if (req.path == "/")
                    send(client_fd, this->httpRes.c_str(), this->httpRes.length(), 0);
                else if (req.method == "GET") {
                    if (is_request_img(req))
                        image_response(req, client_fd);
                    else
                        documents_respons(client_fd, req, data);
                }else
                    response_errors(client_fd, 400, data);
                clear_httprequest(req);
            }
            close(client_fd);
        }
    }
    close(server_fd);
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
