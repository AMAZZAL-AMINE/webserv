/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamazzal <mamazzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 13:40:05 by mamazzal          #+#    #+#             */
/*   Updated: 2024/01/20 12:48:43 by mamazzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../main.h"

Config::Config() {}

std::string grepValue(std::string & key, std::string target) {
  size_t pos =0;
  while (pos < key.length() &&  key[pos] == ' ')
    pos++;
  pos += std::string(target).length();
  while (pos < key.length() &&  key[pos] == ' ')
    pos++;
  if (pos >= key.length() - 1)
    throw confFileError();
  size_t end_pos =  key.find(";") - pos;
  if (end_pos + pos == SIZE_T_MAX)
    throw confFileError();
  std::string port = key.substr(pos, end_pos);
  return port;
}

void Config::parsConfigFile(std::string confFile) {
  std::ifstream file;
  std::string line;
  file.open(confFile);
  if (!file)
    throw  confFileError();
  size_t server_position;
  while (std::getline(file, line)) {
    std::vector<t_config> conf;
    t_config s_conf;
    server_position = line.find("server {");
    if (server_position != SIZE_T_MAX && server_position == 0) {
      while (std::getline(file, line) && line.find("}") == SIZE_T_MAX) {
        if (line.find("port") == 2)
          s_conf.port = _atoi_(grepValue(line, "port"));
        else if (line.find("server_name") == 2)
          s_conf.server_name =  grepValue(line, "server_name");
        else if (line.find("host_name") == 2)
           s_conf.host_name =  grepValue(line, "host_name");
        else if (line.find("error_page_404") == 2)
          s_conf.error404 =  grepValue(line, "error_page_404");
        else if (line.find("error_page_500") == 2)
          s_conf.error500 =  grepValue(line, "error_page_500");
        else if (line.find("error_page_408") == 2)
          s_conf.error408 =  grepValue(line, "error_page_408");
        else if (line.find("error_page_400") == 2)
          s_conf.error400 =  grepValue(line, "error_page_400");
        else if (line.find("error_page_413") == 2)
          s_conf.error413 =  grepValue(line, "error_page_413");
        else if (line.find("error_page_403") == 2)
          s_conf.error403 =  grepValue(line, "error_page_403");
        else if (line.find("error_page_405") == 2)
          s_conf.error405 =  grepValue(line, "error_page_405");
        else if (line.find("error_page_501") == 2)
          s_conf.error501 =  grepValue(line, "error_page_501");
        else if (line.find("max_body_size") == 2)
          s_conf.max_body_size =  _atoi_(grepValue(line, "max_body_size"));
        else if (line.find("autoindex") == 2)
          s_conf.autoindex =  grepValue(line, "autoindex");
        else if (line.find("cgi_path") == 2)
          s_conf.cgi_path =  grepValue(line, "cgi_path");
        else if (line.find("root") == 2)
          s_conf.root =  grepValue(line, "root");
        else
           throw confFileError();
      }
      if (!s_conf.server_name.empty())
        conf.push_back(s_conf);
    }
    if (!conf.empty())
      this->http_config.push_back(conf);
  }
}


const std::vector<std::vector<t_config> > &  Config::getConfig() const {
  return this->http_config;
}

Config::~Config() {}