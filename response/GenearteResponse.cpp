#include "Response.hpp"

std::string Response::generate_response_time() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *gmtime(&now);
    strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", &tstruct);
    return buf;
}


void Response::generateResponseFile(t_response & res, HttpRequest & req, int code, std::string  status, std::string  body, std::string  type) {
    std::string response;
    std::string head = req.version + " " + _itos_(code) + " " + status + "\r\n";
    head += "Date: " + this->generate_response_time() + "\r\n";
    head += "Server: " + res.config.Config["server_name"] + "\r\n";
    head += "Content-Length: " + _itos_(body.size()) + "\r\n";
    head += "Content-Type: " + type + "\r\n";
    head += "Connection : " + res.config.Config["Connection:"] + "\r\n";
    head += "\r\n";
    response = head + body;
    head.clear();
    send(res.client_fd, response.c_str(), response.size(), 0);
}

void Response::generateResponseMovement(t_response & res,HttpRequest & req, std::string & path, std::string  status, int  code) {
    std::string response;
    std::string head = req.version + " " + _itos_(code) + " " + status + "\r\n";
    head += "Date: " + this->generate_response_time() + "\r\n";
    head += "Server: " + res.config.Config["server_name"] + "\r\n";
    head += "Location: " + path + "\r\n";
    head += "Connection : " + res.config.Config["Connection:"] + "\r\n";
    head += "\r\n";
    response = head;
    head.clear();
    send(res.client_fd, response.c_str(), response.size(), 0);
}

std::string Response::defaultErrorPage(int & code, std::string & status_message) {
    std::string body = "<html><head><title>" + status_message + "</title></head><body><center><h1>" + _itos_(code) + " " + status_message + "</h1></center><hr><center>Webserver</center></body></html>";
    return body;
}

std::string readfile_(std::string path) {
    std::ifstream file(path);
    std::string content;
    if (file.is_open()) {
        std::string line;
        while (getline(file, line)) {
            content += line + "\n";
        }
        file.close();
    }
    return content;
}

void Response::errorResponse(t_response & res, HttpRequest & req, int code, std::string status) {
    std::string filename = res.config.Config["error_page_" + _itos_(code)];
    std::string body;
    if (filename.empty()) {
        body = this->defaultErrorPage(code, status);
        this->generateResponseFile(res, req, code, status, body, "text/html");
        body.clear();
        filename.clear();
        return ;
    }else {
        if (res.config.Config["root"].back() != '/')
            res.config.Config["root"] += "/";
        code = 301;
        this->generateResponseMovement(res, req, filename, "Moved Permanently", code);
        filename.clear();
        return ;
    }
}