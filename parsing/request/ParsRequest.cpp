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


t_request pars(std::string gg)
{
    t_request data;
    std::cout << gg << std::endl;
    data.method = get_method(gg);
    return data;
}

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

HttpRequest parseHttpRequest(const std::string & request) {
  HttpRequest httpRequest;
  if (request.find("Transfer-Encoding") != SIZE_T_MAX)
    std::cout << "HELLO DDD\n";
  std::istringstream stream(request);
  stream >> httpRequest.method >> httpRequest.path >> httpRequest.version;
  std::string header;
  while (std::getline(stream, header)) {
    if (header == "\r")
      continue;
    httpRequest.headers.push_back(header);
  }
  return httpRequest;
}
