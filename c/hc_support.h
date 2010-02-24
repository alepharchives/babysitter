#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 4
#endif

#ifndef SCRIPT_HEADER
#define SCRIPT_HEADER "#!/bin/sh -ex"
#endif

#ifndef YYPARSE_PARAM
#define YYPARSE_PARAM config
#endif

#include "honeycomb_config.h"

#ifdef __cplusplus

#ifndef HC_SUPPORT_H
#define HC_SUPPORT_H

extern "C" {
  int yyparse(void *);
  int yylex(void);  
  int yywrap();
  
  int debug(const long int cur_level, int level, const char *fmt, ...);
  char *phase_type_to_string(phase_type t);
  char *attribute_type_to_string(attr_type t);

  phase_type str_to_phase_type(char *str);
  char *collect_to_period(char *str);
  honeycomb_config* a_new_honeycomb_config_object(void);
  int add_phase(honeycomb_config *c, phase *p);
  int add_attribute(honeycomb_config *c, attr_type t, char *value);
  phase *find_or_create_phase(honeycomb_config *c, phase_type t);
  phase *find_phase(honeycomb_config *c, phase_type t);
  int modify_phase(honeycomb_config *c, phase *p);
  phase* new_phase(phase_type t);
  void free_config(honeycomb_config *c);
  void free_phase(phase *p);
}

#endif 

#else

#ifndef HC_SUPPORT_H
#define HC_SUPPORT_H

extern int yylex(void);
extern int yyparse(void *);
extern int yywrap(void);
int yyerror(const char *str);

int debug(const long int cur_level, int level, char *fmt, ...);
char *phase_type_to_string(phase_type t);
char *attribute_type_to_string(attr_type t);

phase_type str_to_phase_type(char *str);
char *collect_to_period(char *str);
honeycomb_config* a_new_honeycomb_config_object(void);
int add_phase(honeycomb_config *c, phase *p);
int add_attribute(honeycomb_config *c, attr_type t, char *value);
phase *find_or_create_phase(honeycomb_config *c, phase_type t);
phase *find_phase(honeycomb_config *c, phase_type t);
phase* new_phase(phase_type t);
int modify_phase(honeycomb_config *c, phase *p);
void free_config(honeycomb_config *c);
void free_phase(phase *p);


#endif

#endif