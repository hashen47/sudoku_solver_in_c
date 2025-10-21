#include <stdio.h>
#include <stdbool.h>


#ifndef ASSERT_H
#define ASSERT_H


#if ASSERT_ENABLED
#define ASSERT(c, m) do { \
	if (!(c)) { \
		fprintf(stderr, "%s:%d: assertion %s failed: %s", __FILE__, __LINE__, #c, m); \
		exit(1); \
	} \
} while(0)
#else
#define ASSERT(c, m)
#endif


#endif
