/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamazzal <mamazzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 22:48:52 by mamazzal          #+#    #+#             */
/*   Updated: 2023/12/31 19:57:20 by mamazzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../main.h"

Server::Server() {
    std::fstream file("gg.html");
    std::string line, htmlData;
    while (std::getline(file, line))
        htmlData += line;
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
    listen(server_fd, 3);
    return server_fd;
}

void Server::serve(const t_config & data) {
    int server_fd, client_fd;
    char buffer[30000] = {0};
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
        poll(fds, 2, -1);
        if (fds[0].revents & POLLIN) {
            fds[1].fd = accept(server_fd, (struct sockaddr *)&address, &addrlen);
            fds[1].events = POLLIN;
            std::cout << "\033[1;36m----------- New client connected -------\033[0m\n" << std::endl;
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
                buffer[rs] = '\0'; // Null-terminate the received data
                std::cout << buffer << std::endl;
                send(client_fd, this->httpRes.c_str(), this->httpRes.length(), 0);
                close(client_fd);
                fds[1].fd = -1;
            }
        }
    }
    close(server_fd);
}


Server::~Server() {}
