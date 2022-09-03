//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "Response.hpp"

std::ostream& operator<<(std::ostream& o, Response const& rhs) {
  o << std::setfill(' ') << " [ RESPONSE DUMP ]\n"
    << std::setw(15) << std::left << "method" << " : "
    << rhs.method << "\n"
    << std::setw(15) << std::left << "httpversion" << " : "
    << rhs.httpversion << "\n"
    << std::setw(15) << std::left << "statuscode" << " : "
    << rhs.statuscode << "\n"
    << std::setw(15) << std::left << "statusmsg" << " : "
    << rhs.statusmsg << "\n"
    << std::setw(15) << std::left << "path" << " : "
    << rhs.path << "\n"
    << std::setw(15) << std::left << "root" << " : "
    << rhs.root << "\n"
    << std::setw(15) << std::left << "originalroot" << " : "
    << rhs.originalroot << "\n"
    << std::setw(15) << std::left << "contenttype" << " : "
    << rhs.contenttype << "\n"
    << std::setw(15) << std::left << "filetype" << " : "
    << rhs.filetype << "\n"
    << std::setw(15) << std::left << "response_path" << " : "
    << rhs.response_path << "\n";
  (void)o;
  return o;
}

int Response::_put(void) {
  return METHOD_NOT_ALLOWED;
}

int Response::_head(void) {
  return METHOD_NOT_ALLOWED;
}

int Response::_connect(void) {
  return METHOD_NOT_ALLOWED;
}

int Response::_options(void) {
  return METHOD_NOT_ALLOWED;
}

int Response::_patch(void) {
  return METHOD_NOT_ALLOWED;
}

int Response::_trace(void) {
  return METHOD_NOT_ALLOWED;
}

void Response::_send(int fd) {
  ssize_t bytes;
  // WebServ::log.error() << ResponseBase::buffer_resp << "\n";
  bytes = send(fd, ResponseBase::buffer_resp, ResponseBase::size, MSG_NOSIGNAL);
  if (bytes == 0 || bytes == -1) {
    WebServ::log.error() << "unable to send response: "
                         << strerror(errno) << "\n";
    finished = true;
  }
  WebServ::log.info() << "Response sent to client " << fd << "\n";
}

int Response::validate_limit_except(void) {
  if (location->limit_except.size()) {
    if (location->limit_except[0] == "ALL")
      return CONTINUE;
    std::vector<std::string>::iterator it = location->limit_except.begin();
    std::vector<std::string>::iterator ite = location->limit_except.end();
    if (std::find(it, ite, method) != location->limit_except.end())
       return CONTINUE;
    return METHOD_NOT_ALLOWED;
  }
  return CONTINUE;
}

int Response::validate_http_version(void) {
  if (req->http_version != "HTTP/1.1")
    return HTTP_VERSION_UNSUPPORTED;
  return CONTINUE;
}

static std::string fetch_path2(std::string const& bin) {
  int pid;
  int io[2];
  char buf[1024];

  pipe(io);
  pid = fork();
  if (pid == 0) {
    dup2(io[1], STDOUT_FILENO);
    execlp("which", "which", bin.c_str(), NULL);
  }
  close(io[1]);
  pid = read(io[0], buf, 1024);
  close(io[0]);
  buf[pid] = '\0';
  return std::string(buf);
}

