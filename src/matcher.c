#include "matcher.h"
#include "alteration_list.h"
#include "token_list.h"
#include "cc_token_list.h"
#include "globals.h"
#include <stdio.h>
//#define NULL 0

static char *match_token(const char *s, a_reg_exp_token *t);
static char *match_cc_token(const char *s, a_cc_token *t);
static char *match_token_list(const char *s, a_token_list *tl);
static char *match_cc_token_list(const char *s, a_cc_token_list *tl, const char is_negated);
static char *check_star(const char *s, a_reg_exp_token *t);

static inline
char *check_char(const char *s, char c)
{
	if (!c)
		return (char *)++s;
	else
		return (*s++ == c ? (char *)s : NULL);
}

static char *check_star(const char *s, a_reg_exp_token *t)
{
	const char *tmp = NULL;
	switch (t->a_re_text_type) {
	case RE_CHAR_TYPE_CHAR:
		while (s) {
			tmp = s;
			s = check_char(s, t->a_text.a_char);
		}
		break;
	case RE_CHAR_TYPE_PARENS:
		while (s) {
			tmp = s;
			s = match(s, t->a_text.a_l);
			a_reset_alt_list(t->a_text.a_l);
		}
		break;
	case RE_CHAR_TYPE_BRACKETS:
		while (s) {
			tmp = s;
			s = match_cc_token_list(s, t->a_text.a_cc_l, t->a_is_negated);
			a_reset_cc_list(t->a_text.a_cc_l);
		}
		break;
	default:
		s = tmp;
	}
	return (char *)tmp;
}

static char *match_token(const char *s, a_reg_exp_token *t)
{
	switch (t->a_quantifier) {
	case A_DOLLAR:
		s = (!*s ? s : NULL);
		break;
	case A_CHAR:
		s = check_char(s, t->a_text.a_char);
		break;
	case A_STAR:
		s = check_star(s, t);
		break;
	default:
		s = NULL;
	}
	return (char *)s;
}

static char *match_cc_token(const char *s, a_cc_token *t)
{
	switch (t->a_char_type) {
	case A_RANGE:
		if (!(*s >= t->a_char.a_range.min && *s <= t->a_char.a_range.max))
			s = NULL;
		break;
	case A_CHAR:
		if (t->a_char.a_char != *s)
			s = NULL;
		break;
	default:
		s = NULL;
	}
	return (char *)s;
}

static char *match_cc_token_list(const char *s, a_cc_token_list *tl, char is_negated)
{
	a_cc_token *t;
	const char *tmp = s;
	if (!*s)
		return NULL;
	t = a_cc_get_next_token(tl);
	while (t) {
		s = match_cc_token(s, t);
		if (s)
			return (char *)s + 1;
		s = tmp;
		t = a_cc_get_next_token(tl);
	}
	return NULL;
}

static char *match_token_list(const char *s, a_token_list *tl)
{
	a_reg_exp_token *t;
	const char *tmp = s;
	char is_cf = 0;
	t = a_get_next_token(tl);
	if (t && (t->a_quantifier == A_CIR_FLEX)) {
		t = a_get_next_token(tl);
		is_cf = 1;
	}
	while (t) {
		/*
		if (!*s)
			return (t->a_quantifier == A_DOLLAR ? (char *)s : NULL);
			*/
		s = match_token(s, t);
		if (!s) {
			if (!*tmp || is_cf)
				return NULL;
			s = ++tmp;
			a_reset_token_list(tl);
		}
		t = a_get_next_token(tl);
	}
	return (char *)s;
}

char *match(const char *s, a_alt_list *al)
{
	a_token_list *tl;
	const char *tmp;
	tl = a_get_next_list(al);
	while (tl) {
		tmp = match_token_list(s, tl);
		if (tmp)
			return (char *)tmp;
		tl = a_get_next_list(al);
	}
	return NULL;
}

/*
int match(char *regexp, char *text)
{
	if (regexp[0] == '^')
		return matchhere(regexp+1, text);
	do {
		if (matchhere(regexp, text))
			return 1;
	} while (*text++ != '\0');
	return 0;
}

int matchhere(char *regexp, char *text)
{
	if (regexp[0] == '\0')
		return 1;
	if (regexp[0] == '\\')
		return match_escape(regexp + 1, text);
	if (regexp[1] == '?')
		return (*text == regexp[0]) ? matchhere(regexp + 2, text + 1) : matchhere(regexp + 2, text);
	if (regexp[1] == '*')
		return matchstar(regexp[0], regexp + 2, text);
	if (regexp[1] == '+')
		return match_plus(regexp[0], regexp + 2, text);
	if (regexp[0] == '$' && regexp[1] == '\0')
		return *text=='\0';
	if (*text != '\0' && (regexp[0] == '.' || regexp[0] == *text))
		return matchhere(regexp + 1, text + 1);
	return 0;
}

int matchstar(int c, char *regexp, char *text)
{
	if (regexp[0] == '?') {
		do {
			if (matchhere(regexp + 1, text))
				return 1;
		} while (*text != '\0' && (*text++ == c || c == '.'));
	} else {
		char *t;
		for (t = text; *t != '\0' && (*t == c || c == '.'); t++);
		do {
			if (matchhere(regexp, t))
				return 1;
		} while (t-- > text);
	}
	return 0;
}

int match_plus(int c, char *reg_exp, char *text)
{
	if (*text == '\0' || (c != '.' && *text != c))
		return 0;
	return matchstar(c, reg_exp, ++text);
}

int match_escape(char *reg_exp, char *text)
{
	if (reg_exp[0] == '\0')
		return 0;
	if (*text != '\0') {
		if (reg_exp[0] == '\\')
			return (*text == '\\') ? matchhere(reg_exp + 1, text + 1) : 0;
		else
			return matchhere(reg_exp, text);
	}
	return 0;
}
*/
