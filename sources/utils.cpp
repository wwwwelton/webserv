//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "utils.hpp"

namespace utils {

void validate_input(int argc, char **argv) {
  if (argc < 2)
    throw LoadException("no file provided");
  if (argc > 2)
    throw LoadException("too many arguments");
  if (argv[1][0] == '\0')
    throw LoadException("no file provided");
  std::string file(argv[1]);
  std::string::size_type pos = file.find_last_of(".");
  if (pos == std::string::npos)
    throw LoadException("invalid file extension");
  std::string ext = file.substr(pos);
  if (ext != CFG_FILE_EXT)
    throw LoadException("invalid file extension");
  std::ifstream ifs(argv[1], std::ios::binary | std::ios::ate);
  if (ifs.fail()) {
    ifs.close();
    throw LoadException(strerror(errno));
  }
  if (ifs.tellg() > (CFG_FILE_MAX_SIZE * 1000)) {
    ifs.close();
    throw LoadException("file too large");
  }
}

}  // namespace utils
