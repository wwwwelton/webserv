#include "Log.hpp"
#include <ctime>
#include <iomanip>
#include <sys/time.h>

#define DEBUG   "DEBUG"
#define NORMAL  "NORMAL"
#define INFO    "INFO"
#define WARNING "WARNING"
#define ERROR   "ERROR"

std::ostream&                       Log::_out = std::cout;
bool                                Log::_colored_output = true;
std::map<std::string, std::string>  Log::_level_colors;

void Log::_init_colors() {
  Log::_level_colors[DEBUG]    = "";
  Log::_level_colors[NORMAL]   = "";
  Log::_level_colors[INFO]     = "\x1b[34m";
  Log::_level_colors[WARNING]  = "\x1b[33m";
  Log::_level_colors[ERROR]    = "\x1b[31m";
  Log::_level_colors["reset"]  = "\x1b[0m";
}

void Log::_print_log(const std::string &level, const std::string &str) {
  if (_colored_output) {
    if (Log::_level_colors.empty()) {
      Log::_init_colors();
    }
    Log::_out << _level_colors.at(level);
  }

  Log::_out << "[" << level << " - ";
  _print_timestamp();
  Log::_out << "] " << str;

  if (_colored_output) {
    Log::_out << Log::_level_colors.at("reset");
  }

  Log::_out << std::endl;
}

void Log::_print_timestamp() {
  struct timeval time;
  gettimeofday(&time, 0);
  std::time_t t = time.tv_sec;
  std::tm* now = std::localtime(&t);
  char buf[100];
  std::strftime(buf, 100, "%Y-%m-%d %X", now);
  Log::_out
    << buf << '.'
    << std::setfill('0') << std::setw(3) << time.tv_usec / 1000;
}

void Log::debug(const std::string &str) {
  _print_log(DEBUG, str);
}

void Log::information(const std::string &str) {
  _print_log(INFO, str);
}

void Log::warning(const std::string &str) { 
  _print_log(WARNING, str);
}

void Log::error(const std::string &str) { 
  _print_log(ERROR, str);
}
