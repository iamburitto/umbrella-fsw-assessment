/**
 * test_framework.h
 *
 * Minimal unit-test helpers for plain C.
 *
 * Usage
 * -----
 *   TEST_ASSERT(condition)           – fail if condition is false
 *   TEST_ASSERT_EQUAL_INT(a, b)      – fail if a != b (integers)
 *   TESTS_BEGIN()                    – call once at the top of main()
 *   TESTS_END()                      – call at the bottom of main();
 *                                      returns EXIT_SUCCESS / EXIT_FAILURE
 *
 * Each failing assertion prints the file, line, and expression, then
 * increments a global failure counter.  The process exits with a
 * non-zero status when any test has failed.
 */

#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdio.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Internal state (defined once per translation unit via the macros)  */
/* ------------------------------------------------------------------ */
static int _tf_failures = 0;
static int _tf_checks   = 0;

/* ------------------------------------------------------------------ */
/* Macros                                                              */
/* ------------------------------------------------------------------ */

#define TESTS_BEGIN() \
    do { _tf_failures = 0; _tf_checks = 0; } while (0)

#define TEST_ASSERT(cond) \
    do { \
        _tf_checks++; \
        if (!(cond)) { \
            fprintf(stderr, "FAIL  %s:%d  %s\n", __FILE__, __LINE__, #cond); \
            _tf_failures++; \
        } \
    } while (0)

#define TEST_ASSERT_EQUAL_INT(expected, actual) \
    do { \
        _tf_checks++; \
        if ((expected) != (actual)) { \
            fprintf(stderr, "FAIL  %s:%d  expected %d but got %d\n", \
                    __FILE__, __LINE__, (int)(expected), (int)(actual)); \
            _tf_failures++; \
        } \
    } while (0)

#define TESTS_END() \
    do { \
        printf("%s: %d check(s), %d failure(s)\n", \
               (_tf_failures == 0 ? "PASS" : "FAIL"), \
               _tf_checks, _tf_failures); \
        return (_tf_failures == 0) ? EXIT_SUCCESS : EXIT_FAILURE; \
    } while (0)

#endif /* TEST_FRAMEWORK_H */
