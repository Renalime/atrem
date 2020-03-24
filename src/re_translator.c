#include "re_translator.h"
#include <stdlib.h>
#include "globals.h"
#include "bracket_parser.h"

static unsigned char a_cf_dollar_token(char *reg_exp, unsigned char token_type, a_alt_list *al);
static inline unsigned char a_is_valid_char(char c);
static inline unsigned char a_check_cir_flex(char *reg_exp, a_alt_list *al);
static inline int a_is_digit(const char c);
static unsigned char a_parse_parens(char *reg_exp, a_alt_list *l);
static char * a_find_closing_paren(char *reg_exp);
static unsigned char a_parse_brackets(char *reg_exp, a_alt_list *l);
static unsigned char a_check_here(char *reg_exp, a_alt_list *al);
static unsigned char a_char_token(char *reg_exp, a_alt_list *al);
static unsigned char a_is_quantifier(char c);
static unsigned char a_parse_braces(char *reg_exp, a_alt_list *al, a_reg_exp_token *t);
static unsigned char a_parse_dot(char *reg_exp, a_alt_list *al);
static int a_str_to_int(const char *s);


unsigned char a_parse_brackets(char *reg_exp, a_alt_list *l)
{
	a_re_text text;
	a_reg_exp_token *token;
	unsigned char is_quantifier;
	a_cc_token_list *list = a_init_cc_token_list();
	if (!list)
		return A_MEM_ERR;
	unsigned char is_negated = 0;
	a_tuple_ret ret;
	if (!list)
		return A_MEM_ERR;
	if (*reg_exp == '^') {
		is_negated = 1;
		reg_exp++;
	}
	ret = a_parse_brackets_init(reg_exp, list);
	if (ret.ret != A_NO_ERR)
		return ret.ret;
	reg_exp = ret.reg_exp + 1;
	is_quantifier = a_is_quantifier(*reg_exp);
	text.a_cc_l = list;
	token = a_gen_token(is_quantifier, RE_CHAR_TYPE_BRACKETS, text, is_negated);
	if (token == NULL)
		return A_MEM_ERR;
	if (is_quantifier == A_BRACES)
		return a_parse_braces(reg_exp + 1, l, token);
	if (a_add_token(token, a_get_last_list(l)) != A_NO_ERR)
		return A_MEM_ERR;
	return (is_quantifier == A_CHAR) ? a_check_here(reg_exp, l) : a_check_here(reg_exp + 1, l);
}

static char * a_find_closing_paren(char *reg_exp)
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
		reg_exp++;
	}
	return NULL;
}

static unsigned char a_parse_parens(char *reg_exp, a_alt_list *l)
{
	a_re_text text;
	a_reg_exp_token *token;
	a_alt_list *nested_list = a_init_alt_list();
	unsigned char ret;
	unsigned char is_quantifier;
	if (nested_list == NULL)
		return A_MEM_ERR;
	ret = a_re_translate(reg_exp, nested_list);
	if (ret != A_NESTED)
		return A_INVALID_RE;
	reg_exp = a_find_closing_paren(reg_exp);
	if (reg_exp == NULL)
		return A_INVALID_RE;
	is_quantifier = a_is_quantifier(*(reg_exp + 1));
	text.a_l = nested_list;
	token = a_gen_token(is_quantifier, RE_CHAR_TYPE_PARENS, text, 0);
	if (token == NULL)
		return A_MEM_ERR;
	if (is_quantifier == A_BRACES)
		return a_parse_braces(reg_exp + 2, l, token);
	a_add_token(token, a_get_last_list(l));
	return (is_quantifier == A_CHAR) ? a_check_here(reg_exp + 1, l) : a_check_here(reg_exp + 2, l);
}

static int a_str_to_int(const char *s)
{
	int res = 0;
	if (*s == '\0')
		return -1;
	while (*s != '\0') {
		res += *s++ - 48;
		res *= 10;
	}
	res /= 10;
	return res;
}

static inline
int a_is_digit(const char c)
{
	return (c >= '0' && c <= '9') ? 1 : 0;
}

static unsigned char a_is_quantifier(char c)
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


