#include "HttpRequest.hpp"
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>

#include <map>
#include <string>

// t_httpform HttpRequest::handler(int fd, Server *host) {
//   t_httpform form;
//   int rc;

//   (void)host;
//   rc = recv(fd, buffer_req, sizeof(buffer_req), 0);
//   buffer_req[rc] = '\0';
//   form.body = buffer_resp;
//   form.bodysize = 74;
//   return form;
// }

Request::Request(int fd) {
  ssize_t bytes = recv(fd, HttpBase::buffer_req, 512000, MSG_NOSIGNAL);

  if (bytes == -1)
    throw std::exception();
  raw = new char[bytes + 1];
  std::memmove(raw, HttpBase::buffer_req, bytes);
  raw[bytes] = '\0';
  std::cout << "this was the request: [\n" << raw << "\n]" << std::endl;
}

Request::~Request() {
  delete [] raw;
}
