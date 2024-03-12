/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamazzal <mamazzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 20:56:18 by mamazzal          #+#    #+#             */
/*   Updated: 2024/03/12 12:34:02 by mamazzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "../main.h"

typedef struct t_rederection {
  std::string old_location;
  std::string new_location_to_redirect;
  int         code;
} t_rederection;

typedef struct t_location {
  int      port;
  std::string location;
  std::string root;
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
  std::string error409;
  std::string upload_dir;
  std::vector<std::string> index;
  std::vector<E_METHOD> methods;
  int         max_body_size;
  t_rederection rederection;
} t_location;



typedef struct t_config {
  //config
  std::string location;
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
  std::string error409;
  std::string upload_dir;
  std::vector<t_location> locations;
  std::vector<E_METHOD> methods;
  std::vector<std::string> index;
  std::vector<int> ports;
  t_rederection rederection;
} t_config;

class Config {
  private :
    std::vector<t_config> http_config;
  public :
    Config();
    void parsConfigFile(std::string confFile);
    const std::vector<t_config> & getConfig() const;
    ~Config();
};

#endif