static unsigned char a_parse_braces(char *reg_exp, a_alt_list *al, a_reg_exp_token *t)
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
	num[i] = '\0';
	range = a_str_to_int(num);
	if (*reg_exp != '}') {
		if (*reg_exp != ',')
			return A_INVALID_RE;
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
		ranges.max = range;
	} else {
		ranges.min = ranges.max = range;
	}
	t->a_braces_range = ranges;
	a_add_token(t, a_get_last_list(al));
	return a_check_here(reg_exp + 1, al);
}

static unsigned char a_parse_dot(char *reg_exp, a_alt_list *al)
{
	a_re_text text;
	a_reg_exp_token *token;
	unsigned char is_quantifier = a_is_quantifier(*(reg_exp + 1));
	text.a_char = 0;
	token = a_gen_token(is_quantifier, RE_CHAR_TYPE_CHAR, text, 0);
	if (token == NULL)
		return A_MEM_ERR;
	if (is_quantifier == A_BRACES)
		return a_parse_braces(reg_exp + 2, al, token);
	a_add_token(token, a_get_last_list(al));
	return (is_quantifier == A_CHAR) ? a_check_here(reg_exp + 1, al) : a_check_here(reg_exp + 2, al);
}

static unsigned char a_char_token(char *reg_exp, a_alt_list *al)
{
	a_re_text text;
	a_reg_exp_token *token;
	unsigned char is_quantifier = a_is_quantifier(*(reg_exp + 1));
	text.a_char = *reg_exp;
	token = a_gen_token(is_quantifier, RE_CHAR_TYPE_CHAR, text, 0);
	if (token == NULL)
		return A_MEM_ERR;
	if (is_quantifier == A_BRACES)
		return a_parse_braces(reg_exp + 2, al, token);
	a_add_token(token, a_get_last_list(al));
	return (is_quantifier == A_CHAR) ? a_check_here(reg_exp + 1, al) : a_check_here(reg_exp + 2, al);
}

static inline
unsigned char a_is_valid_char(char c)
{
	return (c != '^' && c != '$' && c != ']' && c != '+' && c != '*') ? 1 : 0;
}


static unsigned char a_cf_dollar_token(char *reg_exp, unsigned char type, a_alt_list *al)
{
	a_re_text text;
	a_reg_exp_token *token;
	text.a_char = 0;
	token = a_gen_token(type, RE_CHAR_TYPE_CHAR, text, 0);
	if (token == NULL)
		return A_MEM_ERR;
	a_add_token(token, a_get_last_list(al));
	return a_check_here(reg_exp + 1, al);
}

static inline
unsigned char a_check_cir_flex(char *reg_exp, a_alt_list *al)
{
	return (*reg_exp == '^' ? a_cf_dollar_token(reg_exp, A_CIR_FLEX, al) : a_check_here(reg_exp, al));
}

static unsigned char a_check_here(char *reg_exp, a_alt_list *al)
{
	if (*reg_exp == '\0')
		return A_NO_ERR;
	if (*reg_exp == '(')
		return a_parse_parens(reg_exp + 1, al);
	if (*reg_exp == '[')
		return a_parse_brackets(reg_exp + 1, al);
	if (*reg_exp == '$' && *(reg_exp + 1) == '\0')
		return a_cf_dollar_token(reg_exp, A_DOLLAR, al);
	if (*reg_exp == '|')
		return a_re_translate(reg_exp + 1, al);
	if (*reg_exp == ')')
		return A_NESTED;
	if (*reg_exp == '.')
		return a_parse_dot(reg_exp, al);
	if (*reg_exp == '\\' && *(reg_exp + 1) != '\0')
		return a_char_token(reg_exp + 1, al);
	if (a_is_valid_char(*reg_exp))
		return a_char_token(reg_exp, al);

	return A_INVALID_RE;
}

unsigned char a_re_translate(char *reg_exp, a_alt_list *al)
{
	unsigned char ret;
	a_token_list *l = a_init_list();
	if (!l)
		return A_MEM_ERR;
	ret = a_add_token_list(l, al);
	if (ret != A_NO_ERR)
		return ret;
	return a_check_cir_flex(reg_exp, al);
}
