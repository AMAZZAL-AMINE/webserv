/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response_error.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamazzal <mamazzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 15:40:44 by mamazzal          #+#    #+#             */
/*   Updated: 2024/01/24 15:45:48 by mamazzal         ###   ########.fr       */
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
    switch(code) {
        case 404 :
            htmlData = read_html_file(data.error404, data);
            res_status = "404 Not Found";   
            break;
        case 500 :
            htmlData = read_html_file(data.error500, data);
            res_status = "500 Internal Server Error";
            break;
        case 400 :
            htmlData = read_html_file(data.error400, data);
            res_status = "400 Bad Request";
            break;
        case 408 :
            htmlData = read_html_file(data.error408, data);
            res_status = "408 Request Timeout";
            break;
        case 413 :
            htmlData = read_html_file(data.error413, data);
            res_status = "413 Payload Too Large";
            break;
        case 403 :
            htmlData = read_html_file(data.error403, data);
            res_status = "403 Forbidden";
            break;
        case  405 :
            htmlData = read_html_file(data.error405, data);
            res_status = "405 Method Not Allowed";
            break;
        case 501 :
            htmlData = read_html_file(data.error501, data);
            res_status = "501 Not Implemented";
            break;
        case 409 :
            htmlData = read_html_file(data.error409, data);
            res_status = "409 Conflict";
            break;
        default :
            break;
    }
    //todo : if the response is the default status message, so the status code should be 404, else should be 302 or 301
    bool  canICallDefault = false;
    if (htmlData.empty()) {
        canICallDefault = true;
        htmlData = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>" + res_status + " </title></head><body style='text-align: center;'><h1> " + res_status + " </h1><hr /><p>webserv</p></body></html>";
    }
    std::string httpResq = "HTTP/1.1 " + res_status + "\nContent-Type: text/html\nContent-Length: " + std::to_string(htmlData.length()) + "\n\n" + htmlData + "\n";
    send(client_fd, httpResq.c_str(), httpResq.length(), 0);
    std::cout << RED << "[RESPONSE - " << current_date() << "] " <<  BG_WHITE << BLACK << res_status << RESET << std::endl;
}