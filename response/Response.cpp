#include "Response.hpp"

Response::Response() {}


int     Response::isRequestFinished(t_response & res) {
    HttpRequest request = parseHttpRequest(res.request, res.config);
    if (request.method == POST) {
        if (static_cast<size_t>(_atoi_(request.headers["Content-Length"])) > request.full_body.length())
            return 0;
    }
    return 1;
}

void Response::response(t_response & __unused res)
{
    HttpRequest request = parseHttpRequest(res.request, res.config);
    std::string re  = "HTTP/1.1 200 OK\r\n Content-Type: text/html\r\n\r\n <h1>HELLO WORLD</h1>";
    send(res.client_fd, re.c_str(), re.length(), 0);
}

Response::~Response() {}