#include "re_translator.h"
#include <stdlib.h>
#include "globals.h"

unsigned char a_check_here(char *reg_exp, a_token_list *l)
{
	if (*reg_exp == '\0')
		return A_NO_ERR;
	if (*reg_exp == '\\')
		return a_escape_token(reg_exp + 1, l);	
	if (*reg_exp == ')')
		return A_NESTED;
	if (*reg_exp == '(')
		return a_parse_parens(reg_exp + 1, l);
	if (*reg_exp == '$' && *(reg_exp + 1) == '\0')
		return a_generic_token(reg_exp, A_DOLLAR, l);
	if (a_is_valid_char(*reg_exp))
		return a_char_token(reg_exp, l);
	return A_INVALID_RE;
}

/*unsigned char a_check_here_cc(char *reg_exp, a_cc_token_list *l) {
	if (*reg_exp == ']')
		return reg_exp + 1;
	if (*reg_exp == '-')
		
}*/

unsigned char a_parse_brackets(char *reg_exp, a_token_list *l)
{
	a_re_text text;
	a_reg_exp_token *token;
	unsigned char is_quantifier;
	a_cc_token_list *list = a_init_cc_token_list();
	a_cc_token *cc_token;
	a_cc_char cc_char;
	a_cc_range cc_range;
	unsigned char is_negated = 0;
	char class[7];
	unsigned int i;
	unsigned char is_class;
//	unsigned char cc_ret;
	if (list == NULL)
		return A_MEM_ERR; 	
	if (*reg_exp == '^') {
		is_negated = 1;	
		if (*(reg_exp + 1) == '-' || *(reg_exp + 1) == ']') {
			cc_char.a_char = *(reg_exp + 1);
			cc_token = a_cc_gen_token(A_CHAR, cc_char);
			if (cc_token == NULL)
				return A_MEM_ERR;
			a_add_cc_token(cc_token, list);
			reg_exp += 2;
		}
	} else if (*reg_exp == '-' || *reg_exp == ']') {
		cc_char.a_char = *(reg_exp);
		cc_token = a_cc_gen_token(A_CHAR, cc_char);
		if (cc_token == NULL)
			return A_MEM_ERR;
		a_add_cc_token(cc_token, list);
		reg_exp++;
	}
	while (*reg_exp != ']') {
		if ((*reg_exp == '[' && *(reg_exp + 1) != ':') || *reg_exp == '^' || *reg_exp == '\0' || *reg_exp == '\\')
			return A_INVALID_RE;
		if (*reg_exp == '[' && *(reg_exp + 1) == ':') {
			reg_exp += 2;
			for (i = 0; i < 6 && *reg_exp != ':' && *reg_exp != '\0'; i++, reg_exp++) { // i < 6 : because 'xdigit' is the longest possible class word.
				class[i] = *reg_exp;
			}
			if (*reg_exp == '\0' || *reg_exp != ':' || *(reg_exp + 1) != ']')
				return A_INVALID_RE;
			class[i] = '\0';
			is_class = a_is_class(class);
			if (!is_class) {
				return A_INVALID_RE; 
			} else {
				if (a_add_class_word(is_class, list) != A_NO_ERR)
					return A_MEM_ERR;
			}
			reg_exp += 2;
			continue;
		}
		if (*reg_exp == '-') {
			if (*(reg_exp + 1) == ']') {
				cc_char.a_char = *(reg_exp);
				cc_token = a_cc_gen_token(A_CHAR, cc_char);
				a_add_cc_token(cc_token, list);
				reg_exp++;
			} else {
				if (*(reg_exp - 1) > *(reg_exp + 1)) {
					return A_INVALID_RE;
				} else {
					cc_char.a_range.min = *(reg_exp - 1);
					cc_char.a_range.max = *(reg_exp + 1);
					cc_token = a_cc_gen_token(A_RANGE, cc_char);
					reg_exp += 2;
				}
			}
			continue;
		}
		cc_char.a_char = *(reg_exp);
		cc_token = a_cc_gen_token(A_CHAR, cc_char);
		a_add_cc_token(cc_token, list);
		reg_exp++;
	}
}

/* Try to minimize repeating code unsing either macroses or dedicated functions. */

#define A_CC_ADD_CHAR(token, c, cc_char) do { cc_char.a_char = c; token = a_cc_gen_token(A_CHAR, cc_char); } while(0)
#define A_CC_ADD_RANGE(token, min_v, max_v, cc_char) do { cc_char.a_range.min = min_v; cc_char.a_range.max = max_v; token = a_cc_gen_token(A_RANGE, cc_char); } while(0)
#define A_ASSERT_CC_ADD(token, l) do { if (token == NULL) { return A_MEM_ERR; } if (a_add_cc_token(token, l) != A_MEM_ERR) { return A_MEM_ERR; } } while(0)

