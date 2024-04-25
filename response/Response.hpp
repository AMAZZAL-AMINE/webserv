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
        void    response(t_response &);
        void    Post(t_response &, HttpRequest &);
        void    Delete(t_response &, HttpRequest &);
        int     isRequestFinished(t_response &);
        void    deleteFile(const std::string& filename, HttpRequest & __unused request);
        void    Get(HttpRequest& request, int fd);
        void    isFile(HttpRequest& request, int fd);
        void    isDir(HttpRequest& request, int fd);
        ~Response();
};

#endif 