#include "token_list_tests.h"
#include "test_framework.h"
#include "../src/token_list.h"

unsigned int test_count;

static int test_all();
static int test_init_list();

static int test_init_list()
{
	a_token_list *l = a_init_list();
	_assert(l != NULL);
	return 0;
}

static int test_all()
{
	_verify(test_init_list);
}

void do_token_list_tests()
{
	test_all();
}
