#ifndef __ATREM_H
#define __ATREM_H

typedef struct a_alt_list *atrem_regex;

atrem_regex atrem_parse_regex(char *regex);
void atrem_dispose_regex(atrem_regex regex);

#endif
