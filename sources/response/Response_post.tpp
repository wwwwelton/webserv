//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "Response.hpp"

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

int Response::_post(void) {
  int outfile;
  int pid;
  int io[2];
  std::string bin;
  std::string extension;

  if (path.find('.') == std::string::npos) {
    return INTERNAL_SERVER_ERROR;
  }
  extension = path.substr(path.find_last_of('.'));
  if (extension.at(extension.size() - 1) == '/')
    extension.resize(extension.size() - 1);
  if (check_ext(extension))
    return INTERNAL_SERVER_ERROR;
  bin = server->cgi[extension];
  pipe(io);
  outfile = open(DFL_TMPFILE, O_CREAT | O_RDWR | O_TRUNC, 0777);
  std::cout << *this;
  pid = fork();
  if (pid == 0) {
    dup2(io[0], STDIN_FILENO);
    dup2(outfile, STDOUT_FILENO);

    // setenv("HTTP_ACCEPT", "", 1);
    // setenv("HTTP_SEC_FETCH_USER", "?1", 1); // ?
    // setenv("HTTP_SEC_FETCH_DEST", "", 1);   // ?

    // setenv("REDIRECT_HANDLER", "", 1);
    // setenv("HTTP_HOST", "", 1);
    // setenv("HTTP_CONNECTION", "", 1);
    // setenv("CONTENT_TYPE", "", 1);
    // setenv("HTTP_ACCEPT_LANGUAGE", "", 1);
    // setenv("HTTP_SEC_GPC", "1", 1);
    // setenv("HTTP_SEC_FETCH_MODE", "", 1);
    // setenv("HTTP_REFERER", "", 1);
    // setenv("HTTP_ACEPT_ENCODING", "", 1);

    // setenv("AUTH_TYPE", "", 1);
    // setenv("SERVER_SOFTWARE", "", 1);
    setenv("SERVER_NAME", "localhost | 127.0.0.1", 1);
    // setenv("SERVER_ADDR", "127.0.0.1", 1);
    // setenv("SERVER_PORT", "3490", 1);
    // setenv("REMOTE_ADDR", "127.0.0.1", 1);
    // setenv("SERVER_SOFTWARE", "TDD/4.0", 1);
    setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
    // setenv("REQUEST_SCHEME", "http", 1);
    // setenv("CONTEXT_PREFIX", "", 1);
    // setenv("SERVER_ADMIN", "", 1);
    // setenv("SCRIPT_FILENAME", "", 1);
    // setenv("REDIRECT_URL", "/", 1);
    // setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
    setenv("REQUEST_METHOD", "POST", 1);
    // setenv("QUERY_STRING", "", 1);

    setenv("REDIRECT_STATUS", "200", 1);
    // setenv("REQUEST_URI", "/post/upload_handler.php", 1);
    setenv("REQUEST_URI", req->path.c_str(), 1);
    setenv("PATH_INFO", "/", 1);

    // setenv("SCRIPT_NAME", "/usr/bin/php-cgi", 1);
    setenv("SCRIPT_NAME", fetch_path(bin).c_str(), 1);
    setenv("SCRIPT_FILENAME", (server->root + req->path).c_str(), 1);
    // setenv("SCRIPT_FILENAME", "./server_root/sito/upload.php", 1);
    setenv("CONTENT_LENGTH", _itoa(req->body.size()).c_str(), 1);
    setenv("CONTENT_TYPE", req->headers.at("Content-Type").c_str(), 1);
    setenv("REDIRECT_STATUS", "true", 1);
    execlp(bin.c_str(), bin.c_str(), (char *)NULL);
  }
  write(io[1], req->body.c_str(), req->body.size());
  close(io[1]);
  waitpid(pid, NULL, 0);
  close(io[0]);
  close(outfile);
  response_path = DFL_TMPFILE;
  remove_tmp = true;
  return OK;
}
