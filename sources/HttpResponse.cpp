//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                           João Rodriguez                                   #
//#                            Paulo Sergio                                    #
//#                            Welton Leite                                    #
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

void Response::_send(int fd) {
  send(fd, HttpBase::buffer_resp, HttpBase::size, 0);
  WebServ::log.info() << "Response sent to client " << fd << "\n";
}

void Response::_get(void) {
  int code;
  if (path == root) {
    for (size_t i = 0; i < server->index.size(); i++) {
      std::string indexpath = root + server->index[i];
      if (!access(indexpath.c_str(), R_OK))
        return extension_dispatcher(indexpath);
    }
    code = errno;
  }
  else if (!access(path.c_str(), R_OK)) {
    return extension_dispatcher(path);
  }
  code = errno;
  statuscode = "404 ";
  statusmsg = "FAIL\n";
  if (code == ENOENT)
    extension_dispatcher(root + server->error_page[404]);
  else if (code == EACCES) {
    statuscode = "405 ";
    extension_dispatcher(root + server->error_page[405]);
  }
  else
    WebServ::log.error() << "Failed request on Response::_get\n";
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

void Response::_post(void) {
  return;
}

void Response::_delete(void) {
  return;
}

void Response::process(void) {
  (this->*methodptr[method])();
}

std::string Response::find_location(std::string path, Server *server) {
  while (path.size()) {
    if (server->location.count(path))
      return server->location[path].root;
    path = path.erase(path.find_last_of('/'));
  }
  return server->location["/"].root;
}

Response::Response(void) { }
Response::Response(Request const& req, Server *_server)
: httpversion("HTTP/1.1 "), statuscode("200 "), statusmsg("OK\n")
{
  if (req.body.size())
    WebServ::log.debug() << "Request body:\n" << req.body << "\n";
  location = find_location(req.path, _server);
  server = _server;
  path = "./" + location + req.path;
  root = "./" + location + "/";
  method = req.method;
}
