#include "../main.h"


int setup_server(const t_config & data,struct sockaddr_in & address) {
    int server_fd;
    int opt = 1;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(data.port);
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 20);
    return server_fd;
}



void    Server::serve(const t_config& data){
    int fd_server;
    struct sockaddr_in address;
        server_fd = setup_server(data, address);

}