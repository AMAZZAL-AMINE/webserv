#include "Response.hpp"

void Response::uploadFail(t_response & __unused res, HttpRequest & __unused request, size_t i, int r)
{
    std::string upload_dir;
    std::cout << "file name: " << request.file_name[r] << std::endl;

    if (res.config.Config["root"].back() != '/')
        res.config.Config["root"] += "/";
    if (res.config.Config["upload_dir"].back() != '/')
        res.config.Config["upload_dir"] += "/";
    upload_dir = res.config.Config["root"] + res.config.Config["upload_dir"];
    std::cout << "Upload dir: " << upload_dir << std::endl;
    std::ofstream outfile(upload_dir + request.file_name[r], std::ios::binary);
    outfile.write(request.form_data[i].data(), request.form_data[i].size());
    outfile.close();
}

void Response::Post(t_response & __unused res, HttpRequest & __unused request)
{
    int r = 0;
    for(size_t i = 0; i < request.form_data.size(); i++)
    {
        if (request.content_type[i] == "file_upload")
        {
            uploadFail(res, request, i, r);
            r++;
        }
        else
        {
            std::cout << request.content_names[i] << " : " << request.form_data[i] << std::endl; 
        }
    }
}
