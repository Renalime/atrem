#ifndef __COMPILATION_H
#define __COMPILATION_H

#include "token_list.h"
#include "alteration_list.h"

unsigned char a_re_translate(char *reg_exp, a_alt_list *al);
unsigned char a_check_here(char *reg_exp, a_alt_list *al);
unsigned char a_check_cir_flex(char *reg_exp, a_alt_list *al);
unsigned char a_escape_token(char *reg_exp, a_token_list *l);
unsigned char a_generic_token(char *reg_exp, unsigned char token_type, a_alt_list *al);
unsigned char a_is_valid_char(char c);
unsigned char a_char_token(char *reg_exp, a_token_list *l);
unsigned char a_is_quantifier(char c);
unsigned char a_parse_braces(char *reg_exp, a_token_list *l, a_reg_exp_token *t);
unsigned char a_parse_parens(char *reg_exp, a_token_list *l);
unsigned char a_parse_brackets(char *reg_exp, a_token_list *l);
unsigned char a_parse_alternation(char *reg_exp, a_alt_list *al);
int a_str_to_int(char *s);
int a_is_digit(char c);
char * a_find_closing_paren(char *reg_exp);

#endif
