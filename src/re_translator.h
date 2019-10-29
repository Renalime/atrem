#ifndef __COMPILATION_H
#define __COMPILATION_H

#define RE_CHAR_TYPE_STR 0
#define RE_CHAR_TYPE_CHAR 1

enum a_re_quantifiers{A_CHAR, A_STAR, A_ESCAPE, A_PLUS, A_BRACES};
enum a_errors{A_NO_ERR, A_ZERO_ESCAPE, A_MEM_ERR};

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
unsigned char a_re_translate(char *reg_exp);
unsigned char a_check_here(char *reg_exp, a_token_list *l);
unsigned char a_escape_token(char *reg_exp, a_token_list *l);


#endif
