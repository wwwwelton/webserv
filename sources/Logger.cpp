#include "Logger.hpp"
#include <ctime>
#include <iomanip>
#include <sys/time.h>

#define DEBUG   "DEBUG"
#define NORMAL  "NORMAL"
#define INFO    "INFO"
#define WARNING "WARNING"
#define ERROR   "ERROR"

void Logger::_init_colors() {
  this->_level_colors[DEBUG]    = "";
  this->_level_colors[NORMAL]   = "";
  this->_level_colors[INFO]     = "\x1b[34m";
  this->_level_colors[WARNING]  = "\x1b[33m";
  this->_level_colors[ERROR]    = "\x1b[31m";
  this->_level_colors["reset"]  = "\x1b[0m";
}

Logger::Logger(bool colored_output) : _out(std::cout) {
  this->_colored_output = colored_output;
  if (colored_output)
    _init_colors();
}

Logger::Logger(std::ostream& out, bool colored_output) : _out(out) {
  _colored_output = colored_output;
  if (colored_output)
    _init_colors();
}

Logger::Logger(const Logger &other): _out(other._out) {
  *this = other;
}

Logger& Logger::operator=(const Logger &other) {
  _colored_output = other._colored_output;
  if (_colored_output)
    _level_colors = other._level_colors;
  return *this;
}

Logger::~Logger() { }

std::ostream &Logger::_print_log(const std::string &level) const {
  if (_colored_output) {
    this->_out << _level_colors.at(level);
  }

  this->_out << "[" << level << " - ";
  _print_timestamp();
  this->_out << "] ";

  return this->_out;
}

void Logger::_print_timestamp() const {
  struct timeval time;
  gettimeofday(&time, 0);
  std::time_t t = time.tv_sec;
  std::tm* now = std::localtime(&t);
  char buf[100];
  std::strftime(buf, 100, "%Y-%m-%d %X", now);
  this->_out
    << buf << '.'
    << std::setfill('0') << std::setw(3) << time.tv_usec / 1000;
}

std::ostream &Logger::debug() const {
  return _print_log(DEBUG);
}

std::ostream &Logger::info() const {
  return _print_log(INFO);
}

std::ostream &Logger::warning() const { 
  return _print_log(WARNING);
}

std::ostream &Logger::error() const { 
  return _print_log(ERROR);
}
