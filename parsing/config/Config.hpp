/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouali <rouali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 20:56:18 by mamazzal          #+#    #+#             */
/*   Updated: 2024/04/20 16:25:21 by rouali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "../../main.h"

typedef struct t_rederection {
  std::string old_location;
  std::string new_location_to_redirect;
  int         code;
} t_rederection;

typedef struct t_location {
  std::string location_name;
  std::map<std::string, std::string> location;
} t_location;



typedef struct t_config {
  std::map<std::string, std::string> Config;
  std::vector<t_location> location;
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