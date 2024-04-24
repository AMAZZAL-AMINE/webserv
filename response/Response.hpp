#ifndef RESPONSE_CPP
#define RESPONSE_CPP


#include "../main.h"

typedef struct t_response
{
    t_config config;
    std::string request;
    int client_fd;
    int server_fd;
    int isReuqestFinished;
} t_response;

class Response : public Config {
    protected :
        std::map<int, t_response> requests_map;
    public :
        Response();
        int     isRequestFinished(t_response &);
        void    response(t_response &);
        ~Response();
};

#endif 