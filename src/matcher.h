#ifndef __MATCHER_H
#define __MATCHER_H

typedef struct a_alt_list a_alt_list;

char *match(const char *s, a_alt_list *al);
/*
int match(char *regexp, char *text);

int matchhere(char *regexp, char *text);

int matchstar(int c, char *regexp, char *text);

int match_plus(int c, char *reg_exp, char *text);

int match_escape(char *reg_exp, char *text);
*/

#endif
