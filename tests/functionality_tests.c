/*
*  Here each test function tests either one type of a metacharacter or a combination of them.
*  Since each function tests different combinations of text and regular expressions, some of them
*  will pass and others won't. In such casses I assume that a test partially passed.
*  A test function can return 3 values: 0 - not passed, 1 - partially passed and 2 - passed.
*  If a test passed partially, then it prints which strings of text passed and which didn't.
*/



#include "functionality_tests.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define INIT_TEST(TEST_NAME, TEST_STRING, TEST_OVERALL) \
			ti.n_pass_message = #TEST_NAME ": not passed";\
			ti.p_pass_message = #TEST_NAME ": passed partially";\
			ti.pass_message = #TEST_NAME ": passed";\
			ti.test_string = #TEST_STRING;\
			ti.overall = TEST_OVERALL;\
			ti.test_regexp = malloc(ti.overall*(sizeof(char *)));\
			ti.passed = 0;\
			ti.expected = calc_expected(ti.overall);\
			ti.expected_ret = 0;\



char calc_expected(char overall)
{
	char i = 0;
	char res = 0;
	for (; i < overall; i++)
		res += (char)pow(2, i);
	return res;
}

void print_test_message(char *text)
{
	printf("%s\n", test_message_delimiter);
	printf("%s\n", text);
	printf("%s\n", test_message_delimiter);
}

char print_test_result(test_information *ti)
{
	char i = 0;
	if (ti->passed == 0) {
		if (verbose_mode)
			print_test_message(ti->n_pass_message);			
		return N_PASSED;			
	} else if (ti->passed == ti->expected) {
		if (verbose_mode)
			print_test_message(ti->pass_message);
		return PASSED;
	} else {
		if (verbose_mode) {
			print_test_message(ti->p_pass_message);
			for (; i < ti->overall; i++){
				if (!(ti->passed & (char)pow(2, i)))
					printf("1:\nText: %s\nRegex: %s\n", ti->test_string, ti->test_regexp[i]);
			}
		}
	}
	free(ti->test_regexp);
	return P_PASSED;
}

void test_match(test_information *ti)
{
	char ret;
	char i = 0;
	for (; i < ti->overall; i++){
		ret = match(ti->test_regexp[i], ti->test_string);	
		if (ret == ((ti->expected_ret & (char)pow(2, i))>>i))
			ti->passed |= (char)pow(2, i);
	}
}

char test_star()
{
	test_information ti;
	INIT_TEST(test_star, cccc wwww, 2);
	ti.test_regexp[0] = "c*";
	ti.test_regexp[1] = "d*abc";
	ti.expected_ret += (char)pow(2, 0);
	test_match(&ti);
	return print_test_result(&ti);
}


char test_dollar_sign()
{
	test_information ti;
	INIT_TEST(test_dollar_sign, qwerty, 2);
	ti.test_regexp[0] = "qwerty$";
	ti.test_regexp[1] = "qwertyu$";
	ti.expected_ret += (char)pow(2, 0);
	test_match(&ti);
	return print_test_result(&ti);
}

char test_circumflex()
{
	test_information ti;
	INIT_TEST(test_circumflex, qwerty, 2);
	ti.test_regexp[0] = "^qwerty";
	ti.test_regexp[1] = "^werty";
	ti.expected_ret += (char)pow(2, 0);
	test_match(&ti);
	return print_test_result(&ti);	
}

char test_literal()
{
	test_information ti;
	INIT_TEST(test_literal, qwerty, 2);
	ti.test_regexp[0] = "qwerty";
	ti.test_regexp[1] = "sfqweqw";
	ti.expected_ret += (char)pow(2,0);
	test_match(&ti);
	return print_test_result(&ti);
}

char test_dot()
{
	test_information ti;
	INIT_TEST(test_dot, qwerty, 2);
	ti.test_regexp[0] = ".werty";
	ti.test_regexp[1] = "qwerty.";
	ti.expected_ret += (char)pow(2,0);
	test_match(&ti);
	return print_test_result(&ti);
}

#define TEST_RESULT(TEST_NAME, FORMAT) \
			if (f->TEST_NAME == 0)\
				printf(FORMAT, #TEST_NAME, pass_message);\
			else if (f->TEST_NAME == 1)\
				printf(FORMAT, #TEST_NAME, n_pass_message);\
			else\
				printf(FORMAT, #TEST_NAME, p_pass_message);\

void print_results(functionality_test_flags *f)
{
	char *pass_message = "passed";
	char *n_pass_message = "not passed";
	char *p_pass_message = "partially passed"; 
	char *format = "%s: %s\n";
	printf("Functionality tests result:\n");
	TEST_RESULT(test_star, format);
	TEST_RESULT(test_dollar_sign, format);
	TEST_RESULT(test_circumflex, format);
	TEST_RESULT(test_literal, format);
	TEST_RESULT(test_dot, format);
}

#define TEST_CALL(TEST_NAME) \
				ret = TEST_NAME();\
				if (ret == PASSED)\
					flags.TEST_NAME = 0;\
				else if (ret == N_PASSED)\
					flags.TEST_NAME = 1;\
				else flags.TEST_NAME = 2;\


void test_functionality(char verbose)
{
	verbose_mode=verbose;
	char ret;
	functionality_test_flags flags;
	TEST_CALL(test_star);
	TEST_CALL(test_dollar_sign);
	TEST_CALL(test_circumflex);
	TEST_CALL(test_literal);
	TEST_CALL(test_dot);
	print_results(&flags);
}
