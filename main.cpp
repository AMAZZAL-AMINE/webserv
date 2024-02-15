/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamazzal <mamazzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 10:44:16 by mamazzal          #+#    #+#             */
/*   Updated: 2024/01/24 17:30:46 by mamazzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void display_config(std::vector<t_config> http_config) {
  std::vector<t_config>::iterator start = http_config.begin();
  for (; start != http_config.end(); start++) {
    t_config conf = *start;
    std::cout << "SERVER_NAME : " << conf.server_name + "\n";
    std::cout << "PORT : " << conf.port << "\n";
    std::cout << "++++++\n";
  }
}



int main(int argc, char __unused **argv) {
  if (argc != 2) {
    std::cerr << "Error : config file\n";
    return 1;
  }
  try {
    Config sttp_pars;
    sttp_pars.parsConfigFile(argv[1]);
    std::vector<t_config> http_config = sttp_pars.getConfig();
    Server server(http_config.at(0));
    server.serve(http_config);
  } catch (std::exception & e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}
