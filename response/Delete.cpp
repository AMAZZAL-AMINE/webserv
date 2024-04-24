#include "Response.hpp"
#include <iostream>

bool isDirectory(const std::string& path)
{
    struct stat fileStat; // get the file status
    if (stat(path.c_str(), &fileStat) == 0) // check if the file exists
    {
        return S_ISDIR(fileStat.st_mode); // check if it is a directory
    }
    else
    {
        std::cerr << "Error: " << std::strerror(errno) << std::endl; // error
        return false;
    }
}

void Response::deleteFile(const std::string& filename, HttpRequest & __unused request)
{
    std::string file = filename + request.path; // get the file path
    if (std::remove(filename.c_str()) != 0) // remove the file
    {
        std::cerr << "Error 505 Internal Server Error"<< std::endl; // internal server error
    }
    else
        std::cout << "Success 204 No Content" << std::endl; // success
}

void Response::Delete(t_response & __unused res, HttpRequest & __unused request)
{
    std::string rootPath = res.config.Config["root"] + request.path; // get the root path
    if (access(rootPath.c_str(), F_OK) == -1) // check if file exists
    {
        std::cout << "Error 404 Not Found" << std::endl; // file not found
        return;
    }
    if (isDirectory(rootPath)) // check if it is a directory
    {
        if (!request.path.empty() && request.path.back() == '/') // check if the path ends with '/'
        {
            if (access(rootPath.c_str(), W_OK) == 0) // check permission
            {
                deleteFile(rootPath, request); // delete the file
            }
            else
                std::cout << "Error 403 Forbiden" << std::endl; // permission denied
        }
        else
            std::cout << "Error 409 Conflict" << std::endl; // conflict
    }
    else
    {
        deleteFile(rootPath, request); // delete the file
        std::cout << rootPath << " File Deleted" << std::endl;  // success
    }
}
