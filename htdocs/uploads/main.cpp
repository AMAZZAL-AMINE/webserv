/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamazzal <mamazzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 10:44:16 by mamazzal          #+#    #+#             */
/*   Updated: 2024/05/01 18:50:54 by mamazzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "server/Server.hpp"

int main(int argc, char __unused **argv) {
  try {
    if (argc >= 2)
      SyntaxError(argv[1]);
    Server server;
    if (argc >= 2)
      server.parsConfigFile(argv[1]);
    else
      server.generateDefaultConfig();
    server.runServer();
  } catch (std::exception & e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}