void Response::cgi(std::string const &body_path, std::string const &bin) {
  int fd = open(DFL_TMPFILE, O_CREAT | O_RDWR | O_TRUNC, 0644);
  int piper[2];
  pipe(piper);
  if (fd == -1)
    throw(std::exception());
  int status;
  // WebServ::log.error() << body_path.c_str() << "\n";
  int pid = fork();
  if (pid == 0) {
    dup2(piper[0], STDIN_FILENO);
    close(piper[1]);
    setenv("SERVER_PORT", _itoa(server->port).c_str(), 1);
    setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
    if (req->headers.count("Cookie"))
      setenv("HTTP_COOKIE", req->headers.at("Cookie").c_str(), 1);
    setenv("REDIRECT_STATUS", "200", 1);
    setenv("HTTP_HOST", req->headers.at("Host").c_str(), 1);
    setenv("REQUEST_METHOD", "GET", 1);
    setenv("PATH_INFO", req->path.c_str(), 1);
    setenv("SCRIPT_NAME", fetch_path2(bin).c_str(), 1);
    setenv("SCRIPT_FILENAME", body_path.substr(2).c_str(), 1);
    setenv("REQUEST_URI", req->path.c_str(), 1);
    setenv("REDIRECT_STATUS", "true", 1);
    if (!url_parameters.empty())
      setenv("QUERY_STRING", url_parameters.substr(1).c_str(), 1);
    if (dup2(fd, STDOUT_FILENO) == -1) {
      perror("dup2");
      exit(1);
    }
    // WebServ::log.error() << body_path << "\n";
    execlp(bin.c_str(), bin.c_str(), body_path.substr(2).c_str(), NULL);
  }
  close(piper[0]);
  close(piper[1]);
  waitpid(pid, &status, 0);
  close(fd);
  assemble_cgi(DFL_TMPFILE);
}

void Response::dispatch(std::string const& body_path) {
  std::string extension;

  // std::cout << "path: " << body_path << "\n";
  if (body_path.empty()) {
    assemble();
    return;
  }
  std::string tmp(body_path.substr(1));

  if (tmp.find_last_of('.') == std::string::npos)
    extension = "text";
  else
    extension = tmp.substr(tmp.find_last_of('.'));
  if (location->cgi.count(extension)) {
    // WebServ::log.error() << "here\n";
    contenttype = "Content-Type: text/html; charset=utf-8\n";
    cgi(body_path, location->cgi[extension]);
  }
  else if (mimetypes.count(extension)) {
    if (response_path == DFL_TMPFILE)
      assemble_cgi(body_path);
    else {
      contenttype = mimetypes[extension];
      assemble(body_path);
    }
  }
  else {
    WebServ::log.warning() << extension << " support not yet implemented\n";
    contenttype = "Content-Type: application/octet-stream\n";
    assemble(body_path);
  }
}

std::string Response::_itoa(size_t nbr) {
  std::stringstream ss;
  std::string       ret;

  ss << nbr;
  ss >> ret;
  return ret;
}


void Response::set_statuscode(int code) {
  std::stringstream ss;

  statuscode.clear();
  ss << code;
  ss >> statuscode;
  statuscode.push_back(' ');
  if (statuslist.count(response_code))
    statusmsg = statuslist[response_code];
  else
    statusmsg = statuslist[(response_code / 100) * 100];

  if (folder_request) {
    create_directory_listing();
    response_path = DFL_DYNFILE;
  }
  else if (response_code >= BAD_REQUEST) {
    if (server->error_page.count(response_code)) {
      response_path = server->root + "/" + server->error_page[response_code];
    }
    else
      create_error_page();
  }
  else if (response_code >= MOVED_PERMANENTLY) {
    if (server->error_page.count(response_code))
      response_path = server->root + "/" + server->error_page[response_code];
    else
      create_redir_page();
  }
}

void Response::assemble_followup(void) {
  char buf[BUFFER_SIZE];
  size_t body_size;
  std::string str;

  file.read(buf, BUFFER_SIZE);
  body_size = file.gcount();
  if (body_size < BUFFER_SIZE || file.eof())
    finished = true;

  std::memmove(&ResponseBase::buffer_resp[str.size()], buf, body_size);
  ResponseBase::size = body_size;
  ResponseBase::buffer_resp[ResponseBase::size] = '\0';
}

void Response::assemble(void) {
  std::string str(httpversion + statuscode + statusmsg + contenttype);
  str.append(DFL_CONTENTLEN);
  str.replace(str.find("LENGTH"), 6, _itoa(0));
  std::memmove(ResponseBase::buffer_resp, str.c_str(), str.size());
  ResponseBase::size = str.size();
  ResponseBase::buffer_resp[ResponseBase::size] = '\0';
  // WebServ::log.debug() << ResponseBase::buffer_resp;
  WebServ::log.debug() << *this;
}

