//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                           João Rodriguez                                   #
//#                            Paulo Sergio                                    #
//#                            Welton Leite                                    #
//##############################################################################

#include "webserv.hpp"
#include "HttpHandler.hpp"
#include "HttpBase.hpp"

RequestHandler::meth_map RequestHandler::methodptr = RequestHandler::init_map();
Logger RequestHandler::logger = RequestHandler::init_logger();

Logger RequestHandler::init_logger(void) {
  Logger logger(LVL_DEBUG);
  return logger;
}

RequestHandler::meth_map RequestHandler::init_map(void) {
  meth_map _map;
  _map["GET"] = &RequestHandler::_get;
  _map["POST"] = &RequestHandler::_post;
  _map["DELETE"] = &RequestHandler::_delete;
  return _map;
}

void Response::_send(int fd) {
  send(fd, HttpBase::buffer_resp, HttpBase::size, 0);
  Logger().info() << "Response sent to client " << fd << "\n";
}

void RequestHandler::_get(void) {
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
    logger.error() << "Failed request on RequestHandler::_get\n";
}

void RequestHandler::_get_php_cgi(std::string const& body_path) {
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

void RequestHandler::extension_dispatcher(std::string const& body_path) {
  std::string extension(body_path.substr(body_path.find_last_of('.')));
  if (extension == ".html" || extension == ".css")
    return _get_body(body_path);
  else if (extension == ".php")
    return _get_php_cgi(body_path);
  else
    logger.warning() << extension << " support not yet implemented\n";
}

void RequestHandler::_get_body(std::string const& body_path) {
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
  logger.info() << "File requested: " << path << "\n";
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
  if (req.body.size())
    logger.debug() << "Request body:\n" << req.body << "\n";
  location = find_location(req.path, _server);
  server = _server;
  path = "./" + location + req.path;
  root = "./" + location + "/";
  method = req.method;
}
