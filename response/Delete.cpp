#include "Response.hpp"
#include <iostream>
#include <cstdio>
#include <cerrno>

bool isDirectory(const std::string& path)
{
    struct stat fileStat;
    if (stat(path.c_str(), &fileStat) == 0)
    {
        return S_ISDIR(fileStat.st_mode);
    } else
    {
        std::cerr << "Error: " << std::strerror(errno) << std::endl;
        return false;
    }
}

void deleteFile(const std::string& filename, HttpRequest & __unused request)
{
    std::string file = filename + request.path;
    if (std::remove(filename.c_str()) != 0)
    {
        std::cerr << "404 Not Found"<< std::endl;
    }
    else
        std::cout << file << " deleted successfully" << std::endl;
}

void Response::Delete(t_response & __unused res, HttpRequest & __unused request)
{
    if (request.method == DELETE)
    {
        std::string rootPath = res.config.Config["root"] + request.path;
        if (isDirectory(rootPath))
        {
            std::cout << rootPath << " is a directory." << std::endl;
        } else
        {
            deleteFile(rootPath, request);
            std::cout << rootPath << " is a file." << std::endl;
        }
    }
}
