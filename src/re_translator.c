#include "re_translator.h"
#include <stdlib.h>

a_token_list * a_init_list()
{
	a_token_list *l = malloc(sizeof(a_token_list));
	l = malloc(sizeof(a_token_list));
	l->head = malloc(sizeof(a_token_node));
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

unsigned char a_re_translate(char *reg_exp)
{
	a_token_list *l = a_init_list();
	a_rm_list(l);
	return 0;
}
