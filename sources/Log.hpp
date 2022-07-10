#pragma once
#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <map>
#include <string>
#include <iostream>

class Log
{
public:
  Log(std::ostream& out = std::cout, bool colored_output = true);
  Log(const Log &);
  ~Log();

  static void debug(const std::string &str) ;
  static void information(const std::string &str) ;
  static void warning(const std::string &str) ;
  static void error(const std::string &str) ;

private:
  static std::ostream& _out;
  static bool _colored_output;
  static std::map<std::string, std::string> _level_colors;

  static void _init_colors() ;
  static void _print_log(const std::string &level, const std::string &str) ;
  static void _print_timestamp() ;

};

#endif // !LOGGER_HPP
