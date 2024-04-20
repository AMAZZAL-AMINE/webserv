/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rouali <rouali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 14:00:00 by rouali            #+#    #+#             */
/*   Updated: 2024/04/20 15:13:14 by rouali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../main.h"

cgi::cgi(std::string head, std::string body, std::string SCRIPT_NAME, std::string SCRIPT_FILENAME, std::string CONTENT_TYPE, std::string REQUEST_METHOD, std::string CONTENT_LENGTH, std::string QUERY_STRING, std::string SERVER_PROTOCOL, std::string SERVER_SOFTWARE, std::string SERVER_NAME, std::string GATEWAY_INTERFACE, std::string REDIRECT_STATUS)
{
    this->head = head;
    this->body = body;
    this->env["SCRIPT_NAME"] = SCRIPT_NAME;
    this->env["SCRIPT_FILENAME"] = SCRIPT_FILENAME;
    this->env["REQUEST_METHOD"] = REQUEST_METHOD;
    this->env["CONTENT_LENGTH"] = CONTENT_LENGTH;
    this->env["CONTENT_TYPE"] = CONTENT_TYPE;
    this->env["QUERY_STRING"] = QUERY_STRING;
    this->env["SERVER_PROTOCOL"] = SERVER_PROTOCOL;
    this->env["SERVER_SOFTWARE"] = SERVER_SOFTWARE;
    this->env["SERVER_NAME"] = SERVER_NAME;
    this->env["GATEWAY_INTERFACE"] = GATEWAY_INTERFACE;
    this->env["REDIRECT_STATUS"] = REDIRECT_STATUS;

    this->envp = new char *[this->env.size() + 1];
    std::map<std::string, std::string>::iterator it = this->env.begin();
    for (int i = 0; it != this->env.end(); it++)
    {
        std::string entry = it->first + "=" + it->second;
        envp[i] = new char[entry.length() + 1];
        std::strcpy(envp[i], entry.c_str());
        i++;
    }
    envp[this->env.size()] = NULL;
}

cgi::~cgi()
{
    for (int i = 0; envp[i]; i++)
        delete[] envp[i];
    delete[] envp;
}

std::string  cgi::fill_env(std::string SCRIPT_FILENAME, std::string CGI)
{

    int status;
    int fd[2];
    pid_t pid;

    if (pipe(fd) == -1)
    {
        std::cout << "pipe error" << std::endl;
       return "";
    }
    pid = fork();
    if (pid == -1)
    {
        std::cout << "fork error" << std::endl;
        return "";
    }
    if (pid == 0)
    {
        if (this->env["REQUEST_METHOD"] == "POST")
        {
            if (dup2(fd[0], 0) == -1)
            {
                std::cout << "dup2 error" << std::endl;
                exit(1);
            }
        }
        if (dup2(fd[1], 1) == -1)
        {
            std::cout << "dup2 error" << std::endl;
            exit(1);
        }

        close(fd[0]);
        close(fd[1]);

        char **av = new char *[3];
        av[0] = new char[CGI.length() + 1];
        av[1] = new char[std::strlen(SCRIPT_FILENAME.c_str()) + 1];
        av[2] = NULL;
        std::strcpy(av[0], CGI.c_str());
        std::strcpy(av[1], SCRIPT_FILENAME.c_str());
        if (execve(CGI.c_str(), av, envp) == -1)
        {
            //print cmd
            std::cout << "execve error" << std::endl;
            exit(127);
        }
        exit(0);
    }
    else
    {
        if (this->env["REQUEST_METHOD"] == "POST")
        {
            write(fd[1], body.c_str(), body.length());
        }
        waitpid(pid, &status, 0);
        if (WEXITSTATUS(status) != 0)
        {
            std::cout << "CGI ERROR" << std::endl;
            return "";
        }
        close(fd[1]);
        char buffer[1024];
        std::string response;
        while (true)
        {
            int byte = read(fd[0], buffer, 1023);
            if (byte <= 0)
                break;
            if (byte == 1023)
                buffer[byte] = '\0';
            else
                buffer[byte + 1] = '\0';
            if (byte >= 0)
            {
                buffer[byte] = '\0';
                response += buffer;
            }
        }
        return response;
    }
    return "";
}

std::string enum_to_string(E_METHOD method)
{
    if (method == GET)
        return "GET";
    else if (method == POST)
        return "POST";
    else if (method == DELETE)
        return "DELETE";
    return "NULL";
}

std::string  run_cgi(HttpRequest & __unused req,const t_config & data , std::string __unused content_type, std::string script_filename)
{
    std::string head = enum_to_string(req.method) + " " + req.path  + " " + req.version + "\r\n";
    std::string SCRIPT_NAME = script_filename; //"./cgi/cgi.php";
    std::string SCRIPT_FILENAME = script_filename; //"./cgi/cgi.php";
    std::string CONTENT_TYPE = req.headers["Content-Type"];
    std::string REQUEST_METHOD = enum_to_string(req.method);
    std::cout << "full body: " << req.full_body << std::endl;
    std::cout << "full body length = " << req.full_body.size() << std::endl;
    std::string CONTENT_LENGTH = _itos_(req.content_length);
    std::cout << "CONTENT_LENGTH: " << CONTENT_LENGTH << std::endl;
    std::string QUERY_STRING = req.has_query ? req.query : "test=1";
    std::string SERVER_PROTOCOL = req.version;
    std::string SERVER_SOFTWARE = "localhost";
    std::string SERVER_NAME = "localhost";
    std::string GATEWAY_INTERFACE = "CGI/1.1";
    std::string REDIRECT_STATUS = "200";
    cgi my_cgi(head, req.full_body, SCRIPT_NAME, SCRIPT_FILENAME, CONTENT_TYPE,
               REQUEST_METHOD, CONTENT_LENGTH, QUERY_STRING, SERVER_PROTOCOL,
               SERVER_SOFTWARE, SERVER_NAME, GATEWAY_INTERFACE, REDIRECT_STATUS);

    std::string script_excut = my_cgi.fill_env(SCRIPT_FILENAME, data.cgi_path);
    std::cout << "script resutl = " << script_excut << std::endl;
    size_t pos = script_excut.find("\r\n\r\n");
    std::string cgi_headers = script_excut.substr(0, pos);
    script_excut.erase(0, pos + 4);
    std::string html_res = "HTTP/1.1 200 OK\r\n"+cgi_headers +"\r\nContent-Length: " + std::to_string(script_excut.length()) +  "\r\n\r\n" + script_excut;
    std::cout << GREEN << "CGI RESPONSE: " << RESET << std::endl;
    return html_res;
}