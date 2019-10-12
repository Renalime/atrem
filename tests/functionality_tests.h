#ifndef __FUNC_TESTS_H
#define __FUNC_TESTS_H

#include "../src/matcher.h"

#define N_PASSED 0
#define P_PASSED 1
#define PASSED 2

typedef struct functionality_test_flags{
	unsigned char star_test : 2;
	unsigned char dollar_sign_test: 2;
	unsigned char circumflex_test : 2;
	unsigned char literal_test : 2;
	unsigned char dot_test : 2;
}functionality_test_flags;

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
void call_test_star(functionality_test_flags *f);
char test_dollar_sign();
void call_test_dollar_sign(functionality_test_flags *f);
char test_circumflex();
void call_test_circumflex(functionality_test_flags *f);
char test_literal();
void call_test_literal(functionality_test_flags *f);
char test_dot();
void call_test_dot(functionality_test_flags *f);

#endif
