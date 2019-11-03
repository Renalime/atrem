#ifndef __COMPILATION_H
#define __COMPILATION_H

#include "token_list.h"

unsigned char a_re_translate(char *reg_exp);
unsigned char a_check_here(char *reg_exp, a_token_list *l);
unsigned char a_check_cir_flex(char *reg_exp, a_token_list *l);
unsigned char a_escape_token(char *reg_exp, a_token_list *l);
unsigned char a_dollar_token(char *reg_exp, a_token_list *l);
unsigned char a_cir_flex_token(char *reg_exp, a_token_list *l);
unsigned char a_generic_token(char *reg_exp, unsigned char token_type, a_token_list *l);
unsigned char a_is_valid_char(char c);

#endif
