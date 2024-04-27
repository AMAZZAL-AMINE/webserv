#include "Response.hpp"

Response::Response() {}

std::string Response::read_html_file(std::string fhtml, const t_config & data) {
    fhtml = data.root + "/" + fhtml;
    char resolvedPath[PATH_MAX];
    realpath(fhtml.c_str(), resolvedPath);
    std::fstream file(resolvedPath);
    if (!file.is_open())
        return "";
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return buffer.str();
}

int Response::check_file_exist(std::string path) {
    //check permission
    if (access(path.c_str(), F_OK) == -1)
        return 404;
    else if (access(path.c_str(), R_OK) == -1)
        return 403;
    return 0;
}

std::string Response::getLocationInPath(std::string path) {
    std::string location = "";
    if (path[0] == '/')
        path.erase(0, 1);
    if (path.empty())
        return location = "/";
    for (size_t i = 0; i < path.length(); i++) {
        if (path[i] == '/')
            break;
        location += path[i];
    }
    return location;
}

t_config  Response::exchange_location_to_config(const t_location & location, const t_config & old_data) {
    t_config  location_config;
    location_config.root = location.root;
    location_config.port = old_data.port;
    location_config.location = location.location;
    location_config.autoindex = location.autoindex;
    location_config.cgi_path = location.cgi_path;
    location_config.error404 = location.error404;
    location_config.error500 = location.error500;
    location_config.error408 = location.error408;
    location_config.error400 = location.error400;
    location_config.error413 = location.error413;
    location_config.error403 = location.error403;
    location_config.error405 = location.error405;
    location_config.error501 = location.error501;
    location_config.error409 = location.error409;
    location_config.max_body_size = location.max_body_size;
    location_config.index = location.index.empty() ? old_data.index : location.index;
    location_config.upload_dir = location.upload_dir;
    location_config.host_name = old_data.host_name;
    location_config.rederection = location.rederection;
    location_config.server_name = location.location.empty() ? old_data.server_name : location.location;
    location_config.alias = location.alias;
    return location_config;
}

t_config Response::change_location(HttpRequest & req, const t_config & data) {
    std::string location = this->getLocationInPath(req.path);
    if (location.empty())
        return data;
    if (location != "/")
        location = "/" + location;
    for (size_t i = 0; i < data.locations.size(); i++) {
        if (data.locations[i].location == location) {
            t_config location_config = this->exchange_location_to_config(data.locations[i], data);
            location_config.IsDefault = false;
            return location_config;
        }
    }
    for (size_t i = 0; i < data.locations.size(); i++) {
        if (data.locations[i].location == "/") {
            t_config location_config = this->exchange_location_to_config(data.locations[i], data);
            location_config.IsDefault = false;
            return location_config;
        }
    }
    return data;
}

bool Response::check_redirection(HttpRequest & req, int & client_fd, const t_config & data) {
    if (req.path == data.rederection.old_location) {
        std::string fullLocation = data.host_name + ":" + _itos_(data.port) + data.rederection.new_location_to_redirect;
        std::string httpRespionse = "HTTP/1.1 301 Moved Permanently\nLocation: " + data.rederection.new_location_to_redirect + "\n\n";
        send(client_fd, httpRespionse.c_str(), httpRespionse.length(), 0);
        return true;
    }
    return false;
}

void Response::handle_request(HttpRequest & req, int & client_fd, const t_config & data) {
    t_config location_config = this->change_location(req, data);
    if (!location_config.IsDefault && !location_config.alias.empty()) {
        if (req.path[req.path.length() - 1] != '/') {
            req.old_req = req.path;
            if (req.old_req[0] != '/').
                req.old_req = "/" + req.old_req;
            else if (req.old_req[req.old_req.length() - 1] == '/')
                req.old_req.erase(req.old_req.length() - 1, 1);
        }
        location_config.root.clear();
        location_config.root = location_config.alias;
        req.path = req.path.substr(location_config.location.length());
        if (location_config.alias[location_config.alias.length() - 1] != '/')
            location_config.alias += "/";
        if (req.path[0] != '/')
            req.path.erase(0, 1);0
        if (req.path.empty())
            req.path = "/";
    }
    if (this->check_redirection(req, client_fd, location_config))
        return ;
    if (req.path.find(".php") != SIZE_T_MAX && req.method != DELETE) {
        std::string path = location_config.root + req.path;
        if (this->check_file_exist(path) != 0) {
            response_errors(client_fd, this->check_file_exist(path), location_config);
            return;
        }
        std::string cgi_path = run_cgi(req, location_config,std::string("text/html"), path);
        send(client_fd, cgi_path.c_str(), cgi_path.length(), 0);
        std::cout << GREEN << "[RESPONSE - " << current_date() << "] " << RESET << location_config.host_name << ":" << location_config.port << " "  << RESET << "  " << BG_WHITE << enum_to_string(req.method) << " " << req.path << RESET << std::endl;
        return;
    }
    else if (req.method == POST) {
        this->handle_post_requst(req, location_config);
        std::string html = "<html><head><title>" + req.path + "</title></head><body><h1>Your request has been created</h1></body></html>";
        std::string httpRes = "HTTP/1.1 201 Created\nContent-Type: text/html\nContent-Length: " + std::to_string(html.length()) + "\n\n" + html + "\n";
        send(client_fd, httpRes.c_str(), httpRes.length(), 0);
    } else if (req.method == GET) {
        this->handle_get_requst(req, client_fd, location_config);
        std::cout << GREEN << "[RESPONSE - " << current_date() << "] " << RESET << location_config.host_name << ":" << location_config.port << " "  << RESET << " "  << enum_to_string(req.method) << " " << req.path << RESET << std::endl;   
    }
    else if (req.method == DELETE)
        this->handle_delete_request(req, client_fd, location_config);
    else
        response_errors(client_fd, 405, location_config);
    this->clear_httprequest(req);
}

