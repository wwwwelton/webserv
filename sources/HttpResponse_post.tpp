//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "HttpResponse.hpp"

int Response::_post(void) {
  // std::string   file;
  // std::ofstream ofile;
  // std::stringstream stream(req_body);

  // std::getline(stream, file, '\n');
  // std::getline(stream, file, '\n');
  // std::getline(stream, file, '\n');
  // std::getline(stream, file, '\n');
  // // file = location->upload_store + "/" + file; // remove
  // access(file.c_str(), W_OK);
  // if (errno == EACCES)
  //   return FORBIDDEN;
  // ofile.open(file.c_str(), ofile.out | ofile.trunc);
  // if (!ofile.good()) {
  //   WebServ::log.warning() << "file creation failed returning 500\n";
  //   return INTERNAL_SERVER_ERROR;
  // }
  // ofile << req_body;
  // return CREATED;

  int pid;
  int io[2];

  pipe(io);
  WebServ::log.error() << req->body;
  WebServ::log.warning() << "test\n";
  write(io[1], req->body.c_str(), req->body.size());
  pid = fork();
  if (pid == 0) {
    close(io[1]);
    dup2(io[0], STDIN_FILENO);

    // setenv("HTTP_ACCEPT", "", 1);
    // setenv("HTTP_SEC_FETCH_USER", "?1", 1); // ?
    // setenv("HTTP_SEC_FETCH_DEST", "", 1);   // ?

    // setenv("REDIRECT_HANDLER", "", 1);
    setenv("REDIRECT_STATUS", "200", 1);
    // setenv("HTTP_HOST", "", 1);
    // setenv("HTTP_CONNECTION", "", 1);
    // setenv("CONTENT_TYPE", "", 1);
    // setenv("HTTP_ACCEPT_LANGUAGE", "", 1);
    // setenv("HTTP_SEC_GPC", "1", 1);
    // setenv("HTTP_SEC_FETCH_MODE", "", 1);
    // setenv("HPTTP_REFERER", "", 1);
    // setenv("HTTP_ACEPT_ENCODING", "", 1);

    setenv("AUTH_TYPE", "", 1);
    // setenv("SERVER_SOFTWARE", "", 1);
    setenv("SERVER_NAME", "localhost | 127.0.0.1", 1);
    setenv("SERVER_ADDR", "127.0.0.1", 1);
    setenv("SERVER_PORT", "3490", 1);
    setenv("REMOTE_ADDR", "127.0.0.1", 1);
    setenv("SERVER_SOFTWARE", "TDD/4.0", 1);
    setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
    // setenv("DOCUMENT_ROOT", "/home/vln37/workspace/42cursus/lvl15_webserv", 1);
    // setenv("REQUEST_SCHEME", "http", 1);
    // setenv("CONTEXT_PREFIX", "", 1);
    // setenv("SERVER_ADMIN", "", 1);
    // setenv("SCRIPT_FILENAME", "", 1);
    setenv("REDIRECT_URL", "/", 1);
    setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
    setenv("REQUEST_METHOD", "POST", 1);
    setenv("QUERY_STRING", "", 1);

    setenv("REQUEST_URI", "/post/upload_handler.php", 1);
    setenv("PATH_INFO", "/", 1);
    setenv("PATH_TRANSLATED", "/home/vln37/workspace/42cursus/lvl15_webserv/server_root/post/upload_handler.php", 1);
    // setenv("SCRIPT_NAME", "/upload_handler.php", 1);
    setenv("SCRIPT_NAME", "/usr/bin/php-cgi", 1);
    // setenv("SCRIPT_FILENAME", "/home/vln37/workspace/42cursus/lvl15_webserv/server_root/post/upload_handler.php", 1);
    setenv("SCRIPT_FILENAME", "/home/vln37/workspace/42cursus/lvl15_webserv/server_root/post/upload_handler.php", 1);
    // setenv("REQUEST_URI", "/post/")
    setenv("CONTENT_LENGTH", _itoa(req->body.size()).c_str(), 1);
    setenv("CONTENT_TYPE", "multipart/form-data; boundary=------WebKitFormBoundaryoPWer5AwvkzlAciI", 1);
    setenv("REDIRECT_STATUS", "true", 1);
    execlp("php-cgi", "-R", (char *)NULL);
  }
  close(io[1]);
  waitpid(pid, NULL, 0);
  close(io[0]);
  std::cout << "here\n";
  return 401;
}
