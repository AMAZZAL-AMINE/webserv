#ifndef RESPONSE_CPP
#define RESPONSE_CPP


#include "../main.h"

typedef struct t_response
{
    t_config config;
    std::string request;
    std::string type;
    int client_fd;
    int server_fd;
} t_response;

class Response : public Config {
    protected :
        std::map<int, t_response> requests_map;
    public :
        Response();
        void    response(t_response &);
        void    GET(HttpRequest& request, int fd);
        void    isFile(HttpRequest& request, int fd);
        void    isDir(HttpRequest& request, int fd);
        ~Response();
};

#endif