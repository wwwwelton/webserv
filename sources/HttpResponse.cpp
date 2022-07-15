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

void Response::_send(int fd) {
  send(fd, HttpBase::buffer_resp, HttpBase::size, 0);
  WebServ::log.info() << "Response sent to client " << fd << "\n";
}

int Response::_get(void) {
  int code;
  if (path == root) {
    for (size_t i = 0; i < server->index.size(); i++) {
      std::string indexpath = root + server->index[i];
      if (!access(indexpath.c_str(), R_OK)) {
        response_path = indexpath;
        return 200;
      }
    }
    code = errno;
  }
  else if (!access(path.c_str(), R_OK)) {
    response_path = path;
    return 200;
  }
  code = errno;
  statuscode = "404 ";
  statusmsg = "FAIL\n";
  if (code == ENOENT)
    return 404;
    // extension_dispatcher(root + server->error_page[404]);
  else if (code == EACCES) {
    statuscode = "405 ";
    return 405;
    // extension_dispatcher(root + server->error_page[405]);
  }
  else {
    WebServ::log.error() << "Failed request on Response::_get\n";
    return 404;
  }
}

void Response::_get_php_cgi(std::string const& body_path) {
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
  _get_body("./tmp");
  unlink("./tmp");
}

void Response::extension_dispatcher(std::string const& body_path) {
  std::string extension(body_path.substr(body_path.find_last_of('.')));
  if (extension == ".html" || extension == ".css")
    return _get_body(body_path);
  else if (extension == ".php")
    return _get_php_cgi(body_path);
  else
    WebServ::log.warning() << extension << " support not yet implemented\n";
}

void Response::_get_body(std::string const& body_path) {
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

void Response::process(void) {
  response_code = validate_limit_except();
  if (response_code == 0)
    response_code = (this->*methodptr[method])();
  if (response_code != 0) {
    if (server->error_page.count(response_code))
      _get_body(root + server->error_page[response_code]);
    else
      _get_body(root + server->error_page[404]);
  }
  _get_body(response_path);
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
