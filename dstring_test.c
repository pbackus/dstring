#include "minunit.h"
#include "dstring.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int tests_run = 0;

char *
test_new(void)
{
	char *s = dstr_new(0);
	if (!s) { puts("Fatal error: out of memory"), abort(); }

	mu_assert("FAIL test_new: length is not zero",
		dstr_length(s) == 0
		|| (dstr_delete(s), false) /* cleanup */);

	mu_assert("FAIL test_new: NUL terminator missing",
		*s == '\0'
		|| (dstr_delete(s), false) /* cleanup */);

	dstr_delete(s);
	return NULL;
}

char *
test_assign(void)
{
	char *s = dstr_new(0);
	if (!s) { puts("Fatal error: out of memory"), abort(); }

	const char testval[] = "Lorem ipsum dolor sit amet";
	const size_t testlen = strlen(testval);

	s = dstr_assign(s, testval, testlen);

	mu_assert("FAIL test_assign: incorrect length",
		dstr_length(s) == testlen
		|| (dstr_delete(s), false) /* cleanup */);

	mu_assert("FAIL test_assign: incorrect value",
		strncmp(s, testval, dstr_length(s)) == 0
		|| (dstr_delete(s), false) /* cleanup */);

	mu_assert("FAIL test_assign: NUL terminator missing",
		s[dstr_length(s)] == '\0'
		|| (dstr_delete(s), false) /* cleanup */);

	dstr_delete(s);
	return NULL;
}

char *
test_append(void)
{
	char *s = dstr_new(0);
	if (!s) { puts("Fatal error: out of memory"), abort(); }

	const char testval1[] = "Lorem ipsum ";
	const char testval2[] = "dolor sit amet";

	s = dstr_assign(s, testval1, strlen(testval1));
	s = dstr_append(s, testval2, strlen(testval2));

	mu_assert("FAIL test_append: incorrect length",
		dstr_length(s) == strlen(testval1) + strlen(testval2)
		|| (dstr_delete(s), false) /* cleanup */);

	mu_assert("FAIL test_append: incorrect value",
		strncmp(s, "Lorem ipsum dolor sit amet", dstr_length(s)) == 0
		|| (dstr_delete(s), false) /* cleanup */);

	mu_assert("FAIL test_append: NUL terminator missing",
		s[dstr_length(s)] == '\0'
		|| (dstr_delete(s), false) /* cleanup */);

	dstr_delete(s);
	return NULL;
}

char *
all_tests(void)
{
	mu_run_test(test_new);
	mu_run_test(test_assign);
	mu_run_test(test_append);

	return NULL;
}

int
main (int argc, char **argv)
{
	char *result = all_tests();

	if (result) puts(result);
	else puts("All tests passed");

	printf("Tests run: %d\n", tests_run);

	return result != NULL;
}
