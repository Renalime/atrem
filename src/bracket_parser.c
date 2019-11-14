#include "bracket_parser.h"
#include "globals.h"
#include <stdlib.h>

a_tuple_ret a_parse_brackets_init(char *reg_exp, a_cc_token_list *l)
{
	a_cc_char cc_char;
	a_cc_token *token;
	a_tuple_ret ret;
	ret.ret = A_MEM_ERR;
	ret.reg_exp = reg_exp;
	if (*reg_exp == '-' || *reg_exp == ']') {
		cc_char.a_char = *reg_exp;
		if ((token = a_cc_gen_token(A_CHAR, cc_char)) == NULL)
			return ret;
		if (a_add_cc_token(token, l) != A_NO_ERR)
			return ret;
		reg_exp++;
	}
	return a_parse_brackets_check(reg_exp, l);	
}

a_tuple_ret a_parse_brackets_check(char *reg_exp, a_cc_token_list *l)
{
	a_tuple_ret ret;
	ret.reg_exp = reg_exp;
	ret.ret = A_NO_ERR;
	if (*reg_exp == ']')
		return ret;
	if (*(reg_exp + 1) == '-')
		return a_parse_brackets_check_hyphen(reg_exp + 1, l);
	if (*reg_exp == '[' && *(reg_exp + 1) == ':')
		return a_parse_brackets_check_cc(reg_exp, l);
	if (*reg_exp != '\0')
		return a_parse_brackets_check_char(reg_exp, l);
	ret.ret = A_INVALID_RE;
	return ret;
}

a_tuple_ret a_parse_brackets_check_hyphen(char *reg_exp, a_cc_token_list *l)
{
	a_tuple_ret ret;
	a_cc_char cc_char;
	a_cc_token *token;
	ret.ret = A_INVALID_RE;
	ret.reg_exp = reg_exp;
	if (*(reg_exp + 1) == ']') {
		return a_parse_brackets_check_char(reg_exp, l);
	}
	if (*(reg_exp - 1) > *(reg_exp + 1) || *(reg_exp + 1) == '\0')
		return ret;
	cc_char.a_range.min = *(reg_exp - 1);
	cc_char.a_range.max = *(reg_exp + 1);
	ret.ret = A_MEM_ERR;
	if ((token = a_cc_gen_token(A_RANGE, cc_char)) == NULL)
		return ret;
	if (a_add_cc_token(token, l) != A_NO_ERR)
		return ret;
	return a_parse_brackets_check(reg_exp + 2, l);
}

a_tuple_ret a_parse_brackets_check_cc(char *reg_exp, a_cc_token_list *l)
{
	a_tuple_ret ret;
	char class[7];
	unsigned int i;
	unsigned char is_class;
	ret.ret = A_INVALID_RE;
	ret.reg_exp = reg_exp;
	for (i = 0, reg_exp = reg_exp + 2; i < 6 && *reg_exp != '\0' && *reg_exp != ':'; i++, reg_exp++) {
		class[i] = *reg_exp;
	}
	if (!(*reg_exp == ':' && *(reg_exp + 1) == ']'))
		return ret;
	class[i] = '\0';
	is_class = a_is_class(class);
	if (!is_class) {
		return ret;
	}
	if (a_add_class_word(is_class, l) == is_class) {
		return ret;
	}
	return a_parse_brackets_check(reg_exp + 2, l);
}

a_tuple_ret a_parse_brackets_check_char(char *reg_exp, a_cc_token_list *l)
{
	a_cc_char cc_char;
	a_cc_token *token;
	a_tuple_ret ret;
	ret.reg_exp = reg_exp;
	ret.ret = A_MEM_ERR;
	cc_char.a_char = *reg_exp;
	if ((token = a_cc_gen_token(A_CHAR, cc_char)) == NULL)
		return ret;
	if (a_add_cc_token(token, l) != A_NO_ERR)
		return ret;
	return a_parse_brackets_check(reg_exp + 1, l);
}

/* This macroses defined right HERE because they are used in a specific context, particularly in functions that add character classes like [:alnum:], etc. */ 

#define A_CC_ADD_CHAR(token, c, cc_char) do { cc_char.a_char = c; token = a_cc_gen_token(A_CHAR, cc_char); } while(0)
#define A_CC_ADD_RANGE(token, min_v, max_v, cc_char) do { cc_char.a_range.min = min_v; cc_char.a_range.max = max_v; token = a_cc_gen_token(A_RANGE, cc_char); } while(0)
#define A_ASSERT_CC_ADD(token, l) do { if (token == NULL) { return A_MEM_ERR; } if (a_add_cc_token(token, l) != A_MEM_ERR) { return A_MEM_ERR; } } while(0)

