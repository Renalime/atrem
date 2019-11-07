#ifndef __CHAR_CLASS_TOKEN_H
#define __CHAR_CLASS_TOKEN_H

typedef struct {
	unsigned char min;
	unsigned char max;
}a_cc_range;

typedef union {
	unsigned char a_char;
	a_cc_range a_range;
}a_cc_char;

typedef struct {
	unsigned char a_char_type;
	a_cc_char a_char;
}a_cc_token;

typedef struct a_cc_token_node a_cc_token_node;

struct a_cc_token_node {
	a_cc_token *a_token;
	a_cc_token_node *next;
};

typedef struct {
	a_cc_token_node *head;
	a_cc_token_node *current;
	a_cc_token_node *last;
}a_cc_token_list;

a_cc_token_list * a_init_cc_token_list();
void a_rm_cc_token_list(a_cc_token_list *cc_l);
a_cc_token * a_cc_get_next_token(a_cc_token_list *cc_l);
unsigned int a_add_cc_token(a_cc_token *token, a_cc_token_list *cc_l);
a_cc_token * a_cc_gen_token(unsigned int type, a_cc_char c);

#endif
