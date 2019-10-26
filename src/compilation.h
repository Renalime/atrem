#ifndef __COMPILATION_H
#define __COMPILATION_H

#include <stdint.h>

enum a_re_quantifiers{A_CHAR, A_STAR, A_ESCAPE, A_PLUS, A_BRACES, A_ERROR};
enum a_errors{A_ZERO_ESCAPE, A_COMP_ERR};

typedef union {
	char a_char;
	char *a_str;
}a_char_token;

typedef struct {
	int32_t a_quantifier;	
	uint8_t a_char_token_type;
	a_char_token a_text;
	uint8_t a_is_negated;
	uint8_t a_err;
}a_reg_exp_token;

typedef struct {
	a_reg_exp_token *a_token;
	a_token_node *next;
}a_token_node;

typedef struct {
	a_token_node *head;
	a_token_node *current;
}a_token_list;

a_reg_exp_token * a_get_next_token();
uint8_t a_add_token(a_reg_exp_token *t);
void a_rm_list()

#endif
