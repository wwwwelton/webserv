//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "Response.hpp"

void Response::find_location(std::string path, Server *server) {
  std::string remainder;
  while (path.find('/') != std::string::npos) {
    if (server->location.count(path)) {
      location = &server->location[path];
      // if (path.at(path.size() - 1) == '/')
      //   path = path.erase(path.find_last_of('/'));
      return;
    }
    remainder = path.substr(path.find_last_of('/'));
    trailing_path = remainder + trailing_path;
    path = path.erase(path.find_last_of('/'));
  }
  location = &server->location["/"];
}

void Response::reset(void) {
  req = NULL;
  finished = false;
  inprogress = false;
  incorrect_path = false;
  folder_request = false;
  remove_tmp = false;
  valid = true;
  path_ends_in_slash = false;
  header_present = true;
  response_ready = false;
  response_code = CONTINUE;
  trailing_path.clear();
  response_path.clear();
  file.close();
  pid = 0;
  statuscode = " 200";
  statusmsg = "OK\n";
}

std::string Response::get_path(std::string req_path) {
  find_location(req_path, server);

  if (req->path[req->path.size() - 1] == '/')
    path_ends_in_slash = true;
  originalroot = server->location["/"].root;
  root = "./" + location->root;
  if (!trailing_path.empty() &&
      trailing_path != "/" &&
      root[root.size() - 1] == '/')
    path = root + trailing_path.substr(1);
  else if (trailing_path != "/")
    path = root + trailing_path;
  else
    path = root;
  if (path.size() > 2 && path[path.size() - 1] == '/' &&
                          path[path.size() - 2] == '/')
    path.resize(path.size() - 1);
  return path;
}

void Response::set_request(Request *_req) {
  req = _req;
  if (_req->body.size()) {
    WebServ::log.debug() << "Request body:\n" << _req->body << "\n";
    req_body = req->body;
  }
  find_location(_req->path, server);
  if (location->root == originalroot)
    if (root.at(root.size() - 1) != '/')
      root.push_back('/');

  if (req->path[req->path.size() - 1] == '/')
    path_ends_in_slash = true;
  originalroot = server->location["/"].root;
  root = "./" + location->root;
  if (req->method == "POST") {
    if (req->headers.count("Origin"))
      path = "./" + req->headers.at("Origin") + _req->path;
  }
  else {
    if (location->root == originalroot)
      if (root.at(root.size() - 1) != '/')
        root.push_back('/');
    if (!trailing_path.empty() && trailing_path != "/" && root[root.size() - 1] == '/')
      path = root + trailing_path.substr(1);
    else if (trailing_path != "/")
      path = root + trailing_path;
    else
      path = root;
    if (path_ends_in_slash)
      if (path[path.size() - 1] != '/')
        path.push_back('/');
    if (path.size() > 2 && path[path.size() - 1] == '/' &&
                           path[path.size() - 2] == '/')
      path.resize(path.size() - 1);
  }
  method = _req->method;
  response_code = location->redirect.first;
  if (_req->error)
    response_code = _req->error;


  std::cout << location->index[0] << "\n";
  std::cout << "location: " << location->root << "\n";
  std::cout << "req path: " << _req->path << "\n";
  std::cout << "root: " << root << "\n";
  std::cout << "path: " << path << "\n";
  std::cout << "trailing path: " << trailing_path << "\n";
}

Response::Response(void): req(NULL) {
  response_ready = false;
  header_present = true;
  finished = false;
  inprogress = false;
  incorrect_path = false;
  folder_request = false;
  remove_tmp = false;
  valid = true;
  path_ends_in_slash = false;
  response_code = CONTINUE;
  pid = 0;
  statuscode = " 200";
  statusmsg = "OK\n";
}
Response::Response(Request *_req, Server *_server)
: httpversion("HTTP/1.1 "), statuscode("200 "), statusmsg("OK\n"), req(_req)
{
  response_ready = false;
  header_present = true;
  folder_request = false;
  remove_tmp = false;
  finished = false;
  inprogress = false;
  incorrect_path = false;
  server = _server;
  pid = 0;
}

Response::~Response(void) {
  file.close();
}
