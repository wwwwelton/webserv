//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "ResponseBase.hpp"

char ResponseBase::buffer_req[BUFFER_SIZE] = "";
char ResponseBase::buffer_resp[HEADER_SIZE + BUFFER_SIZE] = "";
int  ResponseBase::size = 42;
