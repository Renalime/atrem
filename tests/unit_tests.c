#include "unit_tests.h"
#include <stdio.h>
#include "../src/matcher.h"

#define FAIL() printf("Failure in %s() line %d\n", __func__, __LINE__)
#define SUCCESS() printf("%s(): passed\n", __func__)
#define _assert(test) do { if (!(test)) { FAIL(); return 1; } else { SUCCESS(); } } while(0)
#define _check(test) do { if (!(test)) { FAIL(); } else { SUCCESS(); }
#define _verify(test) do { int r = test(); tests_run++; if (r) return r; } while(0)

static int tests_run = 0;

int test_matchhere_01()
{
	_assert(matchhere("aw$", "aw") == 1);
	return 0;
}

int all_tests()
{
	_verify(test_matchhere_01);
	return 0;
}

void test_units(char verbose){
	verbose_mode = verbose;
	all_tests();
}
