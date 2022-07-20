//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "HttpResponse.hpp"

void Response::find_location(std::string path, Server *server) {
  while (path.size()) {
    if (server->location.count(path)) {
      location = &server->location[path];
      return;
    }
    path = path.erase(path.find_last_of('/'));
  }
  location = &server->location["/"];
}

Response::Response(void): req(NULL) { }
Response::Response(Request const& _req, Server *_server)
: httpversion("HTTP/1.1 "), statuscode("200 "), statusmsg("OK\n"), req(&_req)
{
  if (_req.body.size()) {
    WebServ::log.debug() << "Request body:\n" << _req.body << "\n";
    req_body = req->body;
  }
  // std::map<int, std::string>::iterator it = location->redirect.begin();
  // std::map<int, std::string>::iterator ite = location->redirect.end();
  // for(; it != ite; it++) {
  //   std::cout << it->first << " " << it->second << "\n";
  // }
  folder_request = false;
  remove_tmp = false;
  find_location(_req.path, _server);
  server = _server;
  path = "./" + location->root + _req.path;
  root = "./" + location->root + "/";
  method = req->method;
  response_code = location->redirect.first;
}
