#ifndef __MATCHER_H
#define __MATCHER_H

int match(char *regexp, char *text);

int matchhere(char *regexp, char *text);

int matchstar(int c, char *regexp, char *text);

#endif
