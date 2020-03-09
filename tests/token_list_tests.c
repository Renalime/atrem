#include "token_list_tests.h"
#include "test_framework.h"
#include "../src/token_list.h"
#include "../src/globals.h"

unsigned int test_count;

static int inset_token(a_token_list *l);

static int test_all();
static int test_init_list();
static int test_gen_token();
static int test_add_token();
static int test_get_next_token();


static int test_init_list()
{
	a_token_list *l = a_init_list();
	_assert(l != NULL);
	return 0;
}

static int test_gen_token()
{
	a_re_text c_token;
	c_token.a_char = 'a';
	a_reg_exp_token *t = a_gen_token(A_CHAR, 1, c_token, 0);
	_assert(t != NULL);
	_assert(t->a_quantifier == A_CHAR);
	_assert(t->a_re_text_type == 1);
	_assert(t->a_text.a_char == c_token.a_char);
	_assert(t->a_is_negated == 0);
	return 0;
}

static int test_add_token()
{
	a_re_text c_token = {'a'};
	a_token_list *l = a_init_list();
	a_reg_exp_token *t = a_gen_token(A_CHAR, 1, c_token, 0);
	_assert(a_add_token(t, l) == A_NO_ERR);
	return 0;
}

static int test_get_next_token()
{
	a_token_list *l = a_init_list();
	_assert(a_get_next_token(l) == NULL);
	return 0;
}

static int test_all()
{
	_verify(test_init_list);
	_verify(test_gen_token);
	_verify(test_add_token);
	_verify(test_get_next_token);
}

void do_token_list_tests()
{
	test_all();
}
