#include "cc_token_list.h"
#include "globals.h"
#include <stdlib.h>

a_cc_token_list * a_init_cc_token_list()
{
	a_cc_token_list *list = malloc(sizeof(a_cc_token_list));
	if (list == NULL)
		return NULL;
	list->head = malloc(sizeof(a_cc_token_node));
	if (list->head == NULL)
		return NULL;
	list->current = list->head;
	list->head->next = NULL;
	list->head->a_token = NULL;
	list->last = list->head;
	return list;
}

void a_rm_cc_token_list(a_cc_token_list *cc_l)
{
	a_cc_token_node *it, *tmp;
	if (!cc_l)
		return;
	it = cc_l->head;
	while (it != NULL) {
		tmp = it;
		it = it->next;
		free (tmp->a_token);
		free (tmp);
	}
	free (cc_l);
}

a_cc_token * a_cc_get_next_token(a_cc_token_list *cc_l)
{
	cc_l->current = (cc_l->current ? cc_l->current->next : NULL);
	return (cc_l->current ? cc_l->current->a_token : NULL);
}

unsigned int a_add_cc_token(a_cc_token *token, a_cc_token_list *cc_l)
{
	a_cc_token_node *new_node = malloc(sizeof(a_cc_token_node));
	if (new_node == NULL)
		return A_MEM_ERR;
	new_node->a_token = token;
	new_node->next = NULL;
	cc_l->last->next = new_node;
	cc_l->last = new_node;
	return A_NO_ERR;
}

a_cc_token * a_cc_gen_token(unsigned int type, a_cc_char c)
{
	a_cc_token *new_token = malloc(sizeof(a_cc_token));
	if (new_token == NULL)
		return NULL;
	new_token->a_char_type = type;
	new_token->a_char = c;
	return new_token;
}
