//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "HttpResponse.hpp"

void Response::find_location(std::string path, Server *server) {
  while (path.find('/') != std::string::npos) {
    if (server->location.count(path)) {
      location = &server->location[path];
      return;
    }
    path = path.erase(path.find_last_of('/'));
  }
  location = &server->location["/"];
}

Response::Response(void): req(NULL) { }
Response::Response(Request *_req, Server *_server)
: httpversion("HTTP/1.1 "), statuscode("200 "), statusmsg("OK\n"), req(_req)
{
  folder_request = false;
  remove_tmp = false;
  finished = false;
  inprogress = false;
  server = _server;
}

Response::~Response(void) {
  file.close();
}