unsigned char a_add_alnum_word(a_cc_token_list *l)
{
	a_cc_char cc_char;
	a_cc_token *token;
	A_CC_ADD_RANGE(token, 'A', 'Z', cc_char);
	A_ASSERT_CC_ADD(token, l);
	A_CC_ADD_RANGE(token, 'a', 'z', cc_char);
	A_ASSERT_CC_ADD(token, l);
	A_CC_ADD_RANGE(token, '0', '9', cc_char);
	A_ASSERT_CC_ADD(token, l);
	return A_NO_ERR;
}

unsigned char a_add_alpha_word(a_cc_token_list *l)
{
	a_cc_char cc_char;
	a_cc_token *token;
	A_CC_ADD_RANGE(token, 'A', 'Z', cc_char);
	A_ASSERT_CC_ADD(token, l);
	A_CC_ADD_RANGE(token, 'a', 'z', cc_char);
	A_ASSERT_CC_ADD(token, l);
	return A_NO_ERR;
}

unsigned char a_add_blank_word(a_cc_token_list *l)
{
	a_cc_char cc_char;
	a_cc_token *token;
	A_CC_ADD_CHAR(token, ' ', cc_char);
	A_ASSERT_CC_ADD(token, l);
	A_CC_ADD_CHAR(token, '\t', cc_char);
	A_ASSERT_CC_ADD(token, l);
	return A_NO_ERR;
}

unsigned char a_add_cntrl_word(a_cc_token_list *l)
{
	a_cc_char cc_char;
	a_cc_token *token;
	A_CC_ADD_RANGE(token, 0, 0x1F, cc_char);
	A_ASSERT_CC_ADD(token, l);
	A_CC_ADD_CHAR(token, 0x7E, cc_char);
	A_ASSERT_CC_ADD(token, l);
	return A_NO_ERR;
}

unsigned char a_add_digit_word(a_cc_token_list *l)
{
	a_cc_char cc_char;
	a_cc_token *token;
	A_CC_ADD_RANGE(token, '0', '9', cc_char);
	A_ASSERT_CC_ADD(token, l);
	return A_NO_ERR;
}

unsigned char a_add_graph_word(a_cc_token_list *l)
{
	a_cc_char cc_char;
	a_cc_token *token;
	A_CC_ADD_RANGE(token, 0x21, 0x7E, cc_char);
	A_ASSERT_CC_ADD(token, l);
	return A_NO_ERR;
}

unsigned char a_add_lower_word(a_cc_token_list *l)
{
	a_cc_char cc_char;
	a_cc_token *token;
	A_CC_ADD_RANGE(token, 'a', 'z', cc_char);
	A_ASSERT_CC_ADD(token, l);
	return A_NO_ERR;
}

unsigned char a_add_print_word(a_cc_token_list *l)
{
	a_cc_char cc_char;
	a_cc_token *token;
	A_CC_ADD_RANGE(token, 0x21, 0x7E, cc_char);
	A_ASSERT_CC_ADD(token, l);
	return A_NO_ERR;
}

unsigned char a_add_punct_word(a_cc_token_list *l)
{
	a_cc_char cc_char;
	a_cc_token *token;
	A_CC_ADD_RANGE(token, 0x21, 0x2C, cc_char);
	A_ASSERT_CC_ADD(token, l);
	A_CC_ADD_RANGE(token, 0x2E, 0x2F, cc_char);
	A_ASSERT_CC_ADD(token, l);
	A_CC_ADD_RANGE(token, 0x3A, 0x40, cc_char);
	A_ASSERT_CC_ADD(token, l);
	A_CC_ADD_RANGE(token, 0x5B, 0x60, cc_char);
	A_ASSERT_CC_ADD(token, l);
	A_CC_ADD_RANGE(token, 0x7B, 0x7E, cc_char);
	A_ASSERT_CC_ADD(token, l);
	return A_NO_ERR;
}

unsigned char a_add_space_word(a_cc_token_list *l)
{
	a_cc_char cc_char;
	a_cc_token *token;
	A_CC_ADD_RANGE(token, 0x09, 0x0D, cc_char);
	A_ASSERT_CC_ADD(token, l);
	A_CC_ADD_CHAR(token, ' ', cc_char);
	A_ASSERT_CC_ADD(token, l);
	return A_NO_ERR;
}

