/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamazzal <mamazzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 22:48:52 by mamazzal          #+#    #+#             */
/*   Updated: 2023/12/29 18:23:52 by mamazzal         ###   ########.fr       */
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

void Server::serve() {
    for (;;) {
        int server_fd, new_socket;
        ssize_t valread;
        struct sockaddr_in address;
        int opt = 1;
        socklen_t addrlen = sizeof(address);
        char buffer[1024] = {0};
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(8080);
        bind(server_fd, (struct sockaddr *)&address, sizeof(address));
        listen(server_fd, 3);
        new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        valread = read(new_socket, buffer, 1024 - 1); // subtract 1 for the null terminator at the end
        printf("%s\n", buffer);
        // Send the HTTP response
        send(new_socket, this->httpRes.c_str(), this->httpRes.length(), 0);
        std::cout << "Response sent\n";
        // Closing the connected socket
        close(new_socket);
        // Closing the listening socket
        close(server_fd);
    }
}


Server::~Server() {}