#ifndef TEST_HELPER_H
#define TEST_HELPER_H

int tests_run = 0;
int tests_failed = 0;
int assertions_made = 0;

#ifndef TEST_HEADER
#define TEST_HEADER(x) fprintf(stderr, "[TESTING] " x "\n")
#endif

#endif
