#ifndef __UNIT_TESTS_H
#define __UNIT_TESTS_H

#define PASSED 0
#define N_PASSED 1


//static const char *test_messagdde_delimiter = "*******************************"
static const char *pass_message = ": passed.";
static const char *n_pass_message = ": not passed.";
static char verbose_mode;



void test_units(char verbose);

#endif
