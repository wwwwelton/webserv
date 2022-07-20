//##############################################################################
//#              Copyright(c)2022 Turbo Development Design (TDD)               #
//#                         João Rodriguez - VLN37                             #
//#                         Paulo Sergio - psergio-                            #
//#                         Welton Leite - wleite                              #
//##############################################################################

#include "HttpResponse.hpp"

Response::status_map Response::statuslist = Response::init_status_map();
Response::status_map Response::init_status_map(void) {
  status_map _map;

  _map[200] = "OK\n";
  _map[201] = "CREATED\n";
  _map[202] = "ACCEPTED\n";
  _map[300] = "MULTIPLE CHOICE\n";
  _map[301] = "MOVED PERMANENTLY\n";
  _map[302] = "FOUND\n";
  _map[400] = "BAD REQUEST\n";
  _map[401] = "UNAUTHORIZED\n";
  _map[403] = "FORBIDDEN\n";
  _map[404] = "NOT FOUND\n";
  _map[405] = "METHOD NOT ALLOWED\n";
  _map[415] = "UNSUPPORTED MEDIA TYPE\n";
  _map[500] = "INTERNAL SERVER ERROR\n";
  _map[502] = "BAD GATEWAY\n";
  _map[504] = "GATEWAY TIMEOUT\n";
  _map[506] = "HTTP VERSION UNSUPPORTED\n";
  return _map;
}

Response::meth_map Response::method_map = Response::init_map();
Response::meth_map Response::init_map(void) {
  meth_map _map;

  _map["GET"] = &Response::_get;
  _map["POST"] = &Response::_post;
  _map["DELETE"] = &Response::_delete;
  return _map;
}

Response::function_vector Response::validation_functions = Response::init_pre();
Response::function_vector Response::init_pre(void) {
  function_vector vec;

  vec.push_back(&Response::validate_limit_except);
  vec.push_back(&Response::validate_http_version);
  return vec;
}

Response::function_vector Response::get_functions = Response::init_get();
Response::function_vector Response::init_get(void) {
  function_vector vec;

  vec.push_back(&Response::validate_index);
  vec.push_back(&Response::validate_folder);
  vec.push_back(&Response::validate_path);
  return vec;
}
