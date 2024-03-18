/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response_error.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamazzal <mamazzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 15:40:44 by mamazzal          #+#    #+#             */
/*   Updated: 2024/03/18 21:21:15 by mamazzal         ###   ########.fr       */
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
    std::string file_html = "";
    switch(code) {
        case 404 :
            htmlData = read_html_file(data.error404, data);
            res_status = "404 Not Found";
            file_html = data.error404;
            break;
        case 500 :
            htmlData = read_html_file(data.error500, data);
            res_status = "500 Internal Server Error";
            file_html = data.error500;
            break;
        case 400 :
            htmlData = read_html_file(data.error400, data);
            res_status = "400 Bad Request";
            file_html = data.error400;
            break;
        case 408 :
            htmlData = read_html_file(data.error408, data);
            res_status = "408 Request Timeout";
            file_html = data.error408;
            break;
        case 413 :
            htmlData = read_html_file(data.error413, data);
            res_status = "413 Payload Too Large";
            file_html = data.error413;
            break;
        case 403 :
            htmlData = read_html_file(data.error403, data);
            res_status = "403 Forbidden";
            file_html = data.error403;
            break;
        case  405 :
            htmlData = read_html_file(data.error405, data);
            res_status = "405 Method Not Allowed";
            file_html = data.error405;
            break;
        case 501 :
            htmlData = read_html_file(data.error501, data);
            res_status = "501 Not Implemented";
            file_html = data.error501;
            break;
        case 409 :
            htmlData = read_html_file(data.error409, data);
            res_status = "409 Conflict";
            file_html = data.error409;
            break;
        default :
            break;
    }
    //todo : if bo html error pages set so the response  status code for example  should be 404, else should be 302 or 301 for redirecting the cline to the html page
    bool  __unused canICallDefault = false;
    if (htmlData.empty()) {
        canICallDefault = true;
        htmlData = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>" + res_status + " </title></head><body style='text-align: center;'><h1> " + res_status + " </h1><hr /><p>webserv</p></body></html>";
        std::string httpResq = "HTTP/1.1 " + res_status + "\nContent-Type: text/html\nContent-Length: " + std::to_string(htmlData.length()) + "\n\n" + htmlData + "\n";
        send(client_fd, httpResq.c_str(), httpResq.length(), 0);
        std::cout << RED << "[RESPONSE - " << current_date() << "] " <<  BG_WHITE << BLACK << res_status << RESET << std::endl;
    }else {
        //move the client
        std::string httpRespons = "HTTP/1.1 301 Moved Permanently\nLocation: " + file_html + "\nContent-Type: text/html\nContent-Length: " + std::to_string(htmlData.length()) + "\n\n" + htmlData + "\n";
        send(client_fd, httpRespons.c_str(), httpRespons.length(), 0);
    }
    htmlData.clear();
    res_status.clear();
    file_html.clear();
}