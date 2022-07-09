#include "webserv.h"
#include "HttpHandler.hpp"
#include "HttpBase.hpp"

void Response::_send(int fd) {
  send(fd, HttpBase::buffer_resp, HttpBase::size, 0);
}

RequestHandler::RequestHandler(void) { }
RequestHandler::RequestHandler(Request const& req) {
  std::ifstream in;
  std::string str = "HTTP/1.1 200 OK\n"
                    "Content-Type: text/html\n"
                    "Content-Length: LENGTH\n\n";
  std::string body;
  std::string size;
  std::stringstream ss;

  (void)req;
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
