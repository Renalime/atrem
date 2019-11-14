#ifndef __BRACKET_PARSER_H
#define __BRACKET_PARSER_H

#include "cc_token_list.h"

/* The next struct is just a convinience for returning the next chacacter to process in a string and at the same time the function exit code. */

typedef struct {
	char *reg_exp;
	unsigned char ret;
}a_tuple_ret;

a_tuple_ret a_parse_brackets_init(char *reg_exp, a_cc_token_list *l);
a_tuple_ret a_parse_brackets_check(char *reg_exp, a_cc_token_list *l);
a_tuple_ret a_parse_brackets_check_hyphen(char *reg_exp, a_cc_token_list *l);
a_tuple_ret a_parse_brackets_check_cc(char *reg_exp, a_cc_token_list *l);
a_tuple_ret a_parse_brackets_check_char(char *reg_exp, a_cc_token_list *l);
unsigned char a_add_class_word(unsigned char type, a_cc_token_list *l);
unsigned char a_add_alnum_word(a_cc_token_list *l);
unsigned char a_add_alpha_word(a_cc_token_list *l);
unsigned char a_add_blank_word(a_cc_token_list *l);
unsigned char a_add_cntrl_word(a_cc_token_list *l);
unsigned char a_add_digit_word(a_cc_token_list *l);
unsigned char a_add_graph_word(a_cc_token_list *l);
unsigned char a_add_lower_word(a_cc_token_list *l);
unsigned char a_add_print_word(a_cc_token_list *l);
unsigned char a_add_punct_word(a_cc_token_list *l);
unsigned char a_add_space_word(a_cc_token_list *l);
unsigned char a_add_upper_word(a_cc_token_list *l);
unsigned char a_add_xdigit_word(a_cc_token_list *l);
unsigned char a_is_class(char *s);
unsigned char a_str_is_equal(char *s1, char *s2);

#endif
