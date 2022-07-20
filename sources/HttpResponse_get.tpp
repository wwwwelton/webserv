//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "HttpResponse.hpp"

int Response::validate_path(void) {
  if (!access(path.c_str(), R_OK)) {
    response_path = path;
    return OK;
  }
  if (errno == ENOENT)
    return NOT_FOUND;
  else if (errno == EACCES)
    return FORBIDDEN;
  else {
    WebServ::log.warning() << "Unexpected outcome in Response::validate_path\n";
    return NOT_FOUND;
  }
}


int Response::validate_folder(void) {
  struct stat path_stat;
  if (stat(path.c_str(), &path_stat) == -1)
    return NOT_FOUND;
  if (S_ISREG(path_stat.st_mode)) {
    return 0;
  }
  else if (S_ISDIR(path_stat.st_mode)) {
    if (!location->autoindex) {
      WebServ::log.error() << "here\n";
      if (path == root)
        return FORBIDDEN;
      else
        return NOT_FOUND;
    }
    folder_request = true;
    return OK;
  }
  WebServ::log.warning() << "Unexpected outcome in Response::validate_folder\n";
  return 0;
}


int Response::validate_index(void) {
  if (path == root) {
    for (size_t i = 0; i < server->index.size(); i++) {
      std::string indexpath = root + server->index[i];
      if (!access(indexpath.c_str(), R_OK)) {
        response_path = indexpath;
        return OK;
      }
    }
    if (location->autoindex)
      return 0;
    if (errno == ENOENT)
      return NOT_FOUND;
    if (errno == EACCES)
      return FORBIDDEN;
  }
  return 0;
}


int Response::_get(void) {
  int code = 0;

  for (size_t i = 0; i < get_functions.size() && code == 0; i++)
    code = (this->*get_functions[i])();
  if (code)
    return code;
  WebServ::log.warning() << "Unexpected outcome in Response::_get\n";
  return NOT_FOUND;
}
