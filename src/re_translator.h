#ifndef __COMPILATION_H
#define __COMPILATION_H

#include "token_list.h"

unsigned char a_re_translate(char *reg_exp);
unsigned char a_check_here(char *reg_exp, a_token_list *l);
unsigned char a_escape_token(char *reg_exp, a_token_list *l);


#endif
