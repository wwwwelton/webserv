//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "webserv.hpp"
#include "HttpResponse.hpp"
#include "HttpBase.hpp"

Response::meth_map Response::methodptr = Response::init_map();
Response::meth_map Response::init_map(void) {
  meth_map _map;

  _map["GET"] = &Response::_get;
  _map["POST"] = &Response::_post;
  _map["DELETE"] = &Response::_delete;
  return _map;
}

Response::function_vector Response::pre_method = Response::init_pre();
Response::function_vector Response::init_pre(void) {
  function_vector vec;

  vec.push_back(&Response::validate_limit_except);
  return vec;
}

Response::function_vector Response::get_method = Response::init_get();
Response::function_vector Response::init_get(void) {
  function_vector vec;

  vec.push_back(&Response::validate_index);
  vec.push_back(&Response::validate_path);
  return vec;
}

void Response::_send(int fd) {
  send(fd, HttpBase::buffer_resp, HttpBase::size, 0);
  WebServ::log.info() << "Response sent to client " << fd << "\n";
}

int Response::validate_index(void) {
  if (path == root) {
    for (size_t i = 0; i < server->index.size(); i++) {
      std::string indexpath = root + server->index[i];
      if (!access(indexpath.c_str(), R_OK)) {
        response_path = indexpath;
        return OK;
      }
    }
    if (errno == ENOENT)
      return NOT_FOUND;
    if (errno == EACCES)
      return FORBIDDEN;
  }
  return 0;
}

int Response::validate_path(void) {
  if (!access(path.c_str(), R_OK)) {
    response_path = path;
    return OK;
  }
  if (errno == ENOENT)
    return NOT_FOUND;
  else if (errno == EACCES)
    return FORBIDDEN;
  else {
    WebServ::log.warning() << "Unexpected outcome in Response::validate_path\n";
    return NOT_FOUND;
  }
}

int Response::_get(void) {
  int code = 0;

  for (size_t i = 0; i < get_method.size() && code == 0; i++)
    code = (this->*get_method[i])();
  if (code)
    return code;
  WebServ::log.warning() << "Unexpected outcome in Response::_get\n";
  return NOT_FOUND;
}

void Response::php_cgi(std::string const& body_path) {
  int fd = open("./tmp", O_CREAT | O_RDWR | O_TRUNC);
  if (fd == -1)
    throw(std::exception());
  int status;
  int pid = fork();
  if (pid == 0) {
    if (dup2(fd, STDOUT_FILENO) == -1) {
      perror("dup2");
      exit(1);
    }
    // std::cout << body_path.c_str();
    execlp("php-cgi", "-f", body_path.substr(2).c_str(), NULL);
  }
  waitpid(pid, &status, 0);
  close(fd);
  assemble("./tmp");
  unlink("./tmp");
}

void Response::dispatch(std::string const& body_path) {
  std::string extension;

  if (body_path.find_last_of('.') == std::string::npos)
    extension = ".html";
  else
    extension = body_path.substr(body_path.find_last_of('.'));
  if (extension == ".html" || extension == ".css")
    assemble(body_path);
  else if (extension == ".php")
    php_cgi(body_path);
  else
    WebServ::log.warning() << extension << " support not yet implemented\n";
}

void Response::assemble(std::string const& body_path) {
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
  // std::cout << body << "\n";
  str += body;
  ss << body.size();
  ss >> size;
  str.replace(str.find("LENGTH"), 6, size);
  std::memmove(HttpBase::buffer_resp, str.c_str(), str.size());
  HttpBase::size = str.size();
  // std::cout << str.size() << "\n";
  in.close();
  WebServ::log.debug() << "File requested: " << path << "\n";
}

int Response::_post(void) {
  return 0;
}

int Response::_delete(void) {
  return 0;
}

int Response::validate_limit_except(void) {
  if (location->limit_except.size()) {
    if (location->limit_except == "ALL")
      return 0;
    if (location->limit_except.find(method) != std::string::npos)
      return 0;
  }
  return METHOD_NOT_ALLOWED;
}

void Response::set_statuscode(int code) {
  std::stringstream ss;

  statuscode.clear();
  ss << code;
  ss >> statuscode;
  statuscode.push_back(' ');
}

void Response::process(void) {
  response_code = validate_limit_except();
  if (response_code == 0)
    response_code = (this->*methodptr[method])();
  if (response_code > 399) {
    if (server->error_page.count(response_code))
      response_path = root + server->error_page[response_code];
    else // TODO(welton) default error pages
      response_path = root + server->error_page[NOT_FOUND];
  }
  set_statuscode(response_code);
  dispatch(response_path);
}

void Response::find_location(std::string path, Server *server) {
  while (path.size()) {
    if (server->location.count(path)) {
      location = &server->location[path];
    }
    path = path.erase(path.find_last_of('/'));
  }
  location = &server->location["/"];
}

Response::Response(void) { }
Response::Response(Request const& req, Server *_server)
: httpversion("HTTP/1.1 "), statuscode("200 "), statusmsg("OK\n")
{
  if (req.body.size())
    WebServ::log.debug() << "Request body:\n" << req.body << "\n";
  find_location(req.path, _server);
  server = _server;
  path = "./" + location->root + req.path;
  root = "./" + location->root + "/";
  method = req.method;
  response_code = 0;
}
