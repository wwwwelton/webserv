//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "HttpBase.hpp"

char HttpBase::buffer_req[BUFFER_SIZE] = "";
char HttpBase::buffer_resp[HEADER_SIZE + BUFFER_SIZE] = "";
int  HttpBase::size = 42;
