#include "atrem.h"
#include "re_translator.h"
#include "globals.h"
#include <stdlib.h>

atrem_regex atrem_parse_regex(char *regex)
{
	unsigned char ret;
	a_alt_list *al;
	if (!regex)
		return NULL;
	al = a_init_alt_list();
	if (!al)
		return NULL;
	ret = a_re_translate(regex, al);
	return (ret != A_NO_ERR ? NULL : al);
}

inline void atrem_dispose_regex(atrem_regex regex)
{
	a_rm_alt_list(regex);
	free(regex);
}
