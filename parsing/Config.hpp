/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamazzal <mamazzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 20:56:18 by mamazzal          #+#    #+#             */
/*   Updated: 2024/01/20 12:45:13 by mamazzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "../main.h"

typedef struct t_config {
  //config
  int port;
  std::string root;
  std::string server_name;
  std::string host_name;
  int         max_body_size;
  //error_page
  std::string autoindex;
  std::string cgi_path;
  std::string error404;
  std::string error500;
  std::string error408;
  std::string error400;
  std::string error413;
  std::string error403;
  std::string error405;
  std::string error501;
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