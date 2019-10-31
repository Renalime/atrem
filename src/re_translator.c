#include "re_translator.h"
#include <stdlib.h>
#include "globals.h"

unsigned char a_check_here(char * reg_exp, a_token_list *l)
{
	if (*reg_exp == '\0')
		return A_NO_ERR;
	if (*reg_exp == '\\')
		return a_escape_token(reg_exp + 1, l);	
	return A_NO_ERR;
}

unsigned char a_escape_token(char *reg_exp, a_token_list *l)
{
	a_char_token text;
	a_reg_exp_token *token;
	if (*reg_exp == '\0')
		return A_ZERO_ESCAPE;
	if (*reg_exp == '\\') {
		text.a_char = '\\';
	} else {
		text.a_char = *reg_exp;
	}
	token = a_gen_token(A_ESCAPE, RE_CHAR_TYPE_CHAR, text, 0);
	if (token == NULL)
		return A_MEM_ERR;
	a_add_token(token, l);
	return a_check_here(reg_exp + 1, l);
}

unsigned char a_re_translate(char *reg_exp)
{
	a_token_list *l = a_init_list();
	if (l == NULL)
		return A_MEM_ERR;
	unsigned char ret = a_check_here(reg_exp, l);
	a_rm_list(l);
	return ret;
}
