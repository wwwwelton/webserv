#include "webserv.h"
#include "HttpHandler.hpp"
#include "HttpBase.hpp"

RequestHandler::meth_map RequestHandler::methodptr = RequestHandler::init_map();

std::string RequestHandler::methods[3] = {
  "GET",
  "POST",
  "DELETE",
};

RequestHandler::meth_map RequestHandler::init_map(void) {
  meth_map _map;
  _map["GET"] = &RequestHandler::_get;
  _map["POST"] = &RequestHandler::_post;
  _map["DELETE"] = &RequestHandler::_delete;
  return _map;
}

RequestHandler::funcptr RequestHandler::fptr[3] = {
  &RequestHandler::_get,
  &RequestHandler::_post,
  &RequestHandler::_delete,
};

void Response::_send(int fd) {
  send(fd, HttpBase::buffer_resp, HttpBase::size, 0);
}

void RequestHandler::_get(void) {
  std::cout << "get function\n";
  return;
}

void RequestHandler::_post(void) {
  return;
}

void RequestHandler::_delete(void) {
  return;
}

void RequestHandler::process(void) {
  std::ifstream in;
  std::string str = "HTTP/1.1 200 OK\n"
                    "Content-Type: text/html\n"
                    "Content-Length: LENGTH\n\n";
  std::string body;
  std::string size;
  std::stringstream ss;

  char buf[BUFFER_SIZE + 1];
  in.open("./server_root/wink");
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

RequestHandler::RequestHandler(void) { }
RequestHandler::RequestHandler(Request const& req, Server *_server)
: failresponse("FAIL\n"), okresponse("OK\n"), httpversion("HTTP/1.1 "),
  statuscode("200 ")
{
  std::cout << req;
  (void)req;
  server = _server;
  method = req.method;
  (this->*RequestHandler::methodptr["GET"])();
}
