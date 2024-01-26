/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamazzal <mamazzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 10:53:39 by mamazzal          #+#    #+#             */
/*   Updated: 2024/01/26 17:12:13 by mamazzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
#define MAIN_H
//color
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define RESET "\033[0m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define CYAN "\033[0;36m"
#define MAGENTA "\033[0;35m" 
#define BOLD "\033[1m"
#define UNDERLINE "\033[4m"
#define BLINK "\033[5m"
#define REVERSE "\033[7m"
#define HIDDEN "\033[8m"
#define BLACK "\033[30m"
#define WHITE "\033[37m"
//gradient color
#define GRADIENT "\033[38;5;"
#define GRADIENT_END "m"
//background color
#define BG_BLACK "\033[40m"
#define BG_WHITE "\033[47m"

#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <arpa/inet.h>
#include "parsing/Config.hpp"
#include "exceptionErrors.hpp"
#include "server/Server.hpp"
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>
#include <sstream>
#include <sys/select.h>
#include "./cgi/cgi.hpp"
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>


#define TIME_OUT 1000


int _atoi_(std::string  str);
void display_config(std::vector<std::vector<t_config> > http_config);
HttpRequest parseHttpRequest(const std::string& request, const t_config & config);
void clear_httprequest(HttpRequest & req);
std::string  run_cgi(HttpRequest & __unused req,const t_config & data , std::string content_type, std::string script_filename);
void handle_delete_request(HttpRequest & __unused req, int & __unused client_fd, const t_config  & __unused data);
std::string _itos_(int n);
void response_errors(int client_fd, int code, const t_config & data);
std::string read_html_file(std::string fhtml, const t_config & data);
bool isDirectory(const char* path);
char *current_date();
#endif