unsigned char a_add_upper_word(a_cc_token_list *l)
{
	a_cc_char cc_char;
	a_cc_token *token;
	A_CC_ADD_RANGE(token, 'A', 'Z', cc_char);
	A_ASSERT_CC_ADD(token, l);
	return A_NO_ERR;
}

unsigned char a_add_xdigit_word(a_cc_token_list *l)
{
	a_cc_char cc_char;
	a_cc_token *token;
	A_CC_ADD_RANGE(token, 'A', 'F', cc_char);
	A_ASSERT_CC_ADD(token, l);
	A_CC_ADD_RANGE(token, 'a', 'f', cc_char);
	A_ASSERT_CC_ADD(token, l);
	A_CC_ADD_RANGE(token, '0', '9', cc_char);
	A_ASSERT_CC_ADD(token, l);
	return A_NO_ERR;
}

unsigned char a_add_class_word(unsigned char type, a_cc_token_list *l)
{
	switch (type) {
	case A_CC_ALNUM:
		type = a_add_alnum_word(l);
		break;
	case A_CC_ALPHA:
		type = a_add_alpha_word(l);
		break;
	case A_CC_BLANK:
		type = a_add_blank_word(l);
		break;
	case A_CC_CNTRL:
		type = a_add_cntrl_word(l);
		break;
	case A_CC_DIGIT:
		type = a_add_digit_word(l);
		break;
	case A_CC_GRAPH:
		type = a_add_graph_word(l);
		break;
	case A_CC_LOWER:
		type = a_add_lower_word(l);
		break;
	case A_CC_PRINT:
		type = a_add_print_word(l);
		break;
	case A_CC_PUNCT:
		type = a_add_punct_word(l);
		break;
	case A_CC_SPACE:
		type = a_add_space_word(l);
		break;
	case A_CC_UPPER:
		type = a_add_upper_word(l);
		break;
	case A_CC_XDIGIT:
		type = a_add_xdigit_word(l);
		break;
	}
	return type;
}

unsigned char a_str_is_equal(char *s1, char *s2)
{
	while (*s1 && (*s1++ == *s2++));
	if (*s1 == '\0' && *s2 == '\0')
		return 1;
	else
		return 0;
}

unsigned char a_is_class(char *s)
{
	char *alnum_str = "alnum";
	char *alpha_str = "alpha";
	char *blank_str = "blank";
	char *cntrl_str = "cntrl";
	char *digit_str = "digit";
	char *graph_str = "graph";
	char *lower_str = "lower";
	char *print_str = "print";
	char *punct_str = "punct";
	char *space_str = "space";
	char *upper_str = "upper";
	char *xdigit_str = "xdigit";
	if (*s == 'a') {
		s++;
		if (*s == 'l') {
			s++;
			if (a_str_is_equal(s, alnum_str + 2))
				return A_CC_ALNUM;
			
			if (a_str_is_equal(s, alpha_str + 2))
				return A_CC_ALPHA;
		}
	} else if (*s == 'b') {
		s++;
		if (a_str_is_equal(s, blank_str + 1))
			return A_CC_BLANK;
	} else if (*s == 'c') {
		s++;
		if (a_str_is_equal(s, cntrl_str + 1))
			return A_CC_CNTRL;
	} else if (*s == 'd') {
		s++;
		if (a_str_is_equal(s, digit_str + 1))
			return A_CC_DIGIT;
	} else if (*s == 'g') {
		s++;
		if (a_str_is_equal(s, graph_str + 1))
			return A_CC_GRAPH;
	} else if (*s == 'l') {
		s++;
		if (a_str_is_equal(s, lower_str + 1))
			return A_CC_LOWER;
	} else if (*s == 'p') {
		s++;
		if (*s == 'r') {
			s++;
			if (a_str_is_equal(s, print_str + 2))
				return A_CC_PRINT;
		} else if (*s == 'u') {
			s++;
			if (a_str_is_equal(s, punct_str + 2))
				return A_CC_PUNCT;
		}
	} else if (*s == 's') {
		s++;
		if (a_str_is_equal(s, space_str + 1))
			return A_CC_SPACE;
	} else if (*s == 'u') {
		s++;
		if (a_str_is_equal(s, upper_str + 1))
			return A_CC_UPPER;
	} else if (*s == 'x') {
		s++;
		if (a_str_is_equal(s, xdigit_str + 1))
			return A_CC_XDIGIT;
	}
	return A_CC_ERR;
}