void Response::handle_delete_request(HttpRequest & __unused req, int & __unused client_fd, const t_config  & __unused data) {
    std::string path = data.root + req.path;
    if (this->check_file_exist(path) != 0) {
        response_errors(client_fd, this->check_file_exist(data.root + req.path), data);
        return;
    }
    if (this->isDirectory(path.c_str())) {
        //check if directory not empty 
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir(path.c_str())) != NULL) {
            while ((ent = readdir(dir)) != NULL) {
                if (std::string(ent->d_name) != "." && std::string(ent->d_name) != "..") {
                    response_errors(client_fd, 409, data);
                    return;
                }
            }
            closedir (dir);
        }
    }
    //check if it has permission to delete
    if (access(path.c_str(), W_OK) == -1) {
        response_errors(client_fd, 403, data);
        return;
    }
    if (std::remove(path.c_str()) != 0) {
        response_errors(client_fd, 500, data);
    }
    else {
        std::string html_data = "<html><head><title>Index of " + req.path + "</title></head><body><h1>File : " + req.path + " deleted</h1><hr><pre>";
        std::string httpRes = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(html_data.length()) + "\n\n" + html_data + "\n";
        send(client_fd, html_data.c_str(), html_data.length(), 0);
    }
}

void Response::save_file(HttpRequest & req, const t_config & __unused data, size_t & index) {
    std::string path = data.root + data.upload_dir +  req.file_name[index];
    std::ofstream ofs;
    ofs.open(path, std::ofstream::out | std::ofstream::trunc);
    if (!ofs)
        throw std::runtime_error("Could not open file for writing");
    ofs << req.form_data[index];
    std::cout << YELLOW << "[FILE SAVED - " << current_date() << "] " << RESET << path << std::endl;
    ofs.close();
}

void Response::handle_post_requst(HttpRequest &  req, const t_config & data) {
    for (size_t i = 0; i < req.form_data.size(); i++) {
       if (req.content_type[i] == "file_upload")
            this->save_file(req, data, i);
        else
            std::cout << req.content_names[i] << " : " << req.form_data[i] << std::endl;
    }
}

bool Response::isDirectory(const char* path) {
    struct stat statResult;
    if (stat(path, &statResult) != 0)
        return false;
    return S_ISDIR(statResult.st_mode);
}

void Response::directory_response(HttpRequest & req, int & client_fd, const t_config & data) {
    if (req.path == "/")
        req.path = "";
    else if (req.path[req.path.length() - 1] == '/')
        req.path.erase(req.path.length() - 1, 1);
    std::string full_path = data.root + "/" + req.path;
    char resolvedPath[PATH_MAX];
    realpath(full_path.c_str(), resolvedPath);
    std::string htmlData = "";
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(resolvedPath)) != NULL) {
        if (!data.alias.empty())
            req.path = data.location + req.path;
        htmlData += "<html><head><title>Index of " + req.path + "</title></head><body><h1>index of : " + req.path + "</h1><hr><pre>";
        while ((ent = readdir(dir)) != NULL) {
            htmlData += "<a href=\"" + req.path + "/" + ent->d_name + "\">" + ent->d_name + "</a><br>";
        }
        htmlData += "</pre>";
        closedir (dir);
    } else {
        response_errors(client_fd, 404, data);
        return;
    }
    std::string httpRes = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(htmlData.length()) + "\n\n" + htmlData + "\n";
    send(client_fd, httpRes.c_str(), httpRes.length(), 0);
}

std::vector<std::string> Response::split_string(std::string s, std::string delimiter) {
    std::vector<std::string> extensions;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        extensions.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    extensions.push_back(s);
    return extensions;
}

