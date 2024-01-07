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
      httpRequest.body+= body;
    }
  }else {
    while (std::getline(stream, body)) {
      httpRequest.body += body + "\n";
    }
  }
  if (httpRequest.body.size() == 0)
    httpRequest.has_body = false;
  else
    httpRequest.has_body = true;
}

std::string get_file_name(const std::string & requestBody) {
  int start = requestBody.find("filename=") + 10;
  std::string filename  = "";
  while (requestBody[start] != '"' && requestBody[start] != '\r' && requestBody[start] != '\n' && requestBody[start] != '\0') {
    filename += requestBody[start];
    start++;
  }
  return filename;
}

void get_form_data(std::istringstream & stream, HttpRequest & httpRequest) {
  std::string form_data;
  while (std::getline(stream, form_data)) {
    if (form_data == "\r")
      break;
    httpRequest.form_data += form_data + "\n";
  }
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
    httpRequest.is_ency_upl_file = false;
    if (httpRequest.method == "POST") {
      if (request.find("Transfer-Encoding: chunked") != SIZE_T_MAX)
        httpRequest.is_chunked = true;
      get_body(stream, httpRequest);
      if (request.find("multipart/form-data") != SIZE_T_MAX) {
        httpRequest.is_ency_upl_file = true;
        httpRequest.file_name = get_file_name(request);
        get_form_data(stream, httpRequest);
      }
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
