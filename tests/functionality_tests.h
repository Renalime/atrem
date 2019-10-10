#ifndef __FUNC_TESTS_H
#define __FUNC_TESTS_H

#include "../src/matcher.h"

#define N_PASSED 0;
#define P_PASSED 1;
#define PASSED 2;

typedef struct base_functionality_test{
	unsigned char star_test : 1;
	unsigned char dollar_sign_test: 1;
	unsigned char circumflex_test : 1;
	unsigned char literal_test : 1;
	unsigned char dot_test : 1;
}base_functionality_test_flags;

static const char * test_message_delimiter = "-------------------------------";

void print_test_message(char * text);
void test_functionality();

char test_star();
char test_dollar_sign();
char test_circumflex();
char test_literal();
char test_dot();

#endif
