#include "alteration_list.h"
#include "globals.h"
#include <stdlib.h>

a_alt_list * a_init_alt_list()
{
	a_alt_list *l = malloc(sizeof(a_alt_list));
	if (l == NULL)
		return NULL;
	l->head = malloc(sizeof(a_alt_node));
	if (l->head == NULL)
		return NULL;
	l->current = l->head;
	l->last = l->head;
	l->head->next = NULL;
	l->head->l = NULL;
	return l;
}

void a_rm_alt_list(a_alt_list *l)
{
	a_alt_node *it, *tmp;
	if (l == NULL)
		return;
	if (l->head == NULL) {
		free(l);
		return;
	}
	it = l->head;
	while (it != NULL) {
		tmp = it;
		it = it->next;
		a_rm_list(tmp->l);
		free(tmp);
	}
}

unsigned char a_add_token_list(a_token_list *tl, a_alt_list *al)
{
	a_alt_node *node = malloc(sizeof(a_alt_node));
	if (!node)
		return A_MEM_ERR;
	node->l = tl;
	node->next = NULL;
	al->last->next = node;
	al->last = node;
	return A_NO_ERR;
}

a_token_list * a_get_next_list(a_alt_list *l)
{
	l->current = l->current->next;
	return (l->current) ? l->current->l : NULL;
}

a_token_list * a_get_last_list(a_alt_list *l)
{
	return l->last->l;
}
