/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamazzal <mamazzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 22:48:52 by mamazzal          #+#    #+#             */
/*   Updated: 2024/01/02 19:23:40 by mamazzal         ###   ########.fr       */
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
int setup_server(const t_config & data, sockaddr_in & address) {
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
    const char* relativePath = std::string(std::string("assets") + req.path).c_str();
    char resolvedPath[PATH_MAX];
    std::string line, htmlData = "";
    realpath(relativePath, resolvedPath);
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
}

void Server::serve(const t_config & data) {
    int server_fd, client_fd;
    char buffer[3000] = {0};
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    server_fd = setup_server(data, address);
    struct pollfd fds[2];
    fds[0].fd = server_fd;
    fds[0].events = POLLIN;  // Monitor for incoming connection requests
    fds[1].fd = -1;          // Initialize client socket to -1 (no client initially)
    fds[1].events = POLLIN;
    std::cout << "server runing on "<< std::endl;
    std::cout << "      local : localhost:" << data.port << std::endl;
    std::cout << "      network : 10.11.3.5:" << data.port << std::endl;
    listen(server_fd, 3);
    for (;;) {
        int ret = poll(fds, 2, 1000);
        if (ret == 0) {
           response_errors(client_fd, 408, data);
        }
        if (fds[0].revents & POLLIN) {
            fds[1].fd = accept(server_fd, (struct sockaddr *)&address, &addrlen);
            fds[1].events = POLLIN;
        }
        if (fds[1].revents & POLLIN) {
            client_fd = fds[1].fd;
            ssize_t rs = read(client_fd, buffer, sizeof(buffer) - 1);
            if (rs <= 0) {
                std::cout << "Client disconnected" << std::endl;
                close(client_fd);
                fds[1].fd = -1;
                continue;
            } else {
                buffer[rs] = '\0';
                HttpRequest __unused req = parseHttpRequest(buffer);
                prinHttpRequest(req);
                if (req.method != "GET" && req.method != "POST") {
                    response_errors(client_fd, 400, data);
                    continue;
                }
                if (req.path == "/")
                    send(client_fd, this->httpRes.c_str(), this->httpRes.length(), 0);
                if (req.method == "GET" && req.path == "/")
                    send(client_fd, this->httpRes.c_str(), this->httpRes.length(), 0);
                else if (req.method == "GET" && std::string(buffer).find("Sec-Fetch-Dest: image") != SIZE_T_MAX)
                    image_response(req, client_fd);
                else if (req.method == "GET" && std::string(buffer).find("Sec-Fetch-Dest: document") != SIZE_T_MAX)
                    documents_respons(client_fd, req, data);
                close(client_fd);
                fds[1].fd = -1;
            }
        }
    }
    close(server_fd);
}


Server::~Server() {}
