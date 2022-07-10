#pragma once
#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <map>
#include <string>
#include <iostream>
#include <fstream>

class Logger
{
public:
  explicit Logger(bool colored_output = true);
  Logger(const char *filename, bool colored_output = false);
  Logger(std::ostream &out, bool colored_output = false);
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

  std::ostream &_print_log(const std::string &level) const ;
  void _print_timestamp() const ;

  void _init_colors() ;
  std::map<std::string, std::string> _level_colors;

};

#endif // !LOGGER_HPP
