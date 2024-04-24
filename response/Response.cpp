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
}

Response::~Response() {}