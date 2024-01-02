#include "../../main.h"


std::string get_method(std::string gg) {
  std::string data;
  for (int i = 0; gg[i] != ' '; i++) {
    if (gg[i] == '\0')
      return "error";
    data += gg[i];
  }
  return data;
}

std::string get_path(std::string gg, int start) {
  std::string data;
  for (int i = start; gg[i] != ' '; i++) {
    if (gg[i] == '\0')
      return "error";
    data += gg[i];
  }
  return data;
}

std::string get_http_version(std::string gg, int start) {
  std::string data;
  for (int i = start; gg[i] != '\n'; i++) {
    if (gg[i] == '\0')
      return "error";
    data += gg[i];
  }
  return data;
}

std::string get_host(std::string gg, int start) {
  std::string data;
  int j;
  for (j = start; gg[j] != ' '; j++)
  {
  }
  j++;
  for (int i = j; gg[i] != '\n'; i++) {
    if (gg[i] == '\0')
      return "error";
    data += gg[i];
  }
  return data;
}

std::string get_conection(std::string gg, int start) {
  std::string data;
  int j;
  for (j = start; gg[j] != ' '; j++)
  {
  }
  j++;
  for (int i = j; gg[i] != '\n'; i++) {
    if (gg[i] == '\0')
      return "error";
    data += gg[i];
  }
  return data;
}

// std::string get_user_agent(std::string gg, int start) {
//   std::string data;
//   std::cout << "start : " << start << std::endl;
//   int j;
//   for (j = start; gg[j] != ' '; j++)
//   {
//   }
//   j++;
//   for (int i = start; gg[i] != '\n'; i++) {
//     if (gg[i] == '\0')
//       return "error";
//     data += gg[i];
//   }
//   return data;
// }

// std::string get_accept(std::string gg, int start) {
//   std::string data;
//   int j;
//   for (j = start; gg[j] != ' '; j++)
//   {
//   }
//   j++;
//   for (int i = j; gg[i] != '\n'; i++) {
//     if (gg[i] == '\0')
//       return "error";
//     data += gg[i];
//   }
//   return data;
// }

// std::string get_secGPC(std::string gg, int start) {
//   std::string data;
//   int j;
//   for (j = start; gg[j] != ' '; j++)
//   {
//   }
//   j++;
//   for (int i = j; gg[i] != '\n'; i++) {
//     if (gg[i] == '\0')
//       return "error";
//     data += gg[i];
//   }
//   return data;
// }

// std::string get_Site(std::string gg, int start) {
//   std::string data;
//   int j;
//   for (j = start; gg[j] != ' '; j++)
//   {
//   }
//   j++;
//   for (int i = j; gg[i] != '\n'; i++) {
//     if (gg[i] == '\0')
//       return "error";
//     data += gg[i];
//   }
//   return data;
// }

// std::string get_Mode(std::string gg, int start) {
//   std::string data;
//   int j;
//   for (j = start; gg[j] != ' '; j++)
//   {
//   }
//   j++;
//   for (int i = j; gg[i] != '\n'; i++) {
//     if (gg[i] == '\0')
//       return "error";
//     data += gg[i];
//   }
// 
//   return data;
// }

// std::string get_Dest(std::string gg, int start) {
//   std::string data;
//   int j;
//   for (j = start; gg[j] != '\n'; j++)
//   {
//   }
//   j++;
//   for (int i = j; gg[i] != '\n'; i++) {
//     if (gg[i] == '\0')
//       return "error";
//     data += gg[i];
//   }
//   return data;
// }

// std::string get_Referer(std::string gg, int start) {
//   std::string data;
//   int j;
//   for (j = start; gg[j] != '\n'; j++)
//   {
//   }
//   j++;
//   for (int i = j; gg[i] != '\n'; i++) {
//     if (gg[i] == '\0')
//       return "error";
//     data += gg[i];
//   }
//   return data;
// }

// std::string get_Encoding(std::string gg, int start) {
//   std::string data;
//   int j;
//   for (j = start; gg[j] != '\n'; j++)
//   {
//   }
//   j++;
//   for (int i = j; gg[i] != '\n'; i++) {
//     if (gg[i] == '\0')
//       return "error";
//     data += gg[i];
//   }
//   return data;
// }

// std::string get_Language(std::string gg, int start) {
//   std::string data;
//   int j;
//   for (j = start; gg[j] != '\n'; j++)
//   {
//   }
//   j++;
//   for (int i = j; gg[i] != '\n'; i++) {
//     if (gg[i] == '\0')
//       return "error";
//     data += gg[i];
//   }
//   return data;
// }

std::string get_(std::string gg, int start) 
{
  std::string data;
    int j = 0;
    int pos = start;
    for (j = start; gg[j] != '\n'; j++) {
      pos++;
    }
    j++;
    pos++;
    for (int i = j; gg[i] != ' '; i++)
    {
      pos++;
    }
    j++;
    pos++;
    for (int i = pos; gg[i] != '\n'; i++)
    {
        if (gg[i] == '\0') {
            return "error";
        }
        data += gg[i];
    }
    return data;
}

t_request pars(std::string gg)
{
  t_request data;
  int size = 0;
  data.method = get_method(gg);
  size = data.method.size();
  data.path = get_path(gg, size + 1);
  size += data.path.size();
  data.http_version = get_http_version(gg, size + 1);
  size += data.http_version.size();
  data.host = get_host(gg, size + 1);
  size += data.host.size();
  data.connection = get_conection(gg, size + 1);


  size += data.connection.size();
  data.user_agent = get_(gg, size + 1);
  size += data.user_agent.size();
  data.accept = get_(gg, size + 1);
  size += data.accept.size();
  data.Sec_GPC = get_(gg, size + 1);


  // size += data.Sec_GPC.size();
  // find "Sec-Fetch-Site: " and get the value after it until \n
  
  // data.Fetch_Site = get_(gg, size + 1);
  // size += data.Fetch_Site.size();

  // data.Fetch_Site = get_(gg, size);
  // data.Fetch_Mode = get_Mode(gg, data.method.size() + data.path.size() + data.http_version.size() + data.host.size() + data.connection.size() + data.user_agent.size() + data.accept.size() + data.Sec_GPC.size() + data.Fetch_Site.size() + 24);
  // data.Fetch_Dest = get_Dest(gg, data.method.size() + data.path.size() + data.http_version.size() + data.host.size() + data.connection.size() + data.user_agent.size() + data.accept.size() + data.Sec_GPC.size() + data.Fetch_Site.size() + data.Fetch_Mode.size() + 24);
  // data.Referer = get_Referer(gg, data.method.size() + data.path.size() + data.http_version.size() + data.host.size() + data.connection.size() + data.user_agent.size() + data.accept.size() + data.Sec_GPC.size() + data.Fetch_Site.size() + data.Fetch_Mode.size() + data.Fetch_Dest.size() + 24);
  // data.Accept_Encoding = get_Encoding(gg, data.method.size() + data.path.size() + data.http_version.size() + data.host.size() + data.connection.size() + data.user_agent.size() + data.accept.size() + data.Sec_GPC.size() + data.Fetch_Site.size() + data.Fetch_Mode.size() + data.Fetch_Dest.size() + data.Referer.size() + 24);
  // data.Accept_Language = get_Language(gg, data.method.size() + data.path.size() + data.http_version.size() + data.host.size() + data.connection.size() + data.user_agent.size() + data.accept.size() + data.Sec_GPC.size() + data.Fetch_Site.size() + data.Fetch_Mode.size() + data.Fetch_Dest.size() + data.Referer.size() + data.Accept_Encoding.size() + 24);
  return data;
}