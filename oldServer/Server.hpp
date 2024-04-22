/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamazzal <mamazzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 22:47:50 by mamazzal          #+#    #+#             */
/*   Updated: 2024/04/22 15:07:38 by mamazzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "../main.h"


class Server {
  private:
    std::string httpRes;
    std::string badRequest;
    std::string timeout;
  public : 
    Server(const t_config & data);
    void serve(std::vector<t_config> http_config);
    void handle_post_requst(HttpRequest &, const t_config &);
    void handle_get_requst(HttpRequest &, int &, const t_config &);
    int setup_server(const t_config & data,struct sockaddr_in & address);
    void handle_request(HttpRequest & req, int & client_fd, const t_config & data);
    void request_(int & client_fd, const t_config & data);
    ~Server();
};


#endif