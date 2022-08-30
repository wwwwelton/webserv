//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#pragma once
#ifndef SIGNAL_HPP
#define SIGNAL_HPP

#include <cstdlib>
#include <set>
#include <string>
#include <vector>

#include "String.hpp"
#include "WebServ.hpp"

class WebServ;
class Config;

void sighandler(const int signal, void* ptr);
void init_signals(WebServ* ptr);

#endif  // SIGNAL_HPP
