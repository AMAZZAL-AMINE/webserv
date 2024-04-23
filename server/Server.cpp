#include "Server.hpp"
#include <algorithm> 
#include <fcntl.h>

Server::Server(){
}

void set_nonblock(int socket) {
    int flags = fcntl(socket, F_GETFL);
    flags |= O_NONBLOCK;
    fcntl(socket, F_SETFL, flags);
}


int Server::createServerFd(t_config& conf, struct sockaddr_in & address){
    int socketFd;
    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    memset(&address, 0, sizeof(address));
    if(socketFd == -1)
        throw std::runtime_error ("Socket Failure !!\n");
    set_nonblock(socketFd);
    setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(_atoi_(conf.Config["port"]));
    if ( bind(socketFd, (struct sockaddr*)&address, sizeof(address))) {
        close(socketFd);
        throw std::runtime_error("bind error\n");
    }
     if (listen(socketFd, BACKLOG) < 0)
        throw std::runtime_error ("listening Failure !!\n");
    this->server_fds.push_back(socketFd);
    std::cout << "SERVER READY ON : http://localhost:" + conf.Config["port"] << std::endl;
    return socketFd;
}


void    Server::fd_set_rest(fd_set &read, fd_set& write){
    FD_ZERO(&read);
    FD_ZERO(&write);
    for(size_t i = 0; i < this->server_fds.size();i++)
        FD_SET(server_fds[i], &read);

}

void Server::get_max_fd(int  * client_fds, int & max, fd_set & __unused readFd) {
    for (int i = 0; i < MAX_CLIENT; i++){
        if(client_fds[i] > 0) {
            // FD_SET(client_fds[i], &readFd);
            if (client_fds[i] > max)
                max = client_fds[i];   
        }
    }
}

void    Server::setup_clients(int * client_fds, fd_set& readFd, int &max, std::map<int, int> & clients_map){

    struct sockaddr_in address;
    socklen_t socklen = sizeof(address);
     for(std::vector<int>::iterator it = this->server_fds.begin(); it != this->server_fds.end(); it++){
        if(FD_ISSET(*it, &readFd)){
            int clientFd = accept(*it, (sockaddr *) &address, &socklen);
            if(clientFd < 0)
            {
                std::cout << "accept error\n";
                continue;
            }
            clients_map[clientFd] = *it;
            for (int i = 0; i < MAX_CLIENT; i++){
                if(client_fds[i] == 0){
                    client_fds[i] = clientFd;   
                    break;
                }
            }
            this->get_max_fd(client_fds, max, readFd);
        }
    }
}

void    Server::receve_request(int *client_fds,std::map<int, int> clients_map, fd_set &readFd, fd_set &writeFd){
    char buffer[BUFFER_SIZE+1];
    t_response response;
    for(int i = 0;i < MAX_CLIENT; i++){
        if(client_fds[i] != clients_map[client_fds[i]]){
            int rec = read(client_fds[i],buffer, BUFFER_SIZE );
            if(rec > 0){
                buffer[BUFFER_SIZE] = '\0';
                FD_SET(client_fds[i], &writeFd);
            }else {
                continue;
            }
        }
        if (FD_ISSET(client_fds[i], &writeFd)) {
            response.client_fd = client_fds[i];
            response.server_fd = clients_map[client_fds[i]];
            response.request = buffer;
            response.config = this->servers[response.server_fd];
            this->requests_map[client_fds[i]] = response;
            this->response(this->requests_map[client_fds[i]]);
            FD_CLR(client_fds[i], &writeFd);
            FD_CLR(client_fds[i], &readFd);
            close(client_fds[i]);
            client_fds[i] = 0;
        }
    }
}

void Server::runServer(){
    std::map<int, int> clients_map;
    struct sockaddr_in  address;
    for(size_t i  = 0; i < this->http_config.size(); i++){
        t_config conf = this->http_config[i];
        this->servers[this->createServerFd(conf, address)] = conf;
    }
    int max = *(std::max_element(this->server_fds.begin(),this->server_fds.end()));
    fd_set readFd, writeFd;
    int client_fds[MAX_CLIENT];
    for (int i = 0; i < MAX_CLIENT; i++)
        client_fds[i] = 0;
    
    while(1){
        signal(SIGPIPE, SIG_IGN);
        this->fd_set_rest(readFd, writeFd);
        int select_ret = select(max + 1, &readFd, &writeFd, 0 ,0);
        if(select_ret < 0)
            throw std::runtime_error("select eroors\n");
        this->setup_clients(client_fds,readFd,max, clients_map);
        this->receve_request(client_fds,clients_map, readFd, writeFd);
    }
    for (size_t i = 0; i < this->server_fds.size(); i++) {
        close(this->server_fds[i]);
    }
}

Server::~Server() {}