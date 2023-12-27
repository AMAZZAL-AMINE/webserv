/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamazzal <mamazzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 20:56:18 by mamazzal          #+#    #+#             */
/*   Updated: 2023/12/27 15:39:03 by mamazzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "../main.h"

typedef struct t_config {
  int port;
  std::string server_name;
  std::string host_name;
  int         id;
} t_config;

class Config {
  private :
    std::vector<std::vector<t_config> > http_config;
  public :
    Config();
    void parsConfigFile(std::string confFile);
    ~Config();
};

#endif