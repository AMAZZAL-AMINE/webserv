/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamazzal <mamazzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 20:56:18 by mamazzal          #+#    #+#             */
/*   Updated: 2024/01/16 16:02:23 by mamazzal         ###   ########.fr       */
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
  std::string error404;
  std::string error500;
  std::string error408;
  std::string error400;
  std::string error413;
  std::string error403;
  std::string error405;
} t_config;

class Config {
  private :
    std::vector<std::vector<t_config> > http_config;
  public :
    Config();
    void parsConfigFile(std::string confFile);
    const std::vector<std::vector<t_config> > & getConfig() const;
    ~Config();
};

#endif