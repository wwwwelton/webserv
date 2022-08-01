//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "HttpResponse.hpp"

void Response::set_request(Request const*_req) {
  req = _req;
  if (_req->body.size()) {
    WebServ::log.debug() << "Request body:\n" << _req->body << "\n";
    req_body = req->body;
  }

  find_location(_req->path, server);
  originalroot = server->location["/"].root;
  root = "./" + location->root;
  if (req->method == "POST")
    path = "./" + req->headers.at("Referer") + _req->path;
  else
    path = "./" + server->root + _req->path;
  // std::cout << "location: " << location->root << "\n";
  // std::cout << "req path: " << _req->path << "\n";
  // std::cout << "root: " << root << "\n";
  // std::cout << "path: " << path << "\n";
  method = _req->method;
  response_code = location->redirect.first;
  if (_req->error)
    response_code = _req->error;
  WebServ::log.debug() << *this;
  server->print();
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
