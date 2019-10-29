#include "re_translator.h"
#include <stdlib.h>

a_token_list * a_init_list()
{
	a_token_list *l = malloc(sizeof(a_token_list));
	if (l == NULL)
		return NULL;
	l->head = malloc(sizeof(a_token_node));
	if (l->head == NULL)
		return NULL;
	l->current = l->head;
	l->head->a_token = NULL;
	l->head->next = NULL;
	l->last = l->head;
	return l;
}

void a_rm_list(a_token_list *l)
{
	a_token_node *it = l->head->next;
	a_token_node *temp;
	while (it != NULL) {
		temp = it;
		it = it->next;
		free(temp->a_token);
		free(temp);
	}
	free(l->head);
}

a_reg_exp_token * a_get_next_token(a_token_list *l)
{
	l->current = l->current->next;
	if (l->current != NULL)
		return l->current->a_token; 
	else
		return NULL;
}

unsigned char a_add_token(a_reg_exp_token *t, a_token_list *l)
{
	a_token_node *new_node = malloc(sizeof(a_token_node));
	if (new_node == NULL)
		return A_MEM_ERR;
	new_node->a_token = t;
	new_node->next = NULL;
	l->last->next = new_node; 
	l->last = new_node;
	return A_NO_ERR;
}

unsigned char a_check_here(char * reg_exp, a_token_list *l)
{
	if (*reg_exp == '\0')
		return A_NO_ERR;
	if (*reg_exp == '\\')
		return a_escape_token(reg_exp + 1, l);	
	return A_NO_ERR;
}

unsigned char a_escape_token(char *reg_exp, a_token_list *l)
{
	a_char_token text;
	a_reg_exp_token *token;
	if (*reg_exp == '\0')
		return A_ZERO_ESCAPE;
	if (*reg_exp == '\\') {
		text.a_char = '\\';
	} else {
		text.a_char = *reg_exp;
	}
	token = a_gen_token(A_ESCAPE, RE_CHAR_TYPE_CHAR, text, 0);
	if (token == NULL)
		return A_MEM_ERR;
	a_add_token(token, l);
	return a_check_here(reg_exp + 1, l);
}

a_reg_exp_token * a_gen_token(int q, unsigned char type, a_char_token c_token, unsigned char is_negated)
{
	a_reg_exp_token *token = malloc(sizeof(a_reg_exp_token));
	if (token == NULL)
		return NULL;
	token->a_quantifier = q;
	token->a_char_token_type = type;
	token->a_text = c_token;
	token->a_is_negated = is_negated; 
	return token;
}

unsigned char a_re_translate(char *reg_exp)
{
	a_token_list *l = a_init_list();
	if (l == NULL)
		return A_MEM_ERR;
	unsigned char ret = a_check_here(reg_exp, l);
	a_rm_list(l);
	return ret;
}
