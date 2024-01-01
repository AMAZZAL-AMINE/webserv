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

t_request pars(std::string gg) {
  t_request data;
  data.method = get_method(gg);
  data.path = get_path(gg, data.method.size() + 1);
  data.http_version = get_http_version(gg, data.method.size() + data.path.size() + 2);
  return data;
}