/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamazzal <mamazzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 10:44:16 by mamazzal          #+#    #+#             */
/*   Updated: 2024/04/29 17:02:49 by mamazzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "server/Server.hpp"

int main(int argc, char __unused **argv) {
  if (argc != 2) {
    std::cerr << "Error : config file\n";
    return 1;
  }
  try {
    SyntaxError(argv[1]);
    Server server;
    server.parsConfigFile(argv[1]);
    server.runServer();
  } catch (std::exception & e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}
