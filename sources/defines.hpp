//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#ifndef DEFINES_H
#define DEFINES_H

#define LOG_LEVEL LVL_INFO

// Server host default
#define DFL_BACKLOG 500
// Server vhost default
#define DFL_ADDRESS "127.0.0.1"
#define DFL_PORT 8080
#define DFL_SERVER_NAME1 "localhost"
#define DFL_SERVER_NAME2 "www.localhost"
#define DFL_SERVER_ROOT "server_root"
#define DFL_SERVER_INDEX "index.html index.php"
#define DFL_404_PAGE "custom_404.html"
#define DFL_405_PAGE "custom_405.html"
#define DFL_TIMEOUT 300
#define DFL_CLI_MAX_BODY_SIZE 1024000000

#define DFL_AUTO_INDEX 0
#define DFL_SOCK_FD -1
#define DFL_UPLOAD 0
#define DFL_UPLOAD_STORE "/tmp"
// Server vhost location default
#define DFL_LIM_EXCEPT "ALL"

#define CFG_FIELD_UNIQUE "server"
#define CFG_FIELD_DOUBLE "error_page cgi return location"
#define CFG_MIN_BACKLOG 1
#define CFG_MAX_BACKLOG 4096
#define CFG_MIN_ERR_CODE 400
#define CFG_MAX_ERR_CODE 499
#define CFG_MIN_TIMEOUT 0
#define CFG_MAX_TIMEOUT 500000000
#define CFG_MIN_CLI_MAX_BODY_SIZE 0
#define CFG_MAX_CLI_MAX_BODY_SIZE 1024000
#define CFG_MIN_RED_CODE 100
#define CFG_MAX_RED_CODE 499
#define CFG_FIELD_LIM_EXCEPT "ALL GET POST PUT DELETE"
#define CFG_MIN_PORT 80
#define CFG_MAX_PORT 65000


#define CFG_FILE_EXT ".conf"
// file size in Kilobytes (KB), 1000KB => 1 Megabytes (MB)
#define CFG_FILE_MAX_SIZE 50

#define CONTINUE 0
#define OK 200
#define CREATED 201
#define ACCEPTED 202
#define NO_CONTENT 204
#define PARTIAL_CONTENT 206
#define MULTIPLE_CHOICE 300
#define MOVED_PERMANENTLY 301
#define FOUND 302

#define BAD_REQUEST 400
#define UNAUTHORIZED 401
#define FORBIDDEN 403
#define NOT_FOUND 404
#define METHOD_NOT_ALLOWED 405
#define NOT_ACCEPTABLE 406
#define PROXY_AUTHENTICATION_REQUIRED 407
#define REQUEST_TIMEOUT 408
#define CONFLICT 409
#define GONE 410
#define LENGTH_REQUIRED 411
#define PRECONDITION_FAILED 412
#define REQUEST_ENTITY_TOO_LARGE 413
#define REQUEST_URI_TOO_LONG 414
#define UNSUPPORTED_MEDIA_TYPE 415
#define REQUESTED_RANGE_NOT_SATISFIABLE 416
#define EXPECTATION_FAILED 417

#define INTERNAL_SERVER_ERROR 500
#define BAD_GATEWAY 502
#define GATEWAY_TIMEOUT 504
#define HTTP_VERSION_UNSUPPORTED 505

#define DFL_CONTENTTYPE "Content-Type: text/html; charset=utf-8\n"
#define DFL_CONTENTLEN "Content-Length: LENGTH\n\n"
#define DFL_SEPARATOR "42__SEPARATOR__42\n"
#define MULTIPART "Content-Type: multipart/byteranges; boundary=" DFL_SEPARATOR
#endif  // DEFINES_H
