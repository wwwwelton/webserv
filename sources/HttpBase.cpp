#include "HttpBase.hpp"


char HttpBase::buffer_req[512000] = "";
char HttpBase::buffer_resp[512000] = "HTTP/1.1 200 OK\n"
                                     "Content-Type: text/plain\n"
                                     "Content-Length: 13\n\n"
                                     "Hello world!\n";
int HttpBase::size = 72;
