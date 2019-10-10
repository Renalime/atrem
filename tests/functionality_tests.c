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

void print_test_message(char * text)
{
	printf("%s\n", test_message_delimiter);
	printf("%s\n", text);
	printf("%s\n", test_message_delimiter);
}

char test_star(){
	char *n_pass_message = "test_star: not passed";
	char *p_pass_message = "test_star: passed partially";
	char *pass_message = "test_star: passed";
	char *test_string = "cccc wwww";
	char *test_regexp_present = "c*";
	char *test_regexp_missing = "d*abc";
	char overall = 2;
	char passed = 0;
	char expected = 0;
	char ret;

	for (int i=0; i<overall; i++) {
		expected += (char)pow(2, i);
	}

	ret = match(test_regexp_present, test_string);
	if (ret == 1)
		passed |= (char)0x01;
	ret = match(test_regexp_missing, test_string);
	if (ret == 0)
		passed |= (char)0x02;	

	if (passed == 0) {
		print_test_message(n_pass_message);
		return N_PASSED;			
	} else if (passed == expected) {
		print_test_message(pass_message);
		return PASSED;
	} else {
		print_test_message(p_pass_message);
		if (!(passed & 0x01))
			printf("1:\nText: %s\nRegex: %s\n", test_string, test_regexp_present);
		if (!(passed & 0x02))
			printf("2:\nText: %s\nRegex: %s\n", test_string, test_regexp_missing);
	}
	return P_PASSED;
}

void test_functionality(){
	char ret;
	base_functionality_test_flags flags;
	ret=test_star();
	if (ret == PASSED) {
		flags.star_test=0;
		printf("YEY!\n");
	} else if (ret == P_PASSED) {
		flags.star_test=2;
		printf("Could be better!\n");
	} else {
		flags.star_test=1;
		printf("It's disgusting!\n");
	}
}
