#include "../../main.h"


std::string get_method(std::string gg) {
  std::string data;
  for (int i = 0; gg[i] != ' '; i++) {
    if (gg[i] == '\0')
      return "error";
    data += gg[i];
  }
  return data;
}

std::string get_path(std::string gg, int start) {
  std::string data;
  for (int i = start; gg[i] != ' '; i++) {
    if (gg[i] == '\0')
      return "error";
    data += gg[i];
  }
  return data;
}

std::string get_http_version(std::string gg, int start) {
  std::string data;
  for (int i = start; gg[i] != '\n'; i++) {
    if (gg[i] == '\0')
      return "error";
    data += gg[i];
  }
  return data;
}

std::string get_host(std::string gg, int start) {
  std::string data;
  int i;
  for (i = start; gg[i] != '\n'; i++) {
    if (gg[i] == ' ')
      i++;
    if (gg[i] == '\0')
      return "error";
    data += gg[i];
  }
  return data;
}

std::string get_conection(std::string gg, int start) {
  std::string data;
  int i;
  for (i = start; gg[i] != ' '; i++) {
    if (gg[i] == '\0')
      return "error";
    data += gg[i];
  }
  i++;
  // for (int j = i; gg[j] != '\n'; j++) {
  //   if (gg[j] == '\0')
  //     return "error";
  //   data += gg[j];
  // }
  return data;
}

std::string get_user_agent(std::string gg, int start) {
  std::string data;
  int i;
  for (i = start; gg[i] != ' '; i++) {
    if (gg[i] == '\0')
      return "error";
  }
  i++;
  for (int j = i; gg[j] != '\n'; j++) {
    if (gg[j] == '\0')
      return "error";
    data += gg[j];
  }
  return data;
}

t_request pars(std::string gg) {
  t_request data;
  data.method = get_method(gg);
  data.path = get_path(gg, data.method.size() + 1);
  data.http_version = get_http_version(gg, data.method.size() + data.path.size() + 2);
  data.host = get_host(gg, data.method.size() + data.path.size() + data.http_version.size() + 3);
  data.connection = get_conection(gg, data.method.size() + data.path.size() + data.http_version.size() + data.host.size() + 4);
  data.user_agent = get_user_agent(gg, data.method.size() + data.path.size() + data.http_version.size() + data.host.size() + data.connection.size() + 5);
  return data;
}