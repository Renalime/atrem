#include "print_ir.h"
#include "../../src/token_list.h"

static void parse_token_list(struct a_token_list *l)
{
}

void print_ir(struct a_alt_list *l)
{
	a_token_list *n;
	if (!l)
		return;
	n = a_get_next_list(l);
	while (n) {
		
	}
}
