//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#ifndef DEFINES_H
#define DEFINES_H

#define DFL_ADDRESS "127.0.0.1"
#define DFL_TIMEOUT 300
#define DFL_LIM_EXCEPT "ALL"
#define DFL_ROOT "root"
#define DFL_CLI_MAX_BODY_SIZE 1
#define DFL_SERVER_ROOT "./"
#define DFL_BACKLOG 500;

#define LOG_LEVEL LVL_DEBUG

#define CFG_FILE_EXT ".conf"
// file size in Kilobytes (KB), 1000KB => 1 Megabytes (MB)
#define CFG_FILE_MAX_SIZE 50

#define OK 200
#define CREATED 201
#define ACCEPTED 202
#define MULTIPLE_CHOICE 300
#define MOVED_PERMANENTLY 301
#define FOUND 302
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
#define DFL_CONTENTLEN "Content-Length: LENGTH\n\n"
#endif  // DEFINES_H
