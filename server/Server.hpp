/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamazzal <mamazzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 22:47:50 by mamazzal          #+#    #+#             */
/*   Updated: 2024/01/03 16:58:52 by mamazzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "../main.h"

typedef struct HttpRequest {
    std::string method;
    std::string path;
    std::string version;
    bool is_valid;
    bool ifnotvalid_code_status;
    bool is_chunked;
    int content_length;
    int has_body;
    std::vector<std::string> headers;
    std::vector<std::string> body;
} HttpRequest;

class Server {
  private:
    std::string httpRes;
    std::string badRequest;
    std::string timeout;
  public : 
    Server();
    void serve(const t_config & data);
    ~Server();
};


#endif