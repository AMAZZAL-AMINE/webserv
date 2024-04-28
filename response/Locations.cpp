#include "Response.hpp"


std::string Response::grepLocationFromPath(std::string & path) {
    std::string location_name;
    size_t i = 0;
    if (path[0] == '/' && path.size() > 1) {
        location_name += path[0];
        i++;
    }
    while (path[i] != '/' && i < path.size()) {
        location_name += path[i];
        i++;
    }
    if (location_name == "")
        location_name = "/";
    return location_name;
}

int Response::isPathFindInLocation(std::string & location_name, t_config & config) {
    for (size_t i = 0; i < config.location.size(); i++)  {
        if (config.location[i].location_name == location_name)
            return 1;
    }
    if (location_name == "/") {
        for (size_t i = 0; i < config.location.size(); i++)  {
            if (config.location[i].location_name == "/")
                return 1;
        }
    }
    return 0;
}

t_location Response::getLocationConfig(std::string & location_name, t_config & config) {
    size_t i = 0;
    for (; i < config.location.size(); i++)  {
        if (config.location[i].location_name == location_name)
            return config.location[i];
    }
    for (i = 0; i < config.location.size(); i++)  {
        if (config.location[i].location_name == "/")
            break;
    }
    return config.location[i];
}

void Response::popTheLastWordFromPath(std::string & path) {
    size_t i = path.size() - 1;
    while (path[i] != '/' && i > 0) {
        path.pop_back();
        i--;
    }
    if (path.size() > 1)
        path.pop_back();
}


void Response::locationHasAlias(HttpRequest & req, t_response & resp, std::string & location_name) {
    if (!resp.config.Config["alias"].empty()) {
        req.path.erase(0, location_name.size());
       if (resp.config.Config["alias"].back() != '/' && req.path.front() != '/')
            resp.config.Config["alias"] += "/";
        resp.config.Config["root"]  = resp.config.Config["alias"];
    }
}

std::map<std::string, std::string> splitRedirecion(std::string & redirection) {
    std::map<std::string, std::string> redirection_map;
    std::string code;
    std::string url;
    size_t i = 0;
    while (redirection[i] == ' ')
        i++;
    while (redirection[i] != ' ') {
        code += redirection[i];
        i++;
    }
    while (redirection[i] == ' ')
        i++;
    while (i < redirection.size()) {
        url += redirection[i];
        i++;
    }
    redirection_map[code] = url;
    return redirection_map;
}

int Response::hasRedirection(HttpRequest &  __unused req, t_response & resp) {
    if (!resp.config.Config["return"].empty()) {
        std::map<std::string, std::string> redirection = splitRedirecion(resp.config.Config["return"]);
        std::string resi = "HTTP/1.1 " + redirection.begin()->first + " Moved Permanently \r\nLocation: " + redirection.begin()->second + "\r\n\r\n";
        send(resp.client_fd, resi.c_str(), resi.size(), 0);
        return 1;
    }
    return 0;
}

void Response::changeLocation(HttpRequest & req, t_response & resp) {
    std::string location_str = req.path;
    while (true) {
        if (isPathFindInLocation(location_str, resp.config)) {
            if (location_str.back() == '/')
                location_str.pop_back();
            t_location location = getLocationConfig(location_str, resp.config);
            resp.config.Config = location.location;
            break;
        }else if (!isPathFindInLocation(location_str, resp.config))
            this->popTheLastWordFromPath(location_str);
        else if (location_str == "/")
            break;
    }
    if (this->hasRedirection(req, resp))
        return;
    this->locationHasAlias(req, resp, location_str);
}