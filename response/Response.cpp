#include "Response.hpp"
#include <sys/stat.h>
#include <dirent.h>


Response::Response() {}
Response::~Response() {}


void Response::response(t_response & __unused res)
{
    HttpRequest request = parseHttpRequest(res.request, res.config);
    // std::cout << "REQUEST PATH  : " << res.config.Config["root"]  + request.path << std::endl;
    this->GET(request,res.client_fd );
}
