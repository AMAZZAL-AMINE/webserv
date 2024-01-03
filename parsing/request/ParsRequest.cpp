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

HttpRequest parseHttpRequest(const std::string & request) {
  HttpRequest httpRequest;
   httpRequest.is_valid = true;
  if (is_request_valid(request) == false) {
    httpRequest.is_valid = false;
    httpRequest.ifnotvalid_code_status = 400;
    return httpRequest;
  }
  std::istringstream stream(request);
  stream >> httpRequest.method >> httpRequest.path >> httpRequest.version;
  //check is chunked
  if (httpRequest.method == "POST" && request.find("Transfer-Encoding: chunked") != SIZE_T_MAX)
    httpRequest.is_chunked = true;
  std::string content_length = request.substr(request.find("Content-Length:") + 16);
  httpRequest.content_length = std::stoi(content_length);
  std::string header;
  int start = 0;
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

  //todo : check if body is chunked
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
  return httpRequest;
}
