//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "Response.hpp"

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

  if (stat(path.c_str(), &path_stat) == -1) {
    return NOT_FOUND;
  }
  if (S_ISREG(path_stat.st_mode)) {
    return CONTINUE;
  }
  else if (S_ISDIR(path_stat.st_mode)) {
    if (!location->autoindex) {
      // WebServ::log.warning() << "check autoindex config\n";
      if (path == root)
        return NOT_FOUND;
      else
        return FORBIDDEN;
    }
    folder_request = true;
    if (path.at(path.size() - 1) != '/') {
      incorrect_path = true;
      return MOVED_PERMANENTLY;
    }
    return OK;
  }
  WebServ::log.warning() << "Unexpected outcome in Response::validate_folder\n";
  return CONTINUE;
}


int Response::validate_index(void) {
  // server->print();
  if (path != originalroot && path == root && path[path.size() - 1] != '/') {
    incorrect_path = true;
    return MOVED_PERMANENTLY;
  }
  if (path == root && location->index.size()) {
    for (size_t i = 0; i < location->index.size(); i++) {
      std::string indexpath = root + "/" + location->index[i];
      if (!access(indexpath.c_str(), R_OK)) {
        WebServ::log.debug() << "Redirected to: " << indexpath << "\n";
        response_path = indexpath;
        return OK;
      }
    }
    if (location->autoindex == true) {
      folder_request = true;
    }
    else if (errno == ENOENT)
      return NOT_FOUND;
    else if (errno == EACCES)
      return UNAUTHORIZED;
  }
  return CONTINUE;
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
