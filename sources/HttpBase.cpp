//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "HttpBase.hpp"

char HttpBase::buffer_req[BUFFER_SIZE] = "";
char HttpBase::buffer_resp[BUFFER_SIZE] = "HTTP/1.1 200 OK\n"
                                     "Content-Type: text/plain\n"
                                     "Content-Length: 13\n\n"
                                     "Hello world!\n";
int HttpBase::size = 72;
