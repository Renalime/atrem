#ifndef __COMPILATION_H
#define __COMPILATION_H

#define NULL 0

enum a_re_quantifiers{A_CHAR, A_STAR, A_ESCAPE, A_PLUS, A_BRACES, A_ERROR};
enum a_errors{A_ZERO_ESCAPE, A_COMP_ERR};

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
}a_token_list;

a_token_list *a_rp_list;

a_reg_exp_token * a_get_next_token();
unsigned char a_add_token(a_reg_exp_token *t);
void a_rm_list();
void a_init_list();
unsigned char a_re_translate(char *reg_exp);



#endif
