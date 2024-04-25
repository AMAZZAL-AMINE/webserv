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
        void        response(t_response &);
        void        Post(t_response &, HttpRequest &);
        void        Delete(t_response &, HttpRequest &);
        int         isRequestFinished(t_response &);
        void        deleteFile(const std::string&, HttpRequest &);
        void        Get(HttpRequest&, int);
        void        isFile(HttpRequest&, int);
        void        isDir(HttpRequest&, int);
        void        generateResponseFile(t_response & res, HttpRequest & req, int code, std::string  status);
        void        generateResponseMovement(t_response &,HttpRequest &, std::string &, std::string , int );
        std::string generate_response_time();
        std::string defaultErrorPage(int &, std::string &);
        void        errorResponse(t_response &, HttpRequest &, int, std::string);
        std::string getMimeType(std::string & key);
        std::string getFileExtension(std::string & path);
        std::string readfile_(std::string path);
        ~Response();
};

#endif 