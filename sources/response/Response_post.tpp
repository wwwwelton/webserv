//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "Response.hpp"

// static size_t request_header_end_index(std::vector<char> const& vec) {
//   size_t index = 0;
//   size_t size = vec.size();

//   while (index < size) {
//     if (vec[index] == '\r' && vec[index + 1] == '\n')
//       return index + 2;
//     ++index;
//   }
//   return -1;
// }

static std::string fetch_path(std::string const& bin) {
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

int Response::check_ext(std::string const& extension) {
  if (server->cgi.count(extension))
    return 0;
  return 1;
}

void Response::set_environment(void) {
  in_addr addr;

  // setenv("SERVER_ADDR", "127.0.0.1", 1);
  // setenv("REMOTE_ADDR", "127.0.0.1", 1);
  // setenv("REQUEST_SCHEME", "http", 1);
  // setenv("CONTEXT_PREFIX", "", 1);
  // setenv("SERVER_ADMIN", "", 1);
  // setenv("REDIRECT_URL", "/", 1);
  // setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
  // setenv("QUERY_STRING", "", 1);
  addr.s_addr = server->ip;
  setenv("SERVER_NAME", (server->server_name[0] + " | " + inet_ntoa(addr)).c_str(), 1);
  if (req->headers.count("Host"))
    setenv("HTTP_HOST", req->headers.at("Host").c_str(), 1);
  if (req->headers.count("Referer"))
    setenv("HTTP_REFERER", req->headers.at("Referer").c_str(), 1);
  if (req->headers.count("Accept-Language"))
    setenv("HTTP_ACCEPT_LANGUAGE", req->headers.at("Accept-Language").c_str(), 1);
  if (req->headers.count("Accept-Encoding"))
    setenv("HTTP_ACEPT_ENCODING", req->headers.at("Accept-Encoding").c_str(), 1);
  setenv("SERVER_PORT", _itoa(server->port).c_str(), 1);
  setenv("SERVER_SOFTWARE", "TDD/4.0", 1);
  setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
  setenv("REQUEST_METHOD", "POST", 1);
  if (req->headers.count("Cookie"))
    setenv("HTTP_COOKIE", req->headers.at("Cookie").c_str(), 1);
  setenv("REDIRECT_STATUS", "200", 1);
  // setenv("REQUEST_URI", "/post/upload_handler.php", 1);
  setenv("REQUEST_URI", req->path.c_str(), 1);
  setenv("PATH_INFO", "/", 1);

  // setenv("SCRIPT_NAME", "/usr/bin/php-cgi", 1);
  setenv("SCRIPT_NAME", fetch_path(bin).c_str(), 1);
  setenv("SCRIPT_FILENAME", (server->root + req->path).c_str(), 1);
  // setenv("SCRIPT_FILENAME", "./server_root/sito/upload.php", 1);
  // setenv("CONTENT_LENGTH", _itoa(req->body.size()).c_str(), 1);
  if (req->headers.count("Content-Length"))
    setenv("CONTENT_LENGTH", req->headers.at("Content-Length").c_str(), 1);
  setenv("CONTENT_TYPE", req->headers.at("Content-Type").c_str(), 1);
  setenv("REDIRECT_STATUS", "true", 1);
}

int Response::_post(void) {
  int outfile;
  std::string bin;
  std::string extension;

  WebServ::log.debug() << *this;
  if (pid == 0) {
	// path = "/sito/upload.php/";
    if (path.find('.') == std::string::npos) {
      return INTERNAL_SERVER_ERROR;
    }
    extension = path.substr(path.find_last_of('.'));
    if (extension.at(extension.size() - 1) == '/')
      extension.resize(extension.size() - 1);
    if (check_ext(extension))
      return INTERNAL_SERVER_ERROR;
    bin = server->cgi[extension];
    outfile = open(DFL_TMPFILE, O_CREAT | O_RDWR | O_TRUNC, 0777);
    pipe(io);
    pid = fork();
    if (pid == 0) {
      dup2(io[0], STDIN_FILENO);
      dup2(outfile, STDOUT_FILENO);
      set_environment();
      execlp(bin.c_str(), bin.c_str(), (char *)NULL);
    }
  }
  std::vector<char> res;
  res = parser->get_chunk();
  // size_t index = request_header_end_index(res);
  // if (index == std::string::npos)
    // throw std::exception();
  // write(io[1], &res[index], res.size() - index);
  // header_present = false;
  write(io[1], &res[0], res.size());
  if (!parser->finished)
    return CONTINUE;
  close(io[1]);
  waitpid(pid, NULL, 0);
  close(io[0]);
  close(outfile);
  response_path = DFL_TMPFILE;
  WebServ::log.warning() << "Response finished\n";
  WebServ::log.warning() << response_path << "\n";
  response_ready = true;
  parser->finished = true;
  // remove_tmp = true;
  return OK;
}
