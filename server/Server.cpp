 /* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamazzal <mamazzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 22:48:52 by mamazzal          #+#    #+#             */
/*   Updated: 2024/04/20 13:03:36 by mamazzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../main.h"

Server::Server(const t_config __unused & data) {}

void set_nonblock(int socket) {
    int flags = fcntl(socket, F_GETFL);
    flags |= O_NONBLOCK;
    fcntl(socket, F_SETFL, flags);
}


int Server::setup_server(const t_config & data,struct sockaddr_in & address) {
    int server_fd;
    int opt = 1;

    memset(&address, 0, sizeof(address));
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    set_nonblock(server_fd);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr =  inet_addr(data.host_name.c_str());
    address.sin_port = htons(data.port);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
        throw std::runtime_error("Could not bind server socket");
    int lis = listen(server_fd, BACKLOG);
    if (lis < 0)
        throw std::runtime_error("Could not listen server socket");
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
    Response  response;
    socklen_t addresslen = sizeof(address);
    for (size_t i = 0; i != http_config.size(); i++) {
        for (size_t j = 0; j != http_config[i].ports.size(); j++) {
         t_config conf = http_config[i];
         conf.port = http_config[i].ports[j];
         int fd = setup_server(conf, address);
         if (fd == -1)
            throw std::runtime_error("Could not create server socket");
         server_fds.push_back(fd);
         configs[fd] = http_config[i];
         std::cout << GREEN << "[SERVER STARTED - " << current_date() << "] [" << http_config[i].server_name + "] " << RESET << http_config[i].host_name << ":" << http_config[i].ports[j] << std::endl;
        }
    }
    timeval timeout;
    std::vector<int> client_sockets;
    int max_fd = std::max(server_fds[0], server_fds[1]);
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    const int MAX_CLIENTS = 255;
    int client_socket[MAX_CLIENTS];
    for (int i = 0; i < MAX_CLIENTS; i++)
        client_socket[i] = 0;
    int new_socket;
    int server_fd_;
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
        if (activity < 0) {
            std::cerr << "select error" << std::endl;
            continue;
        }
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (client_socket[i] > 0) {
                FD_SET(client_socket[i], &read_fds);
                if (client_socket[i] > max_fd)
                    max_fd = client_socket[i];
            }
        }
        for(size_t i = 0; i != server_fds.size(); i++) {
            server_fd_ = server_fds[i];
            if (FD_ISSET(server_fd_, &read_fds)) {
                new_socket = accept(server_fd_, (struct sockaddr *)&address, &addresslen);
                if (new_socket < 0) {
                    std::cerr << "accept error" << std::endl;
                    continue;
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
                size_t NEW_BUFFER_SIZE = BUFFER_SIZE;
                if (server_config[sd].req_parsed_data.method == POST &&  server_config[sd].req_parsed_data.full_body.length() > 0)
                    NEW_BUFFER_SIZE = server_config[sd].req_parsed_data.content_length - server_config[sd].req_parsed_data.full_body.length() + 1;
                char buffer[NEW_BUFFER_SIZE];
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
                if (sd != 0 && server_config[sd].req_parsed_data.ifnotvalid_code_status != 0) {
                    response_errors(sd, server_config[sd].req_parsed_data.ifnotvalid_code_status, server_config[sd].conf);
                    close(sd);
                    client_socket[i] = 0;
                    response.clear_httprequest(server_config[sd].req_parsed_data);
                    server_config[sd].request_str = "";
                    server_config.erase(sd);
                    continue;
                }
                if (server_config[sd].req_parsed_data.method == POST &&  server_config[sd].req_parsed_data.is_chunked == true) {
                    if (server_config[sd].req_parsed_data.chunked_end != 1) {
                        FD_CLR(sd, &write_fds);
                        continue;
                    }
                } else if (server_config[sd].req_parsed_data.method == POST) {
                    if (server_config[sd].req_parsed_data.content_length > static_cast<int>(server_config[sd].req_parsed_data.full_body.length())) {
                        FD_CLR(sd, &write_fds);
                        continue;
                    }
                }
                response.handle_request(server_config[sd].req_parsed_data, sd, server_config[sd].conf);
                close(sd);
                client_socket[i] = 0;
                response.clear_httprequest(server_config[sd].req_parsed_data);
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


Server::~Server() {}
