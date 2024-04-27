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

void Response::changeLocation(HttpRequest & req, t_response & resp) {
    std::string location_str = req.path;
    while (true) {
        if (isPathFindInLocation(location_str, resp.config)) {
            if (location_str.back() == '/')
                location_str.pop_back();
            t_location location = getLocationConfig(location_str, resp.config);
            resp.config.Config = location.location;
            break;
        }else if (!isPathFindInLocation(location_str, resp.config)) {
            this->popTheLastWordFromPath(location_str);
            std::cout  << location_str << std::endl;
        }
        else if (location_str == "/")
            break;
    }
}