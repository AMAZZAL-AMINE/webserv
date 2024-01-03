/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamazzal <mamazzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 10:53:39 by mamazzal          #+#    #+#             */
/*   Updated: 2024/01/03 12:45:32 by mamazzal         ###   ########.fr       */
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
#include <arpa/inet.h>
#include "parsing/Config.hpp"
#include "exceptionErrors.hpp"
#include "server/Server.hpp"
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>
#include <sstream>



#define TIME_OUT 1000

int _atoi_(std::string  str);
void display_config(std::vector<std::vector<t_config> > http_config);
HttpRequest parseHttpRequest(const std::string& request);
#endif