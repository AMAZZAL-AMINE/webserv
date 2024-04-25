#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "../main.h"


class Response {
    private:
    public :
        Response();
        static std::string              read_html_file(std::string fhtml, const t_config & data);
        int                             check_file_exist(std::string path);
        std::string                     getLocationInPath(std::string path);
        t_config                        exchange_location_to_config(const t_location & location, const t_config & old_data);
        t_config                        change_location(HttpRequest & req, const t_config & data);
        bool                            check_redirection(HttpRequest & req, int & client_fd, const t_config & data);
        void                            handle_request(HttpRequest & req, int & client_fd, const t_config & data);
        void                            handle_delete_request(HttpRequest & __unused req, int & __unused client_fd, const t_config  & __unused data);
        void                            save_file(HttpRequest & req, const t_config & __unused data, size_t & index);
        void                            handle_post_requst(HttpRequest &  req, const t_config & data);
        bool                            isDirectory(const char* path);
        void                            directory_response(HttpRequest & req, int & client_fd, const t_config & data);
        static std::vector<std::string> split_string(std::string s, std::string delimiter);
        std::string                     get_file_extention(char * resolvedPath, HttpRequest & req);
        void                            file_response(HttpRequest &req, int &client_fd, const t_config & __unused data, char *resolvedPath);
        int                             move_directory_with_slash(HttpRequest & req, int & client_fd, const t_config & data);
        void                            get_response(HttpRequest & req, int & client_fd, const t_config & data);
        void                            handle_get_requst(HttpRequest &  req, int & client_fd, const t_config & data);
        void                            clear_httprequest(HttpRequest & req);
        ~Response();
};

#endif