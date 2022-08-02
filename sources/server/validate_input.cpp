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

static bool empty_file_name(char* file) {
  std::string _file(file);
  return (_file.empty());
}

static bool invalid_file_extension(char* file) {
  std::string _file(file);
  std::string::size_type pos = _file.find_last_of(".");
  if (pos == std::string::npos)
    return (true);
  std::string ext = _file.substr(pos);
  if (ext != CFG_FILE_EXT)
    return (true);
  return (false);
}

static bool cannot_open_file(char* file) {
  std::ifstream ifs(file);
  if (ifs.fail()) {
    ifs.close();
    return (true);
  }
  return (false);
}

static bool file_too_large(char* file) {
  std::ifstream ifs(file, std::ios::binary | std::ios::ate);
  if (ifs.tellg() > (CFG_FILE_MAX_SIZE * 1000)) {
    ifs.close();
    return (true);
  }
  return (false);
}

void validate_input(int argc, char** argv) {
  if (argc < 2)
    throw ValidateInputException("no file provided");
  if (argc > 2)
    throw ValidateInputException("too many arguments");
  if (empty_file_name(argv[1]))
    throw ValidateInputException("no file provided");
  if (invalid_file_extension(argv[1]))
    throw ValidateInputException("invalid file extension");
  if (cannot_open_file(argv[1]))
    throw ValidateInputException(strerror(errno));
  if (file_too_large(argv[1]))
    throw ValidateInputException("file too large");
}
