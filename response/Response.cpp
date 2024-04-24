#include "Response.hpp"

Response::Response() {}

void Response::response(t_response & __unused res)
{
    HttpRequest request = parseHttpRequest(res.request, res.config);
    if (request.method == POST)
        Post(res, request);
    else if (request.method == DELETE)
        Delete(res, request);
}

Response::~Response() {}