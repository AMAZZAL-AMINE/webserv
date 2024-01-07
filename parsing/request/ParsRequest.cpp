#include "../../main.h"



bool is_request_valid(const std::string & request) {
  if (request.find("GET") == SIZE_T_MAX && request.find("POST") == SIZE_T_MAX)
    return false;
  if (request.find("HTTP/1.1") == SIZE_T_MAX)
    return false;
  if (request.find("Host") == SIZE_T_MAX)
    return false;
  return true;
}

void get_body(std::istringstream & stream, HttpRequest & httpRequest) {
  std::string body;
  if (httpRequest.is_chunked == true) {
    while (std::getline(stream, body)) {
      if (body == "\r")
        break;
      httpRequest.body.push_back(body);
    }
  }else {
    while (std::getline(stream, body)) {
      httpRequest.body.push_back(body);
    }
  }
  if (httpRequest.body.size() == 0)
    httpRequest.has_body = false;
  else
    httpRequest.has_body = true;
}


HttpRequest parseHttpRequest(const std::string & request) {
  HttpRequest httpRequest;
   httpRequest.is_valid = true;
  // std::cout << request << std::endl;
  // std::cout << "_____" << std::endl;
  // exit(0);
  if (is_request_valid(request) == false) {
    httpRequest.is_valid = false;
    httpRequest.ifnotvalid_code_status = 400;
    return httpRequest;
  }
  std::istringstream stream(request);
  stream >> httpRequest.method >> httpRequest.path >> httpRequest.version;
    int start = 0;
    std::string header;
    while (std::getline(stream, header)) {
      if (header == "\r") {
        if (start == 0)
          continue;
        else
          break;
      }
      start++;
      httpRequest.headers.push_back(header);
    }
    if (request.find("Content-Length:") != SIZE_T_MAX)
      httpRequest.content_length = _atoi_(request.substr(request.find("Content-Length:") + 16));
    else
      httpRequest.content_length = 0;
  if (httpRequest.method == "POST") {
    if (request.find("Transfer-Encoding: chunked") != SIZE_T_MAX)
      httpRequest.is_chunked = true;
    get_body(stream, httpRequest);
    httpRequest.has_query = false;
  }
  if (httpRequest.path.find("?") != SIZE_T_MAX) {
    std::string query = httpRequest.path.substr(httpRequest.path.find("?") + 1);
    httpRequest.path = httpRequest.path.substr(0, httpRequest.path.find("?"));
    httpRequest.has_query = true;
    httpRequest.query = query;
  }
  return httpRequest;
}
