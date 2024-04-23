#include "Response.hpp"

Response::Response() {}

void Response::response(t_response & __unused res)
{
    HttpRequest request = parseHttpRequest(res.request, res.config);
    std::cout << "REQUEST PATH  : " << res.config.Config["root"]  + request.path << std::endl;
}

Response::~Response() {}