//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "webserv.hpp"
#include "HttpResponse.hpp"
#include "HttpBase.hpp"

Response::status_map Response::statuslist = Response::init_status_map();
Response::status_map Response::init_status_map(void) {
  status_map _map;

  _map[200] = "OK\n";
  _map[201] = "CREATED\n";
  _map[202] = "ACCEPTED\n";
  _map[300] = "MULTIPLE CHOICE\n";
  _map[400] = "BAD REQUEST\n";
  _map[401] = "UNAUTHORIZED\n";
  _map[403] = "FORBIDDEN\n";
  _map[404] = "NOT FOUND\n";
  _map[405] = "METHOD NOT ALLOWED\n";
  _map[500] = "INTERNAL SERVER ERROR\n";
  _map[502] = "BAD GATEWAY\n";
  _map[504] = "GATEWAY TIMEOUT\n";
  _map[506] = "HTTP VERSION UNSUPPORTED\n";
  return _map;
}

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
  vec.push_back(&Response::validate_http_version);
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

int Response::validate_http_version(void) {
  if (req->http_version != "HTTP/1.1")
    return HTTP_VERSION_UNSUPPORTED;
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
    extension = "text";
  else
    extension = body_path.substr(body_path.find_last_of('.'));
  if (extension == "text") {
    contenttype = "Content-Type: text/plain\n";
    assemble(body_path);
  }
  else if (extension == ".html") {
    contenttype = "Content-Type: text/html; charset=utf-8\n";
    assemble(body_path);
  }
  else if (extension == ".css") {
    contenttype = "Content-Type: text/css; charset=utf-8\n";
    assemble(body_path);
  }
  else if (extension == ".php") {
    contenttype = "Content-Type: text/html; charset=utf-8\n";
    php_cgi(body_path);
  }
  else // TODO(VLN37) return 500 default page
    WebServ::log.warning() << extension << " support not yet implemented\n";
}

void Response::assemble(std::string const& body_path) {
  std::ifstream in;
  std::string str = httpversion +
                    statuscode +
                    statusmsg +
                    contenttype +
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
  std::string   file;
  std::ofstream ofile;
  int           code;
  size_t        index;
  size_t        end;

  index = req_body.find('=');
  end = req_body.find('&');
  file = req_body.substr(index + 1, end - index - 1);
  file = location->upload_store + "/" + file;
  code = access(file.c_str(), W_OK);
  if (code == EACCES)
    return FORBIDDEN;
  ofile.open(file.c_str(), ofile.out | ofile.trunc);
  ofile << req_body;
  return CREATED;
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

  if (response_code >= BAD_REQUEST) {
    if (server->error_page.count(response_code))
      response_path = root + server->error_page[response_code];
    else // TODO(welton) default error pages
      response_path = root + server->error_page[NOT_FOUND];
  }
  statuscode.clear();
  ss << code;
  ss >> statuscode;
  statuscode.push_back(' ');
  if (statuslist.count(response_code))
    statusmsg = statuslist[response_code];
  else
    statusmsg = statuslist[(response_code / 100) * 100];
}

void Response::process(void) {
  for (int i = 0; i < pre_method.size() && response_code == 0; i++)
  response_code = (this->*pre_method[i])();
  if (response_code == 0)
    response_code = (this->*methodptr[method])();
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

Response::Response(void): req(NULL) { }
Response::Response(Request const& _req, Server *_server)
: httpversion("HTTP/1.1 "), statuscode("200 "), statusmsg("OK\n"), req(&_req)
{
  if (_req.body.size()) {
    WebServ::log.debug() << "Request body:\n" << _req.body << "\n";
    req_body = req->body;
  }
  find_location(_req.path, _server);
  server = _server;
  path = "./" + location->root + _req.path;
  root = "./" + location->root + "/";
  method = req->method;
  response_code = 0;
}
