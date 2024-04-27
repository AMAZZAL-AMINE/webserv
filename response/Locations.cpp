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
    for (size_t i = 0; i < config.location.size(); i++)  {
        if (config.location[i].location_name == "/")
            return 1;
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

void Response::changeLocation(HttpRequest & req, t_response & resp) {
    std::string location_name = grepLocationFromPath(req.path);
    if (isPathFindInLocation(location_name, resp.config)) {
        t_location location = getLocationConfig(location_name, resp.config);
        resp.config.Config = location.location;
    }
}