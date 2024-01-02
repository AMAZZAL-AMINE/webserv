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
  int j;
  for (j = start; gg[j] != ' '; j++)
  {
  }
  j++;
  for (int i = j; gg[i] != '\n'; i++) {
    if (gg[i] == '\0')
      return "error";
    data += gg[i];
  }
  return data;
}

std::string get_conection(std::string gg, int start) {
  std::string data;
  int j;
  for (j = start; gg[j] != ' '; j++)
  {
  }
  j++;
  for (int i = j; gg[i] != '\n'; i++) {
    if (gg[i] == '\0')
      return "error";
    data += gg[i];
  }
  return data;
}

std::string get_(std::string gg, int start) 
{
  std::string data;
    int j = 0;
    int pos = start;
    for (j = start; gg[j] != '\n'; j++) {
      pos++;
    }
    j++;
    pos++;
    for (int i = j; gg[i] != ' '; i++)
    {
      pos++;
    }
    j++;
    pos++;
    for (int i = pos; gg[i] != '\n'; i++)
    {
        if (gg[i] == '\0') {
            return "error";
        }
        data += gg[i];
    }
    return data;
}

t_request pars(std::string gg)
{
  t_request data;
  int size = 0;
  data.method = get_method(gg);
  size = data.method.size();
  data.path = get_path(gg, size + 1);
  size += data.path.size();
  data.http_version = get_http_version(gg, size + 1);
  size += data.http_version.size();
  data.host = get_host(gg, size + 1);
  size += data.host.size();
  data.connection = get_conection(gg, size + 1);


  size += data.connection.size();
  data.user_agent = get_(gg, size + 1);
  size += data.user_agent.size();
  data.accept = get_(gg, size + 1);
  size += data.accept.size();
  data.Sec_GPC = get_(gg, size + 1);
 
  return data;
}