//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "HttpResponse.hpp"

void Response::_send(int fd) {
  send(fd, HttpBase::buffer_resp, HttpBase::size, 0);
  WebServ::log.info() << "Response sent to client " << fd << "\n";
}

int Response::validate_limit_except(void) {
  if (location->limit_except.size()) {
    if (location->limit_except[0] == "ALL")
      return 0;
    std::vector<std::string>::iterator it = location->limit_except.begin();
    std::vector<std::string>::iterator ite = location->limit_except.end();
    if (std::find(it, ite, method) != location->limit_except.end())
       return 0;
    return METHOD_NOT_ALLOWED;
  }
  return 0;
}

int Response::validate_http_version(void) {
  if (req->http_version != "HTTP/1.1")
    return HTTP_VERSION_UNSUPPORTED;
  return 0;
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
  // WebServ::log.debug() << "Response body: " << body << "\n";
  str += body;
  ss << body.size();
  ss >> size;
  str.replace(str.find("LENGTH"), 6, size);
  std::memmove(HttpBase::buffer_resp, str.c_str(), str.size());
  HttpBase::size = str.size();
  // WebServ::log.debug() << str.size() << "\n";
  in.close();
  // WebServ::log.debug() << "Host: " << req->host << "\n";
  WebServ::log.debug() << "File requested: " << path << "\n";
}

void Response::get_directory_listing(void) {
  std::string _template("<a href=\"PATH\">LINK</a>");
  std::stringstream ss;
  std::ifstream infile;
  std::ofstream outfile;
  std::string tmp;

  infile.open("./sources/templates/index.html");
  outfile.open("./tmp.html", outfile.out | outfile.trunc);

  std::getline(infile, tmp);
  while (tmp.find("<h1>") == std::string::npos) {
    outfile << tmp;
    std::getline(infile, tmp);
  }
  tmp.replace(tmp.find("DIRNAME"), 7, path.substr(path.find_last_of('/') + 1));
  outfile << tmp;

  std::getline(infile, tmp);
  while (tmp.find("PATH") == std::string::npos) {
    outfile << tmp;
    std::getline(infile, tmp);
  }
  _template = tmp;

  struct dirent *dir;
  DIR* directory;
  directory = opendir(path.c_str());
  dir = readdir(directory);
  while (dir != NULL) {
    tmp.replace(tmp.find("PATH"), 4, req->path + "/" + dir->d_name);
    tmp.replace(tmp.find("LINK"), 4, dir->d_name);
    outfile << tmp;
    tmp = _template;
    dir = readdir(directory);
  }

  std::getline(infile, tmp);
  while (tmp.size()) {
    tmp.push_back('\n');
    outfile << tmp;
    std::getline(infile, tmp);
  }

  closedir(directory);
  infile.close();
  outfile.close();
}

void Response::set_statuscode(int code) {
  std::stringstream ss;

  if (folder_request) {
    get_directory_listing();
    response_path = "./tmp.html";
  }
  else if (response_code >= BAD_REQUEST) {
    if (server->error_page.count(response_code))
      response_path = root + server->error_page[response_code];
    else
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
  for (size_t i = 0; i < validation_functions.size() && response_code == 0; i++)
  response_code = (this->*validation_functions[i])();
  if (response_code == 0)
    response_code = (this->*method_map[method])();
  set_statuscode(response_code);
  dispatch(response_path);
  if (location->autoindex)
    unlink("tmp.html");
}

#include "HttpResponse_static.tpp"
#include "HttpResponse_constructors.tpp"
#include "HttpResponse_delete.tpp"
#include "HttpResponse_get.tpp"
#include "HttpResponse_post.tpp"
