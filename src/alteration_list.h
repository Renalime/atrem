#ifndef __ALTERATION_LIST_H
#define __ALTERATION_LIST_H

#include "token_list.h"

typedef struct a_alt_node a_alt_node;
typedef struct a_alt_list a_alt_list;

struct a_alt_node {
	a_alt_node *next;
	a_token_list *l;
};

struct a_alt_list {
	a_alt_node *head;
	a_alt_node *current;
	a_alt_node *last;
};

a_token_list * a_get_next_list(a_alt_list *l);
a_token_list * a_get_last_list(a_alt_list *l);
unsigned char a_add_token_list(a_token_list *tl, a_alt_list *al);
unsigned char a_rm_last_token_list(a_alt_list *al);
void a_rm_alt_list(a_alt_list *l);
a_alt_list * a_init_alt_list();
void a_reset_alt_list(a_alt_list *l);

#endif
