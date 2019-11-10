#ifndef __TOKEN_LIST_H
#define __TOKEN_LIST_H

#include "cc_token_list.h"

/* A temporary workaround for having a pointer to a list in the 'a_re_text' structure */

struct a_token_list;

typedef struct a_token_list a_token_list;

typedef union {
	char a_char;
	a_token_list *a_l;
	a_cc_token_list *a_cc_l;
}a_re_text;

typedef struct {
	int min;
	int max;
}a_re_range;

typedef struct {
	int a_quantifier;	
	a_re_range a_braces_range;
	unsigned char a_re_text_type;
	a_re_text a_text;
	unsigned char a_is_negated;
}a_reg_exp_token;

typedef struct a_token_node a_token_node;

struct a_token_node{
	a_reg_exp_token *a_token;
	a_token_node *next;
};

struct a_token_list {
	a_token_node *head;
	a_token_node *current;
	a_token_node *last;
};

a_reg_exp_token * a_get_next_token(a_token_list *l);
unsigned char a_add_token(a_reg_exp_token *t, a_token_list *l);
void a_rm_list(a_token_list *l);
a_token_list * a_init_list();
a_reg_exp_token * a_gen_token(int q, unsigned char type, a_re_text text, unsigned char is_negated);

#endif
