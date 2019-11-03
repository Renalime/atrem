#ifndef __COMPILATION_H
#define __COMPILATION_H

#include "token_list.h"

unsigned char a_re_translate(char *reg_exp);
unsigned char a_check_here(char *reg_exp, a_token_list *l);
unsigned char a_check_cir_flex(char *reg_exp, a_token_list *l);
unsigned char a_escape_token(char *reg_exp, a_token_list *l);
unsigned char a_generic_token(char *reg_exp, unsigned char token_type, a_token_list *l);
unsigned char a_is_valid_char(char c);
unsigned char a_char_token(char *reg_exp, a_token_list *l);
unsigned char a_is_quantifier(char c);
unsigned char a_parse_braces(char *reg_exp, a_token_list *l, a_reg_exp_token *t);
int a_str_to_int(char *s);
int a_is_digit(char c);

#endif
