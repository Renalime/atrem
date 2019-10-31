#ifndef __TOKEN_LIST_H
#define __TOKEN_LIST_H

typedef union {
	char a_char;
	char *a_str;
}a_char_token;

typedef struct {
	int a_quantifier;	
	unsigned char a_char_token_type;
	a_char_token a_text;
	unsigned char a_is_negated;
}a_reg_exp_token;

typedef struct a_token_node a_token_node;

struct a_token_node{
	a_reg_exp_token *a_token;
	a_token_node *next;
};

typedef struct {
	a_token_node *head;
	a_token_node *current;
	a_token_node *last;
}a_token_list;

a_reg_exp_token * a_get_next_token(a_token_list *l);
unsigned char a_add_token(a_reg_exp_token *t, a_token_list *l);
void a_rm_list(a_token_list *l);
a_token_list * a_init_list();
a_reg_exp_token * a_gen_token(int q, unsigned char type, a_char_token text, unsigned char is_negated);

#endif
