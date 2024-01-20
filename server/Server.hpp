/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamazzal <mamazzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 22:47:50 by mamazzal          #+#    #+#             */
/*   Updated: 2024/01/20 12:57:05 by mamazzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "../main.h"

#define DEFAULT_FORM 200
#define FORM_DATA 201
#define TEXT_PLAIN 202

typedef struct HttpRequest {
  std::string method;
  std::string path;
  std::string version;
  bool is_valid;
  int ifnotvalid_code_status;
  bool is_chunked;
  int content_length;
  int has_body;
  int has_query;
  int if_post_form_type;
  std::vector<std::string> form_data;
  std::vector<std::string> file_name;
  std::vector<std::string> content_type;
  std::vector<std::string> content_names;
  std::map<std::string, std::string> query_params;
  std::string boundary_start;
  std::string boundary_end;
  bool is_ency_upl_file;
  std::map<std::string, std::string> headers;
  std::string query;
  std::string full_body;
} HttpRequest;

class Server {
  private:
    std::string httpRes;
    std::string badRequest;
    std::string timeout;
  public : 
    Server(const t_config & data);
    void serve(const t_config & data);
    void handle_post_requst(HttpRequest &, const t_config &);
    void handle_get_requst(HttpRequest &, int &, const t_config &);
    void handle_request(HttpRequest & req, int & client_fd, const t_config & data);
    
    ~Server();
};


#endif