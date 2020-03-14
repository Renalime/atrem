#include "print_ir.h"
#include "../../src/token_list.h"
#include "../../src/globals.h"
#include <stdint.h>
#include <stdio.h>

static void parse_token_list(a_token_list *l);
static void print_reg_exp_token(a_reg_exp_token *t, uint16_t n_t);
static void print_brackets(a_cc_token_list *l);

unsigned char re_quantifiers_map[10] = {
	'c',
	'*',
	'\\',
	'+',
	'{',
	'$',
	'^',
	'?',
	'(',
	'-',
};
unsigned char re_char_type_map[3] = {
	'{',
	'c',
	'[',
};

static void print_brackets(a_cc_token_list *l)
{
		puts("Brackets inside:\n");
		puts("Brackets outside:\n");
}

static void print_reg_exp_token(a_reg_exp_token *t, uint16_t n_t)
{
	printf("Token #%u\n", n_t);
	printf("Quantifier: %c\n", re_quantifiers_map[t->a_quantifier - 20]);
	if (t->a_quantifier == A_BRACES)
		printf("Min/max: %d/%d\n", t->a_braces_range.min, t->a_braces_range.max);
	printf("Char type: %c\n", re_char_type_map[t->a_re_text_type]);
	if (t->a_re_text_type == RE_CHAR_TYPE_PARENS) {
		puts("Token list inside:\n");
		parse_token_list(t->a_text.a_l);
		puts("Token list outside\n");
	}
	if (t->a_re_text_type == RE_CHAR_TYPE_BRACKETS) {
		print_brackets(t->a_text.a_cc_l);
		printf("Is negated: %c", (t->a_is_negated ? 'y' : 'n'));
	}
}

static void parse_token_list(struct a_token_list *l)
{
	uint16_t n_t = 1;
	a_reg_exp_token *t = a_get_next_token(l);
	while (t) {
		print_reg_exp_token(t, n_t++);
		t = a_get_next_token(l);
	}
}

void print_ir(struct a_alt_list *l)
{
	a_token_list *n;
	if (!l)
		return;
	n = a_get_next_list(l);
	while (n) {
		parse_token_list(n);
		n = a_get_next_list(l);
	}
}
