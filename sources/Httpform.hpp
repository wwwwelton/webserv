#ifndef HTTPFORM_HPP
# define HTTPFORM_HPP

typedef struct httpform {
  char *browser;
  char *method;
  char *body;
  char *language;
  int  bodysize;
} t_httpform;

#endif // HTTPFORM_HPP
