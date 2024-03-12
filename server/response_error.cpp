/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response_error.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamazzal <mamazzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 15:40:44 by mamazzal          #+#    #+#             */
/*   Updated: 2024/03/12 16:28:50 by mamazzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../main.h"

char *current_date() {
    time_t now = time(0);
    char *dt = std::ctime(&now);
    dt[strlen(dt) - 1] = '\0';
    return dt;
}


void response_errors(int client_fd, int code, const t_config & data) {
    std::string htmlData = "";
    std::string res_status = "";
    std::string html_file_name = "";
    switch(code) {
        case 404 :
            htmlData = read_html_file(data.error404, data);
            res_status = "404 Not Found";
            html_file_name = data.error404;
            break;
        case 500 :
            htmlData = read_html_file(data.error500, data);
            res_status = "500 Internal Server Error";
            html_file_name = data.error500;
            break;
        case 400 :
            htmlData = read_html_file(data.error400, data);
            res_status = "400 Bad Request";
            html_file_name = data.error400;
            break;
        case 408 :
            htmlData = read_html_file(data.error408, data);
            res_status = "408 Request Timeout";
            html_file_name = data.error408;
            break;
        case 413 :
            htmlData = read_html_file(data.error413, data);
            res_status = "413 Payload Too Large";
            html_file_name = data.error413;
            break;
        case 403 :
            htmlData = read_html_file(data.error403, data);
            res_status = "403 Forbidden";
            html_file_name = data.error403;
            break;
        case  405 :
            htmlData = read_html_file(data.error405, data);
            res_status = "405 Method Not Allowed";
            html_file_name = data.error405;
            break;
        case 501 :
            htmlData = read_html_file(data.error501, data);
            res_status = "501 Not Implemented";
            html_file_name = data.error501;
            break;
        case 409 :
            htmlData = read_html_file(data.error409, data);
            res_status = "409 Conflict";
            html_file_name = data.error409;
            break;
        default :
            break;
    }
    //todo : if bo html error pages set so the response  status code for example  should be 404, else should be 302 or 301 for redirecting the cline to the html page
    bool  __unused canICallDefault = false;
    if (htmlData.empty()) {
        canICallDefault = true;
        htmlData = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>" + res_status + " </title></head><body style='text-align: center;'><h1> " + res_status + " </h1><hr /><p>webserv</p></body></html>";
    }
    std::string httpResq = "HTTP/1.1 " + res_status + "\nContent-Type: text/html\nContent-Length: " + std::to_string(htmlData.length()) + "\n\n" + htmlData + "\n";
    send(client_fd, httpResq.c_str(), httpResq.length(), 0);
    std::cout << RED << "[RESPONSE - " << current_date() << "] " <<  BG_WHITE << BLACK << res_status << RESET << std::endl;
}