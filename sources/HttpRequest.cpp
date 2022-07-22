//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################


#include "HttpRequest.hpp"

// TODO: remove this
std::ostream& operator<<(std::ostream& out, const Request& request) {
  out
    << "{\n"
    << "  method: \"" << request.method << "\"\n"
    << "  path: \"" << request.path << "\"\n"
    << "  version: \"" << request.http_version << "\"\n"
    << "  headers: {";

  if (request.headers.empty())
    out << " }\n";
  else {
    out << "\n";
    std::map<std::string, std::string>::const_iterator iter = request.headers.begin();
    for (; iter != request.headers.end(); ++iter) {
      out << "    " << iter->first << ": \"" << iter->second << "\"\n";
    }
    out << "  }\n";
  }
  if (request.body.size()) {
    out << "vvv     body    vvv\n\n";
    out << request.body;
    out << "\n\n^^^ end of body ^^^\n";
  } else {
    out << "  body: empty\n";
  }
  out << "}" << std::endl;
  return out;
}

Request::Request() { }

Request::~Request() { }

bool Request::is_valid() const {
  return !this->error;
}
