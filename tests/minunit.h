#ifndef MINUNIT_H
#define MINUNIT_H

#include <stdio.h>

extern int tests_run;

#define MU_ASSERT(message, condition) \
    do { \
        if (!(condition)) { \
            return message; \
        } \
    } while (0)

#define MU_RUN_TEST(test) \
    do { \
        const char *message = test(); \
        tests_run++; \
        if (message != NULL) { \
            fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, message); \
            return message; \
        } \
    } while (0)

#endif