void Response::assemble_cgi(std::string const& body_path) {
  std::string       body;
  size_t            body_size = 0;

  // WebServ::log.debug() << "File requested: " << path << "\n";
  // WebServ::log.debug() << "Body path: " << body_path << "\n";
  file.close();
  file.open(body_path.c_str(), std::ios::binary);
  if (file.bad() || file.fail())
    WebServ::log.error() << "file opening in Response::assemble\n";

  std::string str(httpversion + statuscode + statusmsg);
  if (incorrect_path) {
    // req->path[req->path.size() - 1] != '/';
    str.append("Location: " + req->path + "/\n");
  }
  std::string header;
  std::getline(file, header);
  // WebServ::log.warning() << "Header: " << header << "\n";
  while (header.size() && header[0] != '\r' && header[1] != '\n') {
    str.append(header);
    if (header.find("Status") != std::string::npos) {
      str.replace(str.find("200 "), 4, header.substr(8, 4));

    }
    str.push_back('\n');
    std::getline(file, header);
    // WebServ::log.warning() << "Header: " << header << "\n";
  }

  std::streampos current = file.tellg();
  file.seekg(0, std::ios::end);
  body_max_size = file.tellg() - current;
  // WebServ::log.warning() << body_max_size << "\n";
  file.seekg(current);

  char buf[BUFFER_SIZE];
  file.read(buf, BUFFER_SIZE);
  body_size = file.gcount();
  if (body_max_size < BUFFER_SIZE)
    finished = true;
  else
    inprogress = true;
  str.append(DFL_CONTENTLEN);
  str.replace(str.find("LENGTH"), 6, _itoa(body_max_size));
  std::memmove(ResponseBase::buffer_resp, str.c_str(), str.size());
  std::memmove(&ResponseBase::buffer_resp[str.size()], buf, body_size);
  ResponseBase::size = str.size() + body_size;
  ResponseBase::buffer_resp[ResponseBase::size] = '\0';
  // WebServ::log.error() << ResponseBase::buffer_resp;
  remove_tmp = true;
  WebServ::log.debug() << *this;
}

void Response::assemble(std::string const& body_path) {
  std::string       body;
  size_t            body_size = 0;

  // WebServ::log.debug() << "File requested: " << path << "\n";
  // WebServ::log.debug() << "Body path: " << body_path << "\n";
  file.close();
  file.open(body_path.c_str(), file.ate);
  body_max_size = file.tellg();
  if (file.bad() || file.fail())
    WebServ::log.error() << "file opening in Response::assemble\n";
  file.seekg(std::ios::beg);
  char buf[BUFFER_SIZE];
  file.read(buf, BUFFER_SIZE);
  body_size = file.gcount();
  if (body_max_size < BUFFER_SIZE)
    finished = true;
  else
    inprogress = true;
  std::string str(httpversion + statuscode + statusmsg + contenttype);
  if (incorrect_path) {
    // req->path[req->path.size() - 1] != '/';
    str.append("Location: " + req->path + "/\n");
  }
  str.append(DFL_CONTENTLEN);
  str.replace(str.find("LENGTH"), 6, _itoa(body_max_size));
  std::memmove(ResponseBase::buffer_resp, str.c_str(), str.size());
  std::memmove(&ResponseBase::buffer_resp[str.size()], buf, body_size);
  ResponseBase::size = str.size() + body_size;
  ResponseBase::buffer_resp[ResponseBase::size] = '\0';
  WebServ::log.debug() << *this;
  // WebServ::log.debug() << ResponseBase::buffer_resp;
}

void Response::process(void) {
  if (response_ready) {
    set_statuscode(response_code);
    dispatch(response_path);
    return;
  }
  for (size_t i = 0; i < validation_functions.size() && response_code == 0; i++)
    response_code = (this->*validation_functions[i])();
  if (response_code == 0) {
    response_code = (this->*method_map[method])();
  }
  if (response_code != 0) {
    set_statuscode(response_code);
    dispatch(response_path);
  }
}

#include "Response_static.tpp"
#include "Response_constructors.tpp"
#include "Response_delete.tpp"
#include "Response_get.tpp"
#include "Response_post.tpp"
#include "Response_dynamichtml.tpp"
