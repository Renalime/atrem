#include "matcher.h"
#include "alteration_list.h"
#include "token_list.h"
#include "cc_token_list.h"
#include "globals.h"
#include <limits.h>
#define NULL 0

static char *match_token(const char *s, a_reg_exp_token *t);
static char *match_cc_token(const char *s, a_cc_token *t);
static char *match_token_list(const char *s, a_token_list *tl);
static char *match_cc_token_list(const char *s, a_cc_token_list *tl, const char is_negated);
static char *check_star(const char *s, a_reg_exp_token *t);
static char *check_one(const char *s, a_reg_exp_token *t);
static char *check_plus(const char *s, a_reg_exp_token *t);
static char *check_qm(const char *s, a_reg_exp_token *t);
static char *check_braces(const char *s, a_reg_exp_token *t);

static inline
char *check_char(const char *s, char c)
{
	if (!c)
		return (char *)++s;
	else
		return (*s++ == c ? (char *)s : NULL);
}

static char *check_braces(const char *s, a_reg_exp_token *t)
{
	const char *tmp = s;
	int n = 0;
	int max = (t->a_braces_range.max == -1 ? INT_MAX : t->a_braces_range.max);
	while (n < max) {
		s = check_one(s, t);
		if (s) {
			tmp = s;
			n = n + 1;
		} else {
			break;
		}
	}
	if (n >= t->a_braces_range.min)
		return (char *)tmp;
	return NULL;
}

static char *check_qm(const char *s, a_reg_exp_token *t)
{
	const char *tmp = s;
	s = check_one(s, t);
	return (s ? (char *)s : (char *)tmp);
}

static char *check_plus(const char *s, a_reg_exp_token *t)
{
	const char *tmp = s;
	s = check_star(s, t);
	return (s == tmp ? NULL : (char *)s);
}

static char *check_star(const char *s, a_reg_exp_token *t)
{
	const char *tmp = s;
	while (1) {
		s = check_one(s, t);
		if (s)
			tmp = s;
		else
			break;
	}
	return (char *)tmp;
}

static char *check_one(const char *s, a_reg_exp_token *t)
{
	switch (t->a_re_text_type) {
	case RE_CHAR_TYPE_CHAR:
		s = check_char(s, t->a_text.a_char);
		break;
	case RE_CHAR_TYPE_PARENS:
		s = match(s, t->a_text.a_l);
		a_reset_alt_list(t->a_text.a_l);
		break;
	case RE_CHAR_TYPE_BRACKETS:
		s = match_cc_token_list(s, t->a_text.a_cc_l, t->a_is_negated);
		a_reset_cc_list(t->a_text.a_cc_l);
		break;
	default:
		s = NULL;
	}
	return (char *)s;
}

static char *match_token(const char *s, a_reg_exp_token *t)
{
	switch (t->a_quantifier) {
	case A_DOLLAR:
		s = (!*s ? s : NULL);
		break;
	case A_CHAR:
		s = check_one(s, t);
		break;
	case A_STAR:
		s = check_star(s, t);
		break;
	case A_PLUS:
		s = check_plus(s, t);
		break;
	case A_QUESTION:
		s = check_qm(s, t);
		break;
	case A_BRACES:
		s = check_braces(s, t);
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
			return (is_negated ? NULL : (char *)s + 1);
		s = tmp;
		t = a_cc_get_next_token(tl);
	}
	return (is_negated ? (char *)tmp + 1 : NULL);
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
