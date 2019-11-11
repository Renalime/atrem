#ifndef __COMPILATION_H
#define __COMPILATION_H

#include "token_list.h"
#include "cc_token_list.h"

unsigned char a_re_translate(char *reg_exp);
unsigned char a_check_here(char *reg_exp, a_token_list *l);
unsigned char a_check_cir_flex(char *reg_exp, a_token_list *l);
unsigned char a_escape_token(char *reg_exp, a_token_list *l);
unsigned char a_generic_token(char *reg_exp, unsigned char token_type, a_token_list *l);
unsigned char a_is_valid_char(char c);
unsigned char a_char_token(char *reg_exp, a_token_list *l);
unsigned char a_is_quantifier(char c);
unsigned char a_parse_braces(char *reg_exp, a_token_list *l, a_reg_exp_token *t);
unsigned char a_parse_parens(char *reg_exp, a_token_list *l);
unsigned char a_parse_brackets(char *reg_exp, a_token_list *l);
unsigned char a_add_class_word(unsigned char type, a_cc_token_list *l);
unsigned char a_add_alnum_word(a_cc_token_list *l);
unsigned char a_add_alpha_word(a_cc_token_list *l);
unsigned char a_add_blank_word(a_cc_token_list *l);
unsigned char a_add_cntrl_word(a_cc_token_list *l);
unsigned char a_add_digit_word(a_cc_token_list *l);
unsigned char a_is_class(char *s);
//unsigned char a_cc_parse_hyphen(char *reg_exp, a_token_list *l);
//char * a_check_here_cc(char *reg_exp, a_cc_token_list *l);
//unsigned char a_cc_is_valid_char(char c);
//unsigned char a_cc_generic_token(char c, a_cc_token_list *l);
int a_str_to_int(char *s);
unsigned char a_str_is_equal(char *s1, char *s2);
int a_is_digit(char c);
char * a_find_closing_paren(char *reg_exp);

#endif
