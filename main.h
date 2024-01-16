/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamazzal <mamazzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 10:53:39 by mamazzal          #+#    #+#             */
/*   Updated: 2024/01/16 13:29:54 by mamazzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
#define MAIN_H


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
#include <dirent.h>


#define TIME_OUT 1000

#define ROOT "html_root"

int _atoi_(std::string  str);
void display_config(std::vector<std::vector<t_config> > http_config);
HttpRequest parseHttpRequest(const std::string& request);
void clear_httprequest(HttpRequest & req);
std::string  run_cgi(HttpRequest & req);
#endif