std::string Response::get_file_extention(char * resolvedPath, HttpRequest & req) {
    std::string extension = resolvedPath;
    size_t dotPos = extension.find_last_of('.');
    if (dotPos != std::string::npos)
        extension = extension.substr(dotPos + 1);
    std::vector<std::string> content_types = this->split_string(req.headers["Accept"], ",");
    std::string contentType = "";
    for (size_t i = 0; i < content_types.size(); i++) {
        if (content_types[i].find(extension) != std::string::npos) {
            contentType = content_types[i];
            break;
        }
    }
    if (contentType.empty())
        contentType = "text/plain";
    return contentType;
}

void Response::file_response(HttpRequest &req, int &client_fd, const t_config & __unused data, char *resolvedPath) {
    int fd = open(resolvedPath, O_RDONLY);
    if (fd == -1) {
        response_errors(client_fd, 500, data);
        return;
    }
    std::string contentType = this->get_file_extention(resolvedPath, req);
    struct stat stat_buf;
    fstat(fd, &stat_buf);
    std::string httpRes = "HTTP/1.1 200 OK\nContent-Type: " + contentType + "\nContent-Length: " + std::to_string(stat_buf.st_size) + "\n\n";
    send(client_fd, httpRes.c_str(), httpRes.length(), 0);
    char buffer[BUFFER_SIZE_BIG];
    int bytes_read;
    while ((bytes_read = read(fd, buffer, BUFFER_SIZE_BIG)) != 0) {
        ssize_t bytes_sent = send(client_fd, buffer, bytes_read, 0);
        if (bytes_sent == -1)
            break;
        usleep(1500);
    }
    close(fd);
}

int Response::move_directory_with_slash(HttpRequest & req, int & client_fd, const t_config & data) {
    if (req.path[req.path.length() - 1] != '/' && req.path != "/") {
        if (!data.alias.empty())
            req.path = data.location + req.path;
        std::string httpRes = "HTTP/1.1 301 Moved Permanently\nLocation: " + req.path + "/\n\n";
        send(client_fd, httpRes.c_str(), httpRes.length(), 0);
        std::cout << RED << "[RESPONSE - " << current_date() << "] " <<  BG_WHITE << BLACK << "301 Moved Permanently" << RESET << std::endl;
        return 1;
    }else if (!data.alias.empty() && data.location == req.old_req) {
        std::string httpRes = "HTTP/1.1 301 Moved Permanently\nLocation: " + data.location + "/\n\n";
        send(client_fd, httpRes.c_str(), httpRes.length(), 0);
        std::cout << RED << "[RESPONSE - " << current_date() << "] " <<  BG_WHITE << BLACK << "301 Moved Permanently" << RESET << std::endl;
        return 1;
    }
    return 0;
}

void Response::get_response(HttpRequest & req, int & client_fd, const t_config & data) {
    std::string full_path = data.root + "/" + req.path;
    if (full_path[full_path.length() - 1] == '/')
        full_path.erase(full_path.length() - 1, 1);
    char resolvedPath[PATH_MAX];
    realpath(full_path.c_str(), resolvedPath);
    //check if file exist
    if (this->check_file_exist(resolvedPath) != 0) {
        response_errors(client_fd, this->check_file_exist(resolvedPath), data);
        return;
    } else if (this->isDirectory(resolvedPath)) {
        if (this->move_directory_with_slash(req, client_fd, data))
            return;
        if (access((std::string(resolvedPath) + "/" + data.index[0]).c_str(), F_OK) != -1)
            this->file_response(req, client_fd, data, (char *)(std::string(resolvedPath) + "/" + data.index[0]).c_str());
        else {
            std::cout << "autoindex : " << data.autoindex << std::endl;
            if (data.autoindex == "off")
                response_errors(client_fd, 403, data);
            else if (data.autoindex == "on")
                this->directory_response(req, client_fd, data);
        }
    }
    else
        this->file_response(req, client_fd, data, resolvedPath);
}


void Response::handle_get_requst(HttpRequest &  req, int & client_fd, const t_config & data) {
    if (req.path == "/"  && !req.has_query) {
        std::string html;
        if (data.index.empty()) {
            html = this->read_html_file("index.html", data);
        }else 
            html = this->read_html_file(data.index[0], data);
        if (html.empty())
            this->get_response(req, client_fd, data);
        else {
            std::string httpRes = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(html.length()) + "\n\n" + html + "\n";
            send(client_fd, httpRes.c_str(), httpRes.length(), 0);
        }
    }
    else if (!req.has_body)
        this->get_response(req, client_fd, data);
}

void Response::clear_httprequest(HttpRequest & req) {
    req.path.clear();
    req.version.clear();
    req.query.clear();
    req.headers.clear();
    req.full_body.clear();
}

Response::~Response() {}