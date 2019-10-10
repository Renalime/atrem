/*
*  This file is mostly for the testing purposes. It's not part of the library.
*  Here each test function tests either one type of a metacharacter or a combinati*  on of them.
*  Since each function tests different combinations of text and regular expression*  s, some of them
*  will pass and others won't. In such casses I assume that a test partially passe*  d.
*  A test function can return 3 values: 0 - not passed, 1 - partially passed and 2*  - passed.
*  If a test passed partially, then it prints which strings of text passed and whi*  ch didn't.
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
	char *test_regexp_present = "*c *w";
	char *test_regexp_missing = "*d";
	char overall = 2;
	char passed = 0;
	char expected = pow(2, overall-1);
	char ret;

	ret = match(test_string, test_regexp_present);
	if (ret == 1)
		passed &= 0x01;
	ret = match(test_string, test_regexp_missing);
	if (ret == 1)
		passed &= 0x02;	

	if (passed == 0) {
		print_test_message(n_pass_message);
		return N_PASSED;			
	} else if (passed == expected) {
		print_test_message(p_pass_message);
		return PASSED;
	} else {
		print_test_message(pass_message);
		if (!(passed & 0x01)) {
			printf("1:\nText: %s\nRegex: %s\n", test_string, test_regexp_present);
		} else if (!(passed & 0x02)) {
			printf("2:\nText: %s\nRegex: %s\n", test_string, test_regexp_missing);
		}
	}
	return P_PASSED;
}

void test_functionality(){
	//char ret;
	//base_functionality_test_flags flags;
	printf("Functionality tests!\n");
	//ret=test_star();
}
