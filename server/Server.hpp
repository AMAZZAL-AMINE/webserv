/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamazzal <mamazzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 22:47:50 by mamazzal          #+#    #+#             */
/*   Updated: 2024/01/09 16:18:40 by mamazzal         ###   ########.fr       */
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
    int has_query;
    std::vector<std::string> form_data;
    std::vector<std::string> file_name;
    std::vector<std::string> content_type;
    std::string boundary_start;
    std::string boundary_end;
    bool is_ency_upl_file;
    std::string query;
    std::map<std::string, std::string> headers;
    std::string body;
} HttpRequest;

class Server {
  private:
    std::string httpRes;
    std::string badRequest;
    std::string timeout;
  public : 
    Server();
    void serve(const t_config & data);
    void handle_files_upload(HttpRequest &, std::string & dd);
    ~Server();
};


#endif