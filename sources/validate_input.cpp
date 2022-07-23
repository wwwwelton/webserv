//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "validate_input.hpp"

ValidateInputException::ValidateInputException(const std::string& str)
    : LoadException(str) {
  _m = "WebServ Failed to start: " + str;
}

const char* ValidateInputException::what(void) const throw() {
  return (_m.c_str());
}

void validate_input(int argc, char** argv) {
  if (argc < 2)
    throw ValidateInputException("no file provided");
  if (argc > 2)
    throw ValidateInputException("too many arguments");
  if (argv[1][0] == '\0')
    throw ValidateInputException("no file provided");
  std::string file(argv[1]);
  std::string::size_type pos = file.find_last_of(".");
  if (pos == std::string::npos)
    throw ValidateInputException("invalid file extension");
  std::string ext = file.substr(pos);
  if (ext != CFG_FILE_EXT)
    throw ValidateInputException("invalid file extension");
  std::ifstream ifs(argv[1], std::ios::binary | std::ios::ate);
  if (ifs.fail()) {
    ifs.close();
    throw ValidateInputException(strerror(errno));
  }
  if (ifs.tellg() > (CFG_FILE_MAX_SIZE * 1000)) {
    ifs.close();
    throw ValidateInputException("file too large");
  }
}
