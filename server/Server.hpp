/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamazzal <mamazzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 22:47:50 by mamazzal          #+#    #+#             */
/*   Updated: 2024/02/22 16:20:04 by mamazzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "../main.h"

#define DEFAULT_FORM 200
#define FORM_DATA 201
#define TEXT_PLAIN 202

// enum E_FORM
// {
//   TEXT_PLAIN,
//   DEFAULT_FORM,
//   FORM_DATA,
//   NULL_FORM
// };

typedef struct t_response {
  long get_methdo_file_size;
  long readed_bayt_;
  bool is_header_sent;
  bool is_finished_responsed;
  int  fd;
  std::string readed_str;
} httpResponse;

typedef struct HttpRequest {
  E_METHOD method;
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
  httpResponse response;
} HttpRequest;

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