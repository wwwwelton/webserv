//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "Response.hpp"

void Response::create_directory_listing(void) {
  std::string       _template;
  std::ofstream     out;
  std::string       tmp;

  file.open("./sources/templates/index.html");
  out.open(DFL_DYNFILE, out.out | out.trunc | std::ios::binary);

  file.get(*(out.rdbuf()), '$');
  file.ignore();
  std::getline(file, tmp);
  tmp.push_back('\n');
  tmp.replace(tmp.find("DIRNAME"), 7, path.substr(path.find_last_of('/')));
  out << tmp;

  file.get(*(out.rdbuf()), '$');
  file.ignore();
  std::getline(file, _template);
  _template.push_back('\n');

  struct dirent *dir;
  DIR* directory;
  directory = opendir(path.c_str());
  dir = readdir(directory);
  while (dir != NULL) {
    tmp = _template;
    if (req->path[req->path.size() - 1] == '/')
      tmp.replace(tmp.find("PATH"), 4, req->path + dir->d_name);
    else
      tmp.replace(tmp.find("PATH"), 4, req->path + "/" + dir->d_name);
    tmp.replace(tmp.find("LINK"), 4, dir->d_name);
    out << tmp;
    dir = readdir(directory);
  }

  out << file.rdbuf();
  closedir(directory);
  file.close();
  out.close();
  remove_tmp = true;
}

void Response::create_error_page(void) {
  std::string       content;
  std::ifstream     infile;
  std::ofstream     outfile;

  infile.open("./sources/templates/error.html");
  outfile.open(DFL_DYNFILE, outfile.trunc);
  content.assign(std::istreambuf_iterator<char>(infile),
                 std::istreambuf_iterator<char>());
  content.replace(content.find("PLACEHOLDER"), 11, statuscode + statusmsg);
  content.replace(content.find("PLACEHOLDER"), 11, statuscode + statusmsg);
  outfile << content;
  remove_tmp = true;
  infile.close();
  outfile.close();
  response_path = DFL_DYNFILE;
}

void Response::create_redir_page(void) {
  std::string       content;
  std::ifstream     infile;
  std::ofstream     outfile;

  infile.open("./sources/templates/redirect.html");
  outfile.open(DFL_DYNFILE, outfile.trunc);
  content.assign(std::istreambuf_iterator<char>(infile),
                 std::istreambuf_iterator<char>());
  // WebServ::log.error() << content;
  content.replace(content.find("$URL"), 4, location->redirect.second);
  content.replace(content.find("$URL"), 4, location->redirect.second);
  // WebServ::log.error() << content;
  outfile << content;
  response_path = DFL_DYNFILE;
  remove_tmp = true;
  infile.close();
  outfile.close();
}
