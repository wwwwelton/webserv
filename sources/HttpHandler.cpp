#include "webserv.h"
#include "HttpHandler.hpp"
#include "HttpBase.hpp"

RequestHandler::meth_map RequestHandler::methodptr = RequestHandler::init_map();

RequestHandler::meth_map RequestHandler::init_map(void) {
  meth_map _map;
  _map["GET"] = &RequestHandler::_get;
  _map["POST"] = &RequestHandler::_post;
  _map["DELETE"] = &RequestHandler::_delete;
  return _map;
}

void Response::_send(int fd) {
  send(fd, HttpBase::buffer_resp, HttpBase::size, 0);
}

void RequestHandler::_get(void) {
  if (path == root) {
    for (size_t i = 0; i < server->index.size(); i++) {
      std::string indexpath = root + server->index[i];
      if (!access(indexpath.c_str(), R_OK)) {
        _get_body(indexpath);
        return;
      }
    }
    _get_body(root + server->error_page[404]);
  }
  else if (!access(path.c_str(), R_OK)) {
    _get_body(path);
  }
  else {
      statuscode = "404 ";
      statusmsg = "FAIL\n";
      _get_body(root + server->error_page[404]);
  }
}

void RequestHandler::_get_body(std::string body_path) {
  std::ifstream in;
  std::string str = httpversion +
                    statuscode +
                    statusmsg +
                    DFL_CONTENTTYPE +
                    DFL_CONTENTLEN;
  std::string body;
  std::string size;
  std::stringstream ss;

  char buf[BUFFER_SIZE + 1];
  in.open(body_path.c_str());
  while (in.good()) {
    in.get(buf, BUFFER_SIZE, 0);
    body += buf;
  }
  std::cout << body << "\n";
  str += body;
  ss << body.size();
  ss >> size;
  str.replace(str.find("LENGTH"), 6, size);
  std::memmove(HttpBase::buffer_resp, str.c_str(), str.size());
  HttpBase::size = str.size();
  std::cout << str.size() << "\n";
  in.close();
}

void RequestHandler::_post(void) {
  return;
}

void RequestHandler::_delete(void) {
  return;
}

void RequestHandler::process(void) {
  (this->*methodptr[method])();
}

std::string RequestHandler::find_location(std::string path, Server *server) {
  while (path.size()) {
    if (server->location.count(path))
      return server->location[path].root;
    path = path.erase(path.find_last_of('/'));
  }
  return server->location["/"].root;
}

RequestHandler::RequestHandler(void) { }
RequestHandler::RequestHandler(Request const& req, Server *_server)
: httpversion("HTTP/1.1 "), statuscode("200 "), statusmsg("OK\n")
{
  location = find_location(req.path, _server);
  server = _server;
  path = "./" + location + req.path;
  root = "./" + location + "/";
  // std::cout << "reqpath      -> " << req.path << "\n";
  // std::cout << "path         -> " << path << "\n";
  // std::cout << "server root  -> " << server->root << "\n";
  // std::cout << "server       -> " << root << "\n";
  method = req.method;
}
