#include "matcher.h"

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
	text++;
	return matchstar(c, reg_exp, text);
}
