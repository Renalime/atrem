#include "unit_tests.h"
#include "test_framework.h"
#include "../src/matcher.h"

unsigned int test_count = 0;

int test_matchhere_01()
{
	_assert(matchhere("aw$", "aw") == 1);
	return 0;
}

int test_matchhere_02()
{
	_check(matchhere("qwerty.", "qwertyu") == 1);
	return 0;
}

int test_matchhere_03()
{
	_assert(matchhere("q?werty", "qwerty") == 1);
	return 0;
}

int test_matchhere_04()
{
	_assert(matchhere("q?werty", "werty") == 1);
	return 0;
}

int test_matchhere_05()
{
	_assert(matchhere("qwerty\\?", "qwerty?") == 1);
	return 0;
}
int test_matchstar_01()
{
	_assert(matchstar('.', "qwerty", "ccccqwerty") == 1);
	return 0;
}

int test_matchstar_02()
{
	_assert(matchstar('.', "?)", "(cccccc)wwwww") == 1);
	return 0;
}

int test_match()
{
	_assert(match("^qwerty", "qwertyabc") == 1);
	return 0; 
}

int test_match_plus_01()
{
	_assert(match_plus('.', "?", "w") == 1);	
	return 0;
}

int test_match_plus_02()
{
	_assert(match_plus('c', "ccqwerty", "ccccqwerty") == 1);
	return 0;
}

int test_match_escape_01()
{
	_assert(match_escape("\\qwerty", "\\qwerty") == 1);
	return 0;
}

int test_match_escape_02()
{
	_assert(match_escape("+qwerty", "+qwerty") == 1);
	return 0;
}

int test_match_escape_03()
{
	_assert(match_escape("", "qwerty") == 0);
	return 0;
}

int all_tests()
{
	_verify(test_matchhere_01);
	_verify(test_matchhere_02);
	_verify(test_matchhere_03);
	_verify(test_matchhere_04);
	_verify(test_matchhere_05);
	_verify(test_matchstar_01);
	_verify(test_matchstar_02);
	_verify(test_match);
	_verify(test_match_plus_01);
	_verify(test_match_plus_02);
	_verify(test_match_escape_01);
	_verify(test_match_escape_02);
	_verify(test_match_escape_03);
	return 0;
}

void test_units(char verbose)
{
	all_tests();
}
