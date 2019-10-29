#include "re_translator.h"
#include <stdlib.h>

void a_init_list()
{
	a_rp_list = malloc(sizeof(a_token_list));
	a_rp_list->head = malloc(sizeof(a_token_node));
	a_rp_list->current = a_rp_list->head;
	a_rp_list->head->a_token = NULL;
	a_rp_list->head->next = NULL;
}

void a_rm_list()
{
	a_token_node *it = a_rp_list->head->next;
	a_token_node *temp;
	while (it != NULL) {
		temp = it;
		it = it->next;
		free(temp->a_token);
		free(temp);
	}
	free(a_rp_list->head);
}

a_reg_exp_token * a_get_next_token()
{
	a_rp_list->current = a_rp_list->current->next;
	if (a_rp_list->current != NULL)
		return a_rp_list->current->a_token;
	else
		return NULL;
}

unsigned char a_re_translate(char *reg_exp)
{
	a_init_list();
	a_rm_list();
	return 0;
}
