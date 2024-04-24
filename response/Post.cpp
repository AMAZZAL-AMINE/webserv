#include "Response.hpp"

void Response::Post(t_response & __unused res, HttpRequest & __unused request)
{
    std::cout << "Full Budy : " << request.full_body << std::endl;
}