//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "ServerLocation.hpp"

ServerLocation::ServerLocation(void) {
  root = "";
  client_max_body_size = -1;
  autoindex = -1;
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
  }
  return (*this);
}

void ServerLocation::fill(const Server& srv) {
  if (root.empty())
    root = srv.root;
  if (index.size() == 0)
    index = srv.index;
  if (limit_except.size() == 0)
    limit_except.push_back(DFL_LIM_EXCEPT);
  if (client_max_body_size == -1)
    client_max_body_size = srv.client_max_body_size;
  if (cgi.size() == 0)
    cgi = srv.cgi;
  if (redirect.first == 0 && redirect.second == "")
    redirect = srv.redirect;
  if (autoindex == -1)
    autoindex = srv.autoindex;
}
