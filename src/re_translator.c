#include "re_translator.h"
#include <stdlib.h>
#include "globals.h"
#include "bracket_parser.h"

/*
unsigned char a_check_here(char *reg_exp, a_token_list *l)
{
	if (*reg_exp == '\0')
		return A_NO_ERR;
	if (*reg_exp == '\\')
		return a_escape_token(reg_exp + 1, l);
	if (*reg_exp == '[')
		return a_parse_brackets(reg_exp + 1, l);
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

unsigned char a_parse_brackets(char *reg_exp, a_token_list *l)
{
	a_re_text text;
	a_reg_exp_token *token;
	unsigned char is_quantifier;
	a_cc_token_list *list = a_init_cc_token_list();
	if (list != NULL)
		return A_MEM_ERR;
	unsigned char is_negated = 0;
	a_tuple_ret ret;
	if (list == NULL)
		return A_MEM_ERR;
	if (*reg_exp == '^') {
		is_negated = 1;
		reg_exp++;
	}
	ret = a_parse_brackets_init(reg_exp, list);
	if (ret.ret != A_NO_ERR)
		return ret.ret;
	reg_exp = ret.reg_exp;
	reg_exp++;
	is_quantifier = a_is_quantifier(*reg_exp);
	text.a_cc_l = list;
	token = a_gen_token(is_quantifier, RE_CHAR_TYPE_BRACKETS, text, is_negated);
	if (token == NULL)
		return A_MEM_ERR;
	if (is_quantifier == A_BRACES)
		return a_parse_braces(reg_exp + 1, l, token);
	if (a_add_token(token, l) != A_NO_ERR)
		return A_MEM_ERR;
	return (is_quantifier == A_CHAR) ? a_check_here(reg_exp, l) : a_check_here(reg_exp + 1, l);
}

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
*/

unsigned char a_escape_token(char *reg_exp, a_alt_list *al)
{
	if (*reg_exp == '\0')
		return A_INVALID_RE;
/*	if (*reg_exp == '\\')
		text.a_char = '\\';
	else
		text.a_char = *reg_exp;
*/
/*	token = a_gen_token(A_ESCAPE, RE_CHAR_TYPE_CHAR, text, 0);
	if (token == NULL)
		return A_MEM_ERR;
	a_add_token(token, l);
	return a_check_here(reg_exp + 1, l);
*/
	return a_char_token(reg_exp, al);
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


unsigned char a_parse_braces(char *reg_exp, a_alt_list *al, a_reg_exp_token *t)
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
	if (*reg_exp != '}') {
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
	} else {
		ranges.min = ranges.max = range;
	}
	t->a_braces_range = ranges;
	a_add_token(t, a_get_last_list(al));
	return a_check_here(reg_exp + 1, al);
}

unsigned char a_char_token(char *reg_exp, a_alt_list *al)
{
	a_re_text text;
	a_reg_exp_token *token;
	unsigned char is_quantifier = a_is_quantifier(*(reg_exp + 1));
	text.a_char = *reg_exp;
	token = a_gen_token(is_quantifier, RE_CHAR_TYPE_CHAR, text, 0);
	if (token == NULL)
		return A_MEM_ERR;
	if (is_quantifier == A_BRACES)
		return a_parse_braces(reg_exp + 1, al, token);
	a_add_token(token, a_get_last_list(al));
	return (is_quantifier == A_CHAR) ? a_check_here(reg_exp + 1, al) : a_check_here(reg_exp + 2, al);

}

unsigned char a_is_valid_char(char c)
{
	return (c != '^' && c != '$' && c != ']' && c != '+' && c != '*') ? 1 : 0;
}


unsigned char a_generic_token(char *reg_exp, unsigned char type, a_alt_list *al)
{
	a_re_text text;
	a_reg_exp_token *token;
	if (type == A_CHAR)
		text.a_char = *reg_exp;
	else
		text.a_char = 0;
	token = a_gen_token(type, RE_CHAR_TYPE_CHAR, text, 0);
	if (token == NULL)
		return A_MEM_ERR;
	a_add_token(token, a_get_last_list(al));
	return a_check_here(reg_exp + 1, al);
}

unsigned char a_check_cir_flex(char *reg_exp, a_alt_list *al)
{
	if (*reg_exp == '^')
		return a_generic_token(reg_exp, A_CIR_FLEX, al);
	else
		return a_check_here(reg_exp, al);
}

unsigned char a_check_here(char *reg_exp, a_alt_list *al)
{
	if (*reg_exp == '\0')
		return A_NO_ERR;
	if (*reg_exp == ')')
		return A_NESTED;
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
	if (l == NULL)
		return A_MEM_ERR;
	ret = a_check_cir_flex(reg_exp, al);
	return ret;
}
