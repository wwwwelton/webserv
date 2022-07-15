//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#ifndef DEFINES_H
#define DEFINES_H

#define LOG_LEVEL LVL_DEBUG

#define DEFAULT_ADDRESS "127.0.0.1"
#define DEFAULT_TIMEOUT 300
#define DEFAULT_ROUTE_METHOD "ALL"
#define DEFAULT_ROOT "root"
#define DEFAULT_CLI_MAX_BODY_SIZE 1
#define DEFAULT_SERVER_ROOT "server_root"
#define DEFAULT_BACKLOG 500;

#define OK 200
#define ACCEPTED 202
#define BAD_REQUEST 400
#define UNAUTHORIZED 401
#define FORBIDDEN 403
#define NOT_FOUND 404
#define METHOD_NOT_ALLOWED 405
#define INTERNAL_SERVER_ERROR 500
#define BAD_GATEWAY 502
#define GATEWAY_TIMEOUT 504
#define HTTP_VERSION_UNSUPPORTED 506

#define DFL_CONTENTTYPE "Content-Type: text/html; charset=utf-8\n"
#define DFL_CONTENTLEN  "Content-Length: LENGTH\n\n"
#endif  // DEFINES_H
