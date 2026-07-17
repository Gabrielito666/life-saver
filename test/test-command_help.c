/**
 * @file
 * @source ./test/test-command_help.c
 * @description Tests for life_saver__commandHelp
 */

#include <stdio.h>
#include <assert.h>
#include "command_help.h"

static int tests_run = 0;
static int tests_passed = 0;

/**
 * @brief Test that commandHelp runs without error
 * @return void
 */
static void test_help_runs(void)
{
	life_saver__commandHelp();
	tests_passed++;
	tests_run++;
	printf("  PASS: test_help_runs\n");
}

/**
 * @brief Main test runner
 * @return 0 on success, 1 on failure
 */
int main(void)
{
	setbuf(stdout, NULL);
	printf("Running command-help tests...\n\n");

	test_help_runs();

	printf("\nResults: %d/%d passed\n", tests_passed, tests_run);

	return (tests_passed == tests_run) ? 0 : 1;
}
