#include "HttpRequest.hpp"
#include "Logger.hpp"
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>

#include <map>
#include <string>
#include <sstream>
#include <cstring>

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
  out << "vvv     body    vvv\n\n";
  out << request.body;
  out << "\n\n^^^ end of body ^^^\n";
  out << "}" << std::endl;
  return out;
}

Request::Request(int _fd) {
  this->valid = false;
  fd = _fd;
}

Request* Request::receive(int _fd) {
  ssize_t bytes = recv(_fd, HttpBase::buffer_req, 512000, MSG_NOSIGNAL);

  if (bytes == -1)
    throw std::exception();
  raw = new char[bytes + 1];
  std::memmove(raw, HttpBase::buffer_req, bytes);
  raw[bytes] = '\0';

  this->nbytes = bytes;
  std::vector<std::string>* tokens = tokenize_request(this->raw);
  parse_request(tokens);
  Logger().debug() << "new " << this->method
    << " request for path " << this->path
    << " on host " << this->headers["Host"]
    << std::endl;
  return this;
}

Request::~Request() {
  delete [] raw;
}

bool Request::is_valid() const {
  return this->valid;
}

void remove_eol(std::string& str) {
  for (std::string::iterator iter = str.begin(); iter != str.end(); iter++) {
    if (*iter == '\r' || *iter == '\n') {
      str.erase(iter);
      iter = str.begin();
    }
  }
}

std::vector<std::string>* Request::tokenize_request(char *payload) {
  std::stringstream stream(payload);
  std::string line;
  std::getline(stream, line);

  int method_end = line.find(' ');
  this->method = line.substr(0, method_end);

  int path_end = line.find(' ', method_end + 1);
  this->path = line.substr(method_end + 1, path_end - method_end - 1);

  this->http_version = line.substr(path_end + 1);
  remove_eol(this->http_version);

  std::getline(stream, line);
  while (line[0] != '\r' && line[0] != '\n') {
    std::string header = line.substr(0, line.find(':'));
    std::string value = line.substr(header.size() + 2);
    remove_eol(value);
    this->headers[header] = value;
    std::getline(stream, line);
  }

  while (!stream.eof()) {
    std::getline(stream, line);
    this->body.append(line);
  }
  this->valid = true;
  return 0;
}

void Request::parse_request(std::vector<std::string>* tokens) {
  (void)tokens;
}
