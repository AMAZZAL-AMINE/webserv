#include "Response.hpp"

#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <dirent.h>


void    Response::isDir(HttpRequest& request, int fd){
    std::string path = requests_map[fd].config.Config["root"]  + request.path;
    const char* directory_path = path.c_str();
    DIR* dir;
    struct dirent* entry;

    dir = opendir(directory_path);
    if (dir == NULL)
        perror("opendir dsoem");

    while ((entry = readdir(dir)) != NULL){
        if (strncmp(entry->d_name, "index.", 6) == 0) {
            Response::isFile(request, fd); //file sent to isFile()!
            closedir(dir);
            return ;
        }
    }
    if(requests_map[fd].config.Config["autoindex"] == "on"){
        std::cout << "autoindex is on  " << directory_path << std::endl;
        //list all directory files 
    }
}

void    Response::isFile(HttpRequest& request,int fd){
    std::string path = requests_map[fd].config.Config["root"]  + request.path;
    if(access(path.c_str(), X_OK) != -1){
        std::string cgi_response = run_cgi(request, this->requests_map[fd].config, "", this->requests_map[fd].config.Config["cgi_path"]);
        std::cout <<"request CGI !!!"<<std::endl; //send to CGI
    }else
        std::cout <<" return the file!! 200 OK" << std::endl; 
    
}


void    Response::Get(HttpRequest& request, int fd)
{
    struct stat stats;
    std::string path = requests_map[fd].config.Config["root"]  + request.path;
    if(stat(path.c_str(),&stats) == 0){
        if(S_ISREG(stats.st_mode)){
           this->isFile(request, fd);
        }else if (S_ISDIR(stats.st_mode)){
            if(request.path.back() == '/'){
                std::cout << "its a directory \n";
                this->isDir(request, fd);
            }
            else {
                std::string httpRes = "HTTP/1.1 301 Moved Permanently\nLocation: " + request.path + "/\n\n";
                std::cout << "its dosent end with it \n"; // redirection 301!!
                send(fd, httpRes.c_str(), httpRes.length(), 0);
            }
        }
    }else{
        std::cout << "path not found \n"; // 404 error NOT FOUND
    }
}


