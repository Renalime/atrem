#ifndef __FUNC_TESTS_H
#define __FUNC_TESTS_H

#include "../src/matcher.h"

#define N_PASSED 0
#define P_PASSED 1
#define PASSED 2

typedef struct base_functionality_test{
	unsigned char star_test : 2;
	unsigned char dollar_sign_test: 2;
	unsigned char circumflex_test : 2;
	unsigned char literal_test : 2;
	unsigned char dot_test : 2;
}base_functionality_test_flags;

typedef struct test_information{
	char *n_pass_message;
	char *p_pass_message;
	char *pass_message;
	char *test_string;
	char **test_regexp;
	char n_regexp;
	char overall;
	char passed;
	char expected;
	char expected_ret;
}test_information;


static const char * test_message_delimiter = "-------------------------------";
static char verbose_mode;

void print_test_message(char * text);
char print_test_result(test_information * ti);
char calc_expected(char overall);
void test_match(test_information * ti);
void test_functionality(char verbose);

char test_star();
char test_dollar_sign();
char test_circumflex();
char test_literal();
char test_dot();

#endif
