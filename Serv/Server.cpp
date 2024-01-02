/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouali <rouali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 16:19:54 by rouali            #+#    #+#             */
/*   Updated: 2023/12/30 12:54:41 by rouali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <fstream>

// Server::Server()
// {
// }

// Server::Server(const std::string &configFile) : ConfigFile(configFile), _sockets(0)
// {
//     int port = 8080;
//     _sockets = _socket(port);
//     std::string find = ".conf";
//     if (configFile.find(find) == std::string::npos)
//         throw std::runtime_error("Invalid config file");
//     std::ifstream file(configFile);
    
//     if (!file.is_open())
//         throw std::runtime_error("Error open file");
//     std::string line;
//     while (std::getline(file, line))
//         std::cout << line << std::endl;
//     file.close();
// }

// Server::~Server()
// {
// }

// void Server::run()
// {
// }

// int Server::_socket(int port)
// {
//     std::cout << "Socket created Port: " << port << std::endl;
//     return (0);
// }


// Server::Server() : port(80), host("localhost"), defaultServer(false)
// {
// }   
// void Server::setPort(int p)
// {
//     port = p;
// }

// void Server::setHost(const std::string& h)
// {
//     host = h;
// }

// void Server::setDefaultServer(bool isDefault)
// {
//     defaultServer = isDefault;
// }

// void Server::displayConfig() const
// {
//     std::cout << "Port: " << port << std::endl;
//     std::cout << "Host: " << host << std::endl;
//     std::cout << "Default Server: " << (defaultServer ? "Yes" : "No") << std::endl;
// }