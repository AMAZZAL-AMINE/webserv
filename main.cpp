/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamazzal <mamazzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 10:44:16 by mamazzal          #+#    #+#             */
/*   Updated: 2023/12/29 11:03:05 by mamazzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void display_config(std::vector<std::vector<t_config> > http_config) {
  std::vector<std::vector<t_config> >::iterator start = http_config.begin();
  for (; start != http_config.end(); start++) {
    std::vector<t_config> conf_data = *start;
    for (size_t j = 0; j < conf_data.size(); j++) {
      t_config conf = conf_data[j];
      std::cout << "SERVER_NAME : " << conf.server_name + "\n";
      std::cout << "PORT : " << conf.port << "\n";
    }
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
    Server server;
    display_config(sttp_pars.getConfig());
    server.serve();
  } catch (std::exception & e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}