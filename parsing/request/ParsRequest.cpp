#include "../../main.h"

char hexToChar(const std::string & hex) {
    std::istringstream iss(hex);
    int value;
    iss >> std::hex >> value;
    return static_cast<char>(value);
}

std::string urlDecode(const std::string& input) {
    std::string result;
    result.reserve(input.length());

    for (std::size_t i = 0; i < input.length(); ++i) {
        if (input[i] == '%' && i + 2 < input.length()) {
            std::string hex = input.substr(i + 1, 2);
            result += hexToChar(hex);
            i += 2;
        }else if (input[i] == '+')
            result += ' ';
        else
          result += input[i];
    }

    return result;
}

std::map<std::string, std::string> get_header(std::string & key, HttpRequest & httpRequest) {
  for (std::map<std::string, std::string>::iterator it = httpRequest.headers.begin(); it != httpRequest.headers.end(); it++) {
    if (it->first == key)
      return httpRequest.headers;
  }
  std::map<std::string, std::string> empty_map;
  return empty_map;
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

std::string get_boundary_value(const std::string & request) {
  std::string boundary = "";
  int start = request.find("boundary=") + 9;
  while (request[start] != '\r' && request[start] != '\n' && request[start] != '\0') {
    boundary += request[start];
    start++;
  }
  return boundary;
}

void pars_post_request(std::istringstream & stream, HttpRequest & __unused httpRequest) {
  std::string line;
  std::getline(stream, line);
  int conut = 0;
  while (1)  {
    std::getline(stream, line);
    if (line.find("boundary=-----") != SIZE_T_MAX && conut != 0)
      break;
    conut++;
  }
}

std::string get_key(std::string & line) {
  std::string key = "";
  int start = 0;
  while (line[start] != ':' && line[start] != '\0') {
    key += line[start];
    start++;
  }
  return key;
}

std::string get_value(std::string & line) {
  std::string value = "";
  int start = line.find(":") + 2;
  while (line[start] != '\0' && line[start] != '\r' && line[start] != '\n') {
    value += line[start];
    start++;
  }
  return value;
}

std::map<std::string, std::string> get_headers(std::istringstream & stream) {
  std::map<std::string, std::string> headers;
  std::string header;
  int count = 0;
  while (std::getline(stream, header)) {
    if (header == "\r" && count == 0)
      continue;
    else if (header == "\r" && count != 0)
      break;
    std::string key = get_key(header);
    std::string value = get_value(header);
    headers[key] = value;
    count++;
  }
  return headers;
}

std::string get_name(std::string & line) {
  int start = line.find("name=") + 6;
  std::string name = "";
  while (line[start] != '\"' && line[start] != '\r' && line[start] != '\n' && line[start] != '\0') {
    name += line[start];
    start++;
  }
  return name;
}


void split_body_encrypted_multi_form_data(HttpRequest & httpRequest, std::istringstream & stream) {
  std::string line;
  std::getline(stream, line);
  if (!stream)
    return;
  while (line.find(httpRequest.boundary_start) == SIZE_T_MAX)
    std::getline(stream, line);
  std::string form_data;
  line = "";
  while (std::getline(stream, form_data)) {
    if (form_data.find("filename=") != SIZE_T_MAX)
      httpRequest.file_name.push_back(get_file_name(form_data));
    else if (form_data.find("Content-Type:") != SIZE_T_MAX) {
        httpRequest.content_names.push_back(get_name(form_data));
        httpRequest.content_type.push_back(std::string("file_upload"));
    }else if (form_data.find("Content-Disposition: form-data") != SIZE_T_MAX) {
      httpRequest.content_names.push_back(get_name(form_data));
      httpRequest.content_type.push_back(std::string("text"));
    }
    else if (form_data.find(httpRequest.boundary_end) != SIZE_T_MAX) {
      httpRequest.form_data.push_back(line);
      break;
    }
    else if (form_data.find("--" + httpRequest.boundary_start) != SIZE_T_MAX) {
      httpRequest.form_data.push_back(line);
      line = "";
      form_data = "";
    }
    else {
      if (form_data != "\r")
        line += form_data + "\n";
    }
  }
}

void split_body(HttpRequest & httpRequest, std::istringstream & stream) {
  std::string line = "";
  std::string body = "";
  int count = 0;
  while(std::getline(stream, line)) {
    if (line == "\r" && count == 0)
      continue;
    else if (line == "\r" && count != 0)
      break;
    body += line + "\n";
    count++;
  }
  //split body key name  and value
  std::string key = "";
  std::string value = "";
  int start = 0;
  while (body[start] != '\0') {
    if (body[start] == '=') {
      start++;
      while (body[start] != '&' && body[start] != '\0') {
        value += body[start];
        start++;
      }
      key = httpRequest.if_post_form_type == TEXT_PLAIN ? key : urlDecode(key);
      httpRequest.content_names.push_back(key);
      httpRequest.content_type.push_back(std::string("text"));
      value = httpRequest.if_post_form_type == TEXT_PLAIN ? value : urlDecode(value);
      httpRequest.form_data.push_back(value);
      key = "";
      value = "";
    }
    else if (body[start] == '&') {
      start++;
      while (body[start] != '=' && body[start] != '\0') {
        key += body[start];
        start++;
      }
    }
    else {
      key += body[start];
      start++;
    }
  }
}

void split_body_text_plain(HttpRequest & httpRequest, std::istringstream & stream) {
  std::string key = "";
  std::string value = "";
  std::string line = "";
  int count = 0;
  int index = 0;
  if (!stream)
    return;
  while(std::getline(stream, line)) {
    if (line == "\r" && count == 0)
      continue;
    while (size_t(index) < line.length() && line[index] != '=') {
        key += line[index];
        index++;
    }
    index++;
    while (size_t(index) < line.length() && line[index] != '\0' && line[index] != '\r') {
        value.append(1, line[index]);
        index++;
    }
    httpRequest.content_names.push_back(key);
    httpRequest.content_type.push_back(std::string("text"));
    httpRequest.form_data.push_back(value);
    key = "";
    value = "";
    index = 0;
    count++;
  }
}

void handel_method_post(std::istringstream & stream, HttpRequest & httpRequest) {
  if (!stream)
    return;
  std::string type = "Content-Type";
  std::map<std::string, std::string> content_type_bound = get_header(type, httpRequest);
  httpRequest.content_length = _atoi_(httpRequest.headers["Content-Length"]);
  httpRequest.boundary_start = get_boundary_value(content_type_bound[type]);
  httpRequest.boundary_end = httpRequest.boundary_start + "--";
  std::string target = "Content-Type";
  std::string content_type = get_header(target, httpRequest)[target];
  if (content_type.find("multipart/form-data") != SIZE_T_MAX) {
    httpRequest.if_post_form_type = FORM_DATA;
    split_body_encrypted_multi_form_data(httpRequest, stream);
  }else if (content_type.find("application/x-www-form-urlencoded") != SIZE_T_MAX) {
    httpRequest.if_post_form_type = DEFAULT_FORM;
    split_body(httpRequest, stream);
  }else if (content_type.find("text/plain") != SIZE_T_MAX) {
    httpRequest.if_post_form_type = TEXT_PLAIN;
    split_body_text_plain(httpRequest, stream);
  }
}

void pars_post_query(std::string query, HttpRequest & httpRequest) {
  std::string key = "";
  std::string value = "";
  for (size_t i = 0; i < query.length(); i++) {
    while (i < query .length() && query[i] != '=') {
      key += query[i];
      i++;
    }
    i++;
    if (i >= query.length())
      break;
    while (i < query.length() && query[i] != '&')  {
      value += query[i];
      i++;
    }
    httpRequest.content_names.push_back(urlDecode(key));
    httpRequest.content_type.push_back(std::string("text"));
    httpRequest.form_data.push_back(urlDecode(value));
    key = "";
    value = "";
  }
}


void split_chunked_body(std::istringstream & stream, HttpRequest & __unused httpRequest) {
  int  __unused count = 0;
  std::string __unused body = "";
  std::string line = "";
  if (!stream)
    return;
  while (std::getline(stream, line)) {
    if (count == 0 && line == "\r") {
      count = 0;
      continue;
    }
    else if (count % 2 != 0) {
      //erase /r/n
      if (line !=  "\r" && line != "\n" && line != "") {
        line.erase(line.length() - 1, 1);
        body += line + "&";
      }
    }
    else if (count % 2 == 0 && line == "\r")
      break;
    count++;
  }

  std::string key = "";
  std::string value = "";
  size_t start = 0;
  while (start < body.length()) {
    while (start < body.length() && body[start] != '=') {
      key += body[start];
      start++;
    }
    start++;
    if (start >= body.length())
      break;
    while (start < body.length() && body[start] != '\0' && body[start] != '&') {
      value += body[start];
      start++;
    }
    httpRequest.content_names.push_back(key);
    httpRequest.content_type.push_back(std::string("text"));
    httpRequest.form_data.push_back(value);
    key = "";
    value = "";
    start++;
  }
}


void parst_get_query(std::string query, HttpRequest & httpRequest) {
  std::string key = "";
  std::string value = "";
  for (size_t i = 0; i < query.length(); i++) {
    while (i < query.length() && query[i] != '=') {
      key += query[i];
      i++;
    }
    i++;
    if (i >= query.length())
      break;
    while (i < query.length() && query[i] != '&')  {
      value += query[i];
      i++;
    }
    httpRequest.query_params[urlDecode(key)] = urlDecode(value);
    key = "";
    value = "";
  }
}

int is_valid_request(HttpRequest & httpRequest) {
  httpRequest.is_valid = false;
  if (httpRequest.method == "POST") {
    std::string content_type = "Transfer-Encoding";
    std::map<std::string, std::string> head = get_header(content_type, httpRequest);
    if (head[content_type] != "chunked") {
      return (httpRequest.ifnotvalid_code_status = 501, -1);
    }
    else if (httpRequest.headers["Transfer-Encoding"] == "" &&  httpRequest.headers["Content-length"] == "") {
      httpRequest.ifnotvalid_code_status = 400;
      return -1;
    }
  }
  httpRequest.is_valid = true;
  return 1;
}

HttpRequest parseHttpRequest(const std::string & request) {
  HttpRequest httpRequest;
  std::istringstream stream(request);
  stream >> httpRequest.method >> httpRequest.path >> httpRequest.version;
  httpRequest.headers = get_headers(stream);
  httpRequest.is_valid = true;
  if (is_valid_request(httpRequest) == -1)
    return httpRequest;
  if (httpRequest.method == "POST") {
    if (httpRequest.headers["Transfer-Encoding"] == "chunked")  {
      httpRequest.is_chunked = true;
      split_chunked_body(stream, httpRequest);
    }else {
      if (httpRequest.path.find("?") != SIZE_T_MAX) {
        std::string query = httpRequest.path.substr(httpRequest.path.find("?") + 1);
        pars_post_query(query, httpRequest);
        httpRequest.path = httpRequest.path.substr(0, httpRequest.path.find("?"));
      }
      handel_method_post(stream, httpRequest);
    }
    httpRequest.has_query = false;
    httpRequest.has_body = true;
    httpRequest.is_ency_upl_file = true;
  }
  else {
    httpRequest.has_body = false;
    httpRequest.is_chunked = false;
    httpRequest.has_query = false;
    if (httpRequest.path.find("?") != SIZE_T_MAX) {
      std::string query = httpRequest.path.substr(httpRequest.path.find("?") + 1);
      parst_get_query(query, httpRequest);
      httpRequest.path = httpRequest.path.substr(0, httpRequest.path.find("?"));
      httpRequest.has_query = true;
    }
  }
  return httpRequest;
}
