#ifndef SERVER_HPP
#define SERVER_HPP

#include "../parsing/config/Config.hpp"
#include "../response/Response.hpp"

class Server : public Response {
    private:
        std::vector<int> server_fds;
        std::map<int,t_config> servers;
    public:
        Server();
        int createServerFd(t_config& conf, struct sockaddr_in & address);
        void runServer();
        void get_max_fd(int  * client_fds, int & max, fd_set & readFd) ;
        ~Server();
        void    fd_set_rest(fd_set &read, fd_set& write);
        void    setup_clients(int * client_fds, fd_set& readFd, int& max, std::map<int, int> & clients_map);
        void    receve_request(int *client_fds,std::map<int, int> clients_map, fd_set &readFd, fd_set &writeFd);
};

#endif