unsigned char a_add_alnum_word(a_cc_token_list *l)
{
	a_cc_char cc_char;
	cc_char.a_range.min = 'A';
	cc_char.a_range.max = 'Z';
	a_cc_token *token = a_cc_gen_token(A_RANGE, cc_char);
	A_ASSERT_CC_ADD(token, l);
	cc_char.a_range.min = 'a';
	cc_char.a_range.max = 'z';
	token = a_cc_gen_token(A_RANGE, cc_char);
	A_ASSERT_CC_ADD(token, l);
	cc_char.a_range.min = '0';
	cc_char.a_range.max = '9';
	token = a_cc_gen_token(A_RANGE, cc_char);
	A_ASSERT_CC_ADD(token, l);
	return A_NO_ERR;
}

unsigned char a_add_alpha_word(a_cc_token_list *l)
{
	a_cc_char cc_char;
	cc_char.a_range.min = 'A';
	cc_char.a_range.max = 'Z';
	a_cc_token *token = a_cc_gen_token(A_RANGE, cc_char);
	A_ASSERT_CC_ADD(token, l);
	cc_char.a_range.min = 'a';
	cc_char.a_range.max = 'z';
	token = a_cc_gen_token(A_RANGE, cc_char);
	A_ASSERT_CC_ADD(token, l);
	return A_NO_ERR;
}

unsigned char a_add_blank_word(a_cc_token_list *l)
{
	a_cc_char cc_char;
	cc_char.a_char = ' ';
	a_cc_token *token = a_cc_gen_token(A_CHAR, cc_char);
	A_ASSERT_CC_ADD(token, l);
	cc_char.a_char = '\t';
	token = a_cc_gen_token(A_CHAR, cc_char);
	A_ASSERT_CC_ADD(token, l);
	return A_NO_ERR;
}

unsigned char a_add_cntrl_word(a_cc_token_list *l)
{
	a_cc_char cc_char;
	cc_char.a_range.min = 0;
	cc_char.a_range.max = 0x1F;
	a_cc_token *token = a_cc_gen_token(A_RANGE, cc_char);
	A_ASSERT_CC_ADD(token, l);
	cc_char.a_char = 0x7E;
	token = a_cc_gen_token(A_CHAR, cc_char);
	A_ASSERT_CC_ADD(token, l);
	return A_NO_ERR;
}

unsigned char a_add_digit_word(a_cc_token_list *l)
{
	a_cc_char cc_char;
	cc_char.a_range.min = '0';
	cc_char.a_range.max = '9';
	a_cc_token *token = a_cc_gen_token(A_RANGE, cc_char);
	A_ASSERT_CC_ADD(token, l);
	return A_NO_ERR;
}

