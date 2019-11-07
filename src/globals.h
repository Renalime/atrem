#ifndef __GLOBALS_H
#define __GLOBALS_H

#define RE_CHAR_TYPE_PARENS 0
#define RE_CHAR_TYPE_CHAR 1
#define RE_CHAR_TYPE_BRACKETS 2

enum a_re_quantifiers{A_CHAR, A_STAR, A_ESCAPE, A_PLUS, A_BRACES, A_DOLLAR, A_CIR_FLEX, A_QUESTION, A_NESTED};
enum a_errors{A_NO_ERR, A_INVALID_RE, A_MEM_ERR};

#endif
