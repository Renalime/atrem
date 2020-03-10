#include "alteration_list_tests.h"
#include "test_framework.h"
#include "../src/alteration_list.h"
#include "../src/globals.h"

int test_init_alt_list()
{
	_assert(a_init_alt_list() != NULL);
	return 0;
}

int test_add_token_list()
{
	a_alt_list *l = a_init_alt_list();
	_assert(a_add_token_list(NULL, l) == A_NO_ERR);
	return 0;
}

int test_get_next_list()
{
	a_alt_list *l = a_init_alt_list();
	_assert(a_get_next_list(l) == NULL);
	return 0;
}

int test_get_last_list()
{
	a_alt_list *l = a_init_alt_list();
	_assert(a_get_last_list(l) == NULL);
	return 0;
}

int test_all()
{
	_verify(test_init_alt_list);
	_verify(test_add_token_list);
	_verify(test_get_next_list);
	_verify(test_get_last_list);
}

void do_alteration_list_tests()
{
	test_all();
}
