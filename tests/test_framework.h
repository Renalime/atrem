#ifndef __TEST_FRAMEWORK_H
#define __TEST_FRAMEWORK_H

#include <stdio.h>

#define FAIL() printf("Failed in %s() at line %d\n", __func__, __LINE__)
#define _assert(test) do { if (!(test)) { FAIL(); return 1; } } while(0)
#define _check(test) do { if (!(test)) { FAIL(); } } while(0)
#define _verify(test) do { int r = test(); if (r) { return r; } else { test_count++; } } while(0)

extern unsigned int test_count;

#endif
