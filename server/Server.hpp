/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamazzal <mamazzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 22:47:50 by mamazzal          #+#    #+#             */
/*   Updated: 2024/01/02 18:54:10 by mamazzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "../main.h"

typedef struct HttpRequest {
    std::string method;
    std::string path;
    std::string version;
    std::vector<std::string> headers;
} HttpRequest;


typedef struct t_requ {
    std::string method;
    std::string path;
    std::string http_version;
    std::string host;
    std::string connection;
    std::string user_agent;
    std::string accept;
    std::string Sec_GPC;
    std::string Fetch_Site;
    std::string Fetch_Mode;
    std::string Fetch_Dest;
    std::string Referer;
    std::string Accept_Encoding;
    std::string Accept_Language;
    std::string body;
    int size;
} t_request;

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