#include "Response.hpp"

std::string Response::generate_response_time() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *gmtime(&now);
    strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", &tstruct);
    return buf;
}


std::string Response::getFileExtension(std::string & path) {
    std::string extension;

    for (size_t i = path.size() - 1; i > 0; i--) {
        if (path[i] == '.')
            break;
        extension += path[i];
    }
    std::reverse(extension.begin(), extension.end());
    return extension;
}

std::string Response::getMimeType(std::string & key) {
    for (std::map<std::string, std::string>::iterator it = this->mime_types.begin(); it != this->mime_types.end(); it++) {
        if (it->first.find(key) != std::string::npos)
            return it->second;
    }
    return "text/plain";
}


void Response::generateResponseFile(t_response & res, HttpRequest & req, int code, std::string  status) {
    std::string response;
    if (res.config.Config["root"].back() != '/')
        res.config.Config["root"] += "/";
    std::string path = res.config.Config["root"] + req.path;
    std::string extension = getFileExtension(path);
    std::string body = readfile_(path);
    std::string type =  getMimeType(extension);
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

std::string Response::readfile_(std::string path) {
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
        body = defaultErrorPage(code, status);
        std::string httpres = "HTTP/1.1 " + _itos_(code) + " " + status + "\r\n";
        httpres += "Date: " + this->generate_response_time() + "\r\n";
        httpres += "Server: " + res.config.Config["server_name"] + "\r\n";
        httpres += "Content-Length: " + _itos_(body.size()) + "\r\n";
        httpres += "Content-Type: text/html\r\n";
        httpres += "Connection : " + res.config.Config["Connection:"] + "\r\n";
        httpres += "\r\n";
        httpres += body;
        send(res.client_fd, httpres.c_str(), httpres.size(), 0);
        body.clear();
        filename.clear();
        httpres.clear();
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