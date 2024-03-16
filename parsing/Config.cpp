/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamazzal <mamazzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 13:40:05 by mamazzal          #+#    #+#             */
/*   Updated: 2024/03/16 16:14:27 by mamazzal         ###   ########.fr       */
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

std::string grepLocationValue(std::string line) {
  if (std::string("location").length() + 2 >= line.length())
    throw confFileError();
  std::string location = "";
  for (size_t i = std::string("location").length() + 3; i < line.length(); i++) {
    if (line[i] == ' ' || line[i] == '{')
      break;
    location += line[i];
  }
  return location;
}

std::vector<E_METHOD> split_methods(std::string methods) {
  std::vector<E_METHOD> methods_;
  std::vector<std::string> methods_str = split_string(methods, " ");
  for (size_t i = 0; i < methods_str.size(); i++) {
    if (methods_str[i] == "GET")
      methods_.push_back(GET);
    else if (methods_str[i] == "POST")
      methods_.push_back(POST);
    else if (methods_str[i] == "DELETE")
      methods_.push_back(DELETE);
    else
      throw MethodsException();
  }
  return methods_;
}

void check_methods(std::vector<E_METHOD> methods) {
  if (methods.size() == 0)
    throw MethodsException();
  for (size_t i = 0; i < methods.size(); i++) {
    for (size_t j = i + 1; j < methods.size(); j++) {
      if (methods[i] == methods[j])
        throw MethodsException();
    }
    if (methods[i] != GET && methods[i] != POST && methods[i] != DELETE)
      throw MethodsException();
  }
}

t_rederection get_derection(std::string line) {
  t_rederection rederection;
  for (size_t i = 0; i < line.length(); i++) {
    if (line[i] == ' ')
      break;
    rederection.old_location += line[i];
  }
  for (size_t i = rederection.old_location.length() + 1; i < line.length(); i++) {
    if (line[i] == ' ')
      break;
    rederection.new_location_to_redirect += line[i];
  }
  rederection.code = _atoi_(line.substr(rederection.old_location.length() + rederection.new_location_to_redirect.length() + 2));
  return rederection;
}

std::vector<int> get_ports(std::string  line) {
  std::vector<std::string> str_ports = split_string(line, " ");
  std::vector<int> ports;
  for (size_t i = 0; i < str_ports.size(); i++)
    ports.push_back(_atoi_(str_ports[i]));
  return ports;
}

std::string clean_upload_key(std::string dir) {
  if (dir[0] == '/')
    dir.erase(0, 1);
  if (dir[dir.length() - 1] == '/')
    dir.erase(dir.length() - 1, 1);
  dir = "/" + dir + "/";
  return dir;
}

t_location get_location(std::ifstream & file, std::string & line) {
  t_location location;
  if (line.find("location") == 2) {
    location.location = grepLocationValue(line);
    while (std::getline(file, line) && line.find("}") == SIZE_T_MAX) {
      if (line.find("root") == 4)
        location.root = grepValue(line, "root");
      else if (line.find("autoindex") == 4)
        location.autoindex = grepValue(line, "autoindex");
      else if (line.find("cgi_path") == 4)
        location.cgi_path = grepValue(line, "cgi_path");
      else if (line.find("error_page_404") == 4)
        location.error404 = grepValue(line, "error_page_404");
      else if (line.find("error_page_500") == 4)
        location.error500 = grepValue(line, "error_page_500");
      else if (line.find("error_page_408") == 4)
        location.error408 = grepValue(line, "error_page_408");
      else if (line.find("error_page_400") == 4)
        location.error400 = grepValue(line, "error_page_400");
      else if (line.find("error_page_413") == 4)
        location.error413 = grepValue(line, "error_page_413");
      else if (line.find("error_page_403") == 4)
        location.error403 = grepValue(line, "error_page_403");
      else if (line.find("error_page_405") == 4)
        location.error405 = grepValue(line, "error_page_405");
      else if (line.find("error_page_501") == 4)
        location.error501 = grepValue(line, "error_page_501");
      else if (line.find("error_page_409") == 4)
        location.error409 = grepValue(line, "error_page_409");
      else if (line.find("max_body_size") == 4)
        location.max_body_size = _atoi_(grepValue(line, "max_body_size"));
      else if (line.find("index") == 4) {
        std::string index = grepValue(line, "index");
        location.index = split_string(index, " ");
      }else if (line.find("upload_dir") == 4)
        location.upload_dir = clean_upload_key(grepValue(line, "upload_dir"));
      else if (line.find("methods") == 4) {
        std::string methods = grepValue(line, "methods");
        location.methods = split_methods(methods);
      } else if (line.find("redirect") == 4) {
          std::string methods = grepValue(line, "redirect");
          location.rederection = get_derection(methods);
      }else if (line.find("alias") == 4)
        location.alias = grepValue(line, "alias");
    }
  } 
  return location;
}



void Config::parsConfigFile(std::string confFile) {
  std::ifstream file;
  std::string line;
  file.open(confFile);
  if (!file)
    throw  confFileError();
  size_t server_position;
  while (std::getline(file, line)) {
    t_config s_conf;
    server_position = line.find("server {");
    if (server_position != SIZE_T_MAX && server_position == 0) {
      while (std::getline(file, line) && line.find("}") == SIZE_T_MAX) {
        if (line.find("ports") == 2)
          s_conf.ports = get_ports(grepValue(line, "ports"));
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
        else if (line.find("error_page_409") == 2)
          s_conf.error409 =  grepValue(line, "error_page_409");
        else if (line.find("max_body_size") == 2)
          s_conf.max_body_size =  _atoi_(grepValue(line, "max_body_size"));
        else if (line.find("autoindex") == 2)
          s_conf.autoindex =  grepValue(line, "autoindex");
        else if (line.find("cgi_path") == 2)
          s_conf.cgi_path =  grepValue(line, "cgi_path");
        else if (line.find("root") == 2)
          s_conf.root =  grepValue(line, "root");
        else if (line.find("location") == 2)
          s_conf.locations.push_back(get_location(file, line));
        else if (line.find("index") == 2) {
          std::string index = grepValue(line, "index");
          s_conf.index = split_string(index, " ");
        }else if (line.find("upload_dir") == 2)
          s_conf.upload_dir =  clean_upload_key(grepValue(line, "upload_dir"));
        else if (line.find("methods") == 2) {
          std::string methods = grepValue(line, "methods");
          s_conf.methods = split_methods(methods);
        } else if (line.find("redirect") == 2) {
          std::string methods = grepValue(line, "redirect");
          s_conf.rederection = get_derection(methods);
        }
      }
    }
    if (!s_conf.server_name.empty()) {
      check_methods(s_conf.methods);
      
      s_conf.IsDefault = true;
      this->http_config.push_back(s_conf);
    }
  }
  file.close();
}


const std::vector<t_config> &  Config::getConfig() const {
  return this->http_config;
}

Config::~Config() {}