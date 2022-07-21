//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "ServerLocation.hpp"

ServerLocation::ServerLocation(void) {
  root = DFL_SERVER_ROOT;
  index.push_back(DFL_SERVER_INDEX_PAGE1);
  index.push_back(DFL_SERVER_INDEX_PAGE2);
  limit_except.push_back(DFL_LIM_EXCEPT);
  client_max_body_size = DFL_CLI_MAX_BODY_SIZE;
  cgi = std::map<std::string, std::string>();
  redirect = std::make_pair(0, "");
  autoindex = DFL_AUTO_INDEX;
  filled = false;
}

ServerLocation::ServerLocation(std::string root,
                               std::vector<std::string> index,
                               std::vector<std::string> limit_except,
                               int client_max_body_size,
                               std::map<std::string, std::string> cgi,
                               std::pair<int, std::string> redirect,
                               bool autoindex,
                               bool filled)
    : root(root),
      index(index),
      limit_except(limit_except),
      client_max_body_size(client_max_body_size),
      cgi(cgi),
      redirect(redirect),
      autoindex(autoindex),
      filled(true) {
  return;
}

ServerLocation::ServerLocation(const ServerLocation& src) {
  *this = src;
}

ServerLocation::~ServerLocation(void) {
  return;
}

ServerLocation& ServerLocation::operator=(const ServerLocation& rhs) {
  if (this != &rhs) {
    root = rhs.root;
    index = rhs.index;
    limit_except = rhs.limit_except;
    client_max_body_size = rhs.client_max_body_size;
    cgi = rhs.cgi;
    redirect = rhs.redirect;
    autoindex = rhs.autoindex;
    filled = true;
  }
  return (*this);
}
