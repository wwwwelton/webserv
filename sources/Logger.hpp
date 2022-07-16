//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#pragma once
#include <ostream>
#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <map>
#include <string>
#include <iostream>
#include <fstream>

enum log_levels {
  LVL_NOTHING,
  LVL_ERROR,
  LVL_WARNING,
  LVL_INFO,
  LVL_DEBUG
};

class Logger
{
public:
  explicit Logger(log_levels log_level = LVL_INFO, bool colored_output = true);
  Logger(const char *filename, log_levels log_level = LVL_INFO, bool colored_output = false);
  Logger(std::ostream &out, log_levels log_level = LVL_INFO, bool colored_output = false);
  Logger(const Logger &);
  Logger &operator=(const Logger &);
  ~Logger();

  std::ostream &debug() const ;
  std::ostream &info() const ;
  std::ostream &warning() const ;
  std::ostream &error() const ;

private:
  std::ofstream file_stream;
  std::ostream& _out;
  bool _colored_output;
  std::map<std::string, std::string> _level_colors;

  log_levels _log_level;


  std::ostream &_print_log(const std::string &level) const ;
  void _print_timestamp() const ;

  void _init_colors() ;

  class NullBuffer: public std::streambuf {
  public:
    int overflow(int c) { return c; };
  };

  class NullStream: public std::ostream {
  public:
    NullStream(): std::ostream(&_null_buffer) { }
  private:
    NullBuffer _null_buffer;
  };

  static NullStream _null_stream;
};

#endif // !LOGGER_HPP
