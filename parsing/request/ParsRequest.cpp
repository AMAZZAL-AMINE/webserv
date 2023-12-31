#include "../../main.h"


t_request pars(std::string gg) {
  t_request data;

  data.method = strdup(gg.substr(0, gg.find(" ")).c_str());
  gg.erase(0, gg.find(" ") + 1);
  data.path = strdup(gg.substr(0, gg.find(" ")).c_str());
  gg.erase(0, gg.find(" ") + 1);
  data.version = strdup(gg.substr(0, gg.find("\r\n")).c_str());
  gg.erase(0, gg.find("\r\n") + 2);
  data.host = strdup(gg.substr(0, gg.find("\r\n")).c_str());
  gg.erase(0, gg.find("\r\n") + 2);
  data.user_agent = strdup(gg.substr(0, gg.find("\r\n")).c_str());
  gg.erase(0, gg.find("\r\n") + 2);
  data.accept = strdup(gg.substr(0, gg.find("\r\n")).c_str());
  gg.erase(0, gg.find("\r\n") + 2);
  data.accept_language = strdup(gg.substr(0, gg.find("\r\n")).c_str());
  gg.erase(0, gg.find("\r\n") + 2);
  data.accept_encoding = strdup(gg.substr(0, gg.find("\r\n")).c_str());
  gg.erase(0, gg.find("\r\n") + 2);
  data.connection = strdup(gg.substr(0, gg.find("\r\n")).c_str());
  gg.erase(0, gg.find("\r\n") + 2);
  data.cookie = strdup(gg.substr(0, gg.find("\r\n")).c_str());
  gg.erase(0, gg.find("\r\n") + 2);
  data.content_type = strdup(gg.substr(0, gg.find("\r\n")).c_str());
  gg.erase(0, gg.find("\r\n") + 2);
  data.body = strdup(gg.substr(0, gg.find("\r\n")).c_str());
  return data;
}