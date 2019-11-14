#include "token_list.h"
#include <stdlib.h>
#include "globals.h"

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
	if (l == NULL)
		return;
	a_token_node *it = l->head;
	a_token_node *temp;
	while (it != NULL) {
		temp = it;
		it = it->next;
		free(temp->a_token);
		free(temp);
	}
}

a_reg_exp_token * a_get_next_token(a_token_list *l)
{
	l->current = l->current->next;
	return l->current->a_token; 
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

a_reg_exp_token * a_gen_token(int q, unsigned char type, a_re_text c_token, unsigned char is_negated)
{
	a_reg_exp_token *token = malloc(sizeof(a_reg_exp_token));
	if (token == NULL)
		return NULL;
	token->a_quantifier = q;
	token->a_re_text_type = type;
	token->a_text = c_token;
	token->a_is_negated = is_negated; 
	return token;
}
