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
    setenv("SCRIPT_FILENAME", "server_root/post/upload_handler.php", 1);
    setenv("SCRIPT_NAME", "upload_handler.php", 1);
    // setenv("REQUEST_URI", "/post/")
    setenv("CONTENT_LENGTH", _itoa(req->body.size() - 1).c_str(), 1);
    setenv("CONTENT_TYPE", "multipart/form-data", 1);
    setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
    setenv("REQUEST_METHOD", "POST", 1);
    setenv("REDIRECT_STATUS", "true", 1);
    setenv("PATH_INFO", "server_root/post/register.html", 1);
    execlp("php-cgi", "php-cgi", (char *)NULL);
  }
  close(io[1]);
  waitpid(pid, NULL, 0);
  close(io[0]);
  std::cout << "here\n";
  return 401;
}