unsigned char a_add_graph_word(a_cc_token_list *l)
{
	a_cc_char cc_char;
	cc_char.a_range.min = 0x21;
	cc_char.a_range.max = 0x7E;
	a_cc_token *token = a_cc_gen_token(A_RANGE, cc_char);
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

/*unsigned char a_cc_parse_hyphen(char *reg_exp, a_cc_token_list *l)
{
	if (*(reg_exp + 1) == ']'
}*/

unsigned char a_parse_parens(char *reg_exp, a_token_list *l)
{
	a_re_text text;
	a_reg_exp_token *token;
	a_token_list *nested_list = a_init_list();
	unsigned char ret;
	unsigned char is_quantifier;
	if (nested_list == NULL)
		return A_MEM_ERR;
	ret = a_check_here(reg_exp, nested_list);
	if (ret != A_NO_ERR)
		return ret;
	reg_exp = a_find_closing_paren(reg_exp);	
	if (reg_exp == NULL)
		return A_INVALID_RE;
	is_quantifier = a_is_quantifier(*(reg_exp + 1));	
	text.a_l = nested_list;
	token = a_gen_token(is_quantifier, RE_CHAR_TYPE_PARENS, text, 0);
	if (token == NULL)
		return A_MEM_ERR;
	if (is_quantifier == A_BRACES)
		return a_parse_braces(reg_exp + 1, l, token);	
	a_add_token(token, l);
	return (is_quantifier == A_CHAR) ? a_check_here(reg_exp + 1, l) : a_check_here(reg_exp + 2, l);
}

char * a_find_closing_paren(char *reg_exp) 
{
	unsigned int nested = 0;
	while (*reg_exp != '\0') {
		if (*reg_exp == '\\' && (*(reg_exp + 1) == ')' || *(reg_exp + 1) == '(')) {
			reg_exp += 2;	
			continue;
		}
		if (*reg_exp == ')') {
			if (nested == 0)
				return reg_exp;	
			else
				nested--;
		}
		if (*reg_exp == '(')
			nested++;
	}
	return NULL;
}

unsigned char a_char_token(char *reg_exp, a_token_list *l)
{
	a_re_text text;
	a_reg_exp_token *token;	
	unsigned char is_quantifier = a_is_quantifier(*(reg_exp + 1));
	text.a_char = *reg_exp;		
	token = a_gen_token(is_quantifier, RE_CHAR_TYPE_CHAR, text, 0);
	if (token == NULL)
		return A_MEM_ERR;
	if (is_quantifier == A_BRACES)
		return a_parse_braces(reg_exp + 1, l, token);
	a_add_token(token, l);
	return (is_quantifier == A_CHAR) ? a_check_here(reg_exp + 1, l) : a_check_here(reg_exp + 2, l);
}

int a_str_to_int(char *s)
{
	int res = 0;
	while (*s != '\0') {
		res += *s++ - 48;
		res *= 10;
	}
	res /= 10;
	return res;
}

int a_is_digit(char c)
{
	return (c >= '0' && c <= '9') ? 1 : 0;
}

unsigned char a_parse_braces(char *reg_exp, a_token_list *l, a_reg_exp_token *t)
{
	a_re_range ranges;
	int range;
	char num[10];
	unsigned char i = 0;
	while (*reg_exp == ' ' || *reg_exp == '\t')
		reg_exp++;
	while (a_is_digit(*reg_exp))
		num[i++] = *reg_exp++;
	while (*reg_exp == ' ' || *reg_exp == '\t')
		reg_exp++;
	if (*reg_exp != ',')
		return A_INVALID_RE;
	num[i] = '\0';
	range = a_str_to_int(num);	
	if (range == 0)
		ranges.min = -1;
	else
		ranges.min = range;
	reg_exp++;
	i = 0;
	while (*reg_exp == ' ' || *reg_exp == '\t')
		reg_exp++;
	while (a_is_digit(*reg_exp))
		num[i++] = *reg_exp++;
	while (*reg_exp == ' ' || *reg_exp == '\t')
		reg_exp++;
	if (*reg_exp != '}')
		return A_INVALID_RE;
	num[i] = '\0';
	range = a_str_to_int(num);	
	if (range == 0)
		ranges.max = -1;
	else
		ranges.max = range;
	t->a_braces_range = ranges;
	a_add_token(t, l);
	return a_check_here(reg_exp + 1, l);
}

unsigned char a_is_quantifier(char c)
{
	switch(c) {
	case '*':
		return A_STAR;
		break;
	case '+':
		return A_PLUS;
		break;
	case '{':
		return A_BRACES;
		break;
	case '?':
		return A_QUESTION;
		break;	
	}
	return A_CHAR;
}

unsigned char a_check_cir_flex(char *reg_exp, a_token_list *l)
{
	if (*reg_exp == '^')
		return a_generic_token(reg_exp, A_CIR_FLEX, l);
	else
		return a_check_here(reg_exp, l);
}

unsigned char a_escape_token(char *reg_exp, a_token_list *l)
{
	a_re_text text;
	a_reg_exp_token *token;
	if (*reg_exp == '\0')
		return A_INVALID_RE;
	if (*reg_exp == '\\')
		text.a_char = '\\';
	else
		text.a_char = *reg_exp;
	token = a_gen_token(A_ESCAPE, RE_CHAR_TYPE_CHAR, text, 0);
	if (token == NULL)
		return A_MEM_ERR;
	a_add_token(token, l);
	return a_check_here(reg_exp + 1, l);
}

unsigned char a_generic_token(char *reg_exp, unsigned char token_type, a_token_list *l)
{
	a_re_text text;
	a_reg_exp_token *token;
	if (token_type == A_CHAR)
		text.a_char = *reg_exp;	
	else
		text.a_char = 0;
	token = a_gen_token(token_type, RE_CHAR_TYPE_CHAR, text, 0);
	if (token == NULL)
		return A_MEM_ERR;
	a_add_token(token, l);
	return a_check_here(reg_exp + 1, l);
}

unsigned char a_is_valid_char(char c)
{
	return (c != '^' && c != '$' && c != ']' && c != '+' && c != '*') ? 1 : 0;
}

unsigned char a_re_translate(char *reg_exp)
{
	unsigned char ret;
	a_token_list *l = a_init_list();
	if (l == NULL)
		return A_MEM_ERR;
	ret = a_check_cir_flex(reg_exp, l);
	return ret;
}
