/**
 * @file
 * @source ./test/test-parse-args.c
 * @description Tests for life_saver__parseArgs
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "parse-args.h"

static int tests_run = 0;
static int tests_passed = 0;

/**
 * @brief Test single -i flag
 * @return void
 */
static void test_single_input(void)
{
	char *argv[] = { "life-saver", "-i", "./src" };
	int argc = 3;

	ParseResult result = life_saver__parseArgs(argc, argv);

	assert(result.ok);
	assert(result.args.input_count == 1);
	assert(strcmp(result.args.inputs[0], "./src") == 0);
	tests_passed++;
	tests_run++;
	printf("  PASS: test_single_input\n");
}

/**
 * @brief Test multiple -i flags
 * @return void
 */
static void test_multiple_inputs(void)
{
	char *argv[] = { "life-saver", "-i", "./src", "-i", "./debian", "-i", "./test" };
	int argc = 7;

	ParseResult result = life_saver__parseArgs(argc, argv);

	assert(result.ok);
	assert(result.args.input_count == 3);
	assert(strcmp(result.args.inputs[0], "./src") == 0);
	assert(strcmp(result.args.inputs[1], "./debian") == 0);
	assert(strcmp(result.args.inputs[2], "./test") == 0);
	tests_passed++;
	tests_run++;
	printf("  PASS: test_multiple_inputs\n");
}

/**
 * @brief Test -o flag
 * @return void
 */
static void test_output(void)
{
	char *argv[] = { "life-saver", "-i", "./src", "-o", "backup.tar.gz" };
	int argc = 5;

	ParseResult result = life_saver__parseArgs(argc, argv);

	assert(result.ok);
	assert(strcmp(result.args.output, "backup.tar.gz") == 0);
	tests_passed++;
	tests_run++;
	printf("  PASS: test_output\n");
}

/**
 * @brief Test single -r flag
 * @return void
 */
static void test_raw(void)
{
	char *argv[] = { "life-saver", "-i", "./src", "-r", "./raw1" };
	int argc = 5;

	ParseResult result = life_saver__parseArgs(argc, argv);

	assert(result.ok);
	assert(result.args.raw_count == 1);
	assert(strcmp(result.args.raw_inputs[0], "./raw1") == 0);
	tests_passed++;
	tests_run++;
	printf("  PASS: test_raw\n");
}

/**
 * @brief Test multiple -r flags
 * @return void
 */
static void test_multiple_raw(void)
{
	char *argv[] = { "life-saver", "-i", "./src", "-r", "./raw1", "-r", "./raw2" };
	int argc = 7;

	ParseResult result = life_saver__parseArgs(argc, argv);

	assert(result.ok);
	assert(result.args.raw_count == 2);
	assert(strcmp(result.args.raw_inputs[0], "./raw1") == 0);
	assert(strcmp(result.args.raw_inputs[1], "./raw2") == 0);
	tests_passed++;
	tests_run++;
	printf("  PASS: test_multiple_raw\n");
}

/**
 * @brief Test -h flag
 * @return void
 */
static void test_help(void)
{
	char *argv[] = { "life-saver", "-h" };
	int argc = 2;

	ParseResult result = life_saver__parseArgs(argc, argv);

	assert(result.ok);
	assert(result.args.help == 1);
	tests_passed++;
	tests_run++;
	printf("  PASS: test_help\n");
}

/**
 * @brief Test long option --input
 * @return void
 */
static void test_long_input(void)
{
	char *argv[] = { "life-saver", "--input", "./src" };
	int argc = 3;

	ParseResult result = life_saver__parseArgs(argc, argv);

	assert(result.ok);
	assert(result.args.input_count == 1);
	assert(strcmp(result.args.inputs[0], "./src") == 0);
	tests_passed++;
	tests_run++;
	printf("  PASS: test_long_input\n");
}

/**
 * @brief Test long option --output
 * @return void
 */
static void test_long_output(void)
{
	char *argv[] = { "life-saver", "-i", "./src", "--output", "backup.tar.gz" };
	int argc = 5;

	ParseResult result = life_saver__parseArgs(argc, argv);

	assert(result.ok);
	assert(strcmp(result.args.output, "backup.tar.gz") == 0);
	tests_passed++;
	tests_run++;
	printf("  PASS: test_long_output\n");
}

/**
 * @brief Test long option --raw-input
 * @return void
 */
static void test_long_raw(void)
{
	char *argv[] = { "life-saver", "-i", "./src", "--raw-input", "./raw1" };
	int argc = 5;

	ParseResult result = life_saver__parseArgs(argc, argv);

	assert(result.ok);
	assert(result.args.raw_count == 1);
	assert(strcmp(result.args.raw_inputs[0], "./raw1") == 0);
	tests_passed++;
	tests_run++;
	printf("  PASS: test_long_raw\n");
}

/**
 * @brief Test long option --help
 * @return void
 */
static void test_long_help(void)
{
	char *argv[] = { "life-saver", "--help" };
	int argc = 2;

	ParseResult result = life_saver__parseArgs(argc, argv);

	assert(result.ok);
	assert(result.args.help == 1);
	tests_passed++;
	tests_run++;
	printf("  PASS: test_long_help\n");
}

/**
 * @brief Test no arguments defaults
 * @return void
 */
static void test_defaults(void)
{
	char *argv[] = { "life-saver" };
	int argc = 1;

	ParseResult result = life_saver__parseArgs(argc, argv);

	assert(result.ok);
	assert(result.args.input_count == 0);
	assert(result.args.raw_count == 0);
	assert(result.args.output == NULL);
	assert(result.args.help == 0);
	tests_passed++;
	tests_run++;
	printf("  PASS: test_defaults\n");
}

/**
 * @brief Test combined flags
 * @return void
 */
static void test_combined(void)
{
	char *argv[] = { "life-saver", "-i", "./src", "-i", "./debian", "-r", "./raw1", "-o", "out.tar.gz" };
	int argc = 9;

	ParseResult result = life_saver__parseArgs(argc, argv);

	assert(result.ok);
	assert(result.args.input_count == 2);
	assert(strcmp(result.args.inputs[0], "./src") == 0);
	assert(strcmp(result.args.inputs[1], "./debian") == 0);
	assert(result.args.raw_count == 1);
	assert(strcmp(result.args.raw_inputs[0], "./raw1") == 0);
	assert(strcmp(result.args.output, "out.tar.gz") == 0);
	tests_passed++;
	tests_run++;
	printf("  PASS: test_combined\n");
}

/**
 * @brief Test error on missing argument for -i
 * @return void
 */
static void test_error_missing_input_arg(void)
{
	char *argv[] = { "life-saver", "-i" };
	int argc = 2;

	ParseResult result = life_saver__parseArgs(argc, argv);

	assert(!result.ok);
	assert(strlen(result.error) > 0);
	tests_passed++;
	tests_run++;
	printf("  PASS: test_error_missing_input_arg\n");
}

/**
 * @brief Test error on unknown option
 * @return void
 */
static void test_error_unknown_option(void)
{
	char *argv[] = { "life-saver", "-x" };
	int argc = 2;

	ParseResult result = life_saver__parseArgs(argc, argv);

	assert(!result.ok);
	assert(strlen(result.error) > 0);
	tests_passed++;
	tests_run++;
	printf("  PASS: test_error_unknown_option\n");
}

/**
 * @brief Main test runner
 * @return 0 on success, 1 on failure
 */
int main(void)
{
	setbuf(stdout, NULL);
	printf("Running parse-args tests...\n\n");

	test_single_input();
	test_multiple_inputs();
	test_output();
	test_raw();
	test_multiple_raw();
	test_help();
	test_long_input();
	test_long_output();
	test_long_raw();
	test_long_help();
	test_defaults();
	test_combined();
	test_error_missing_input_arg();
	test_error_unknown_option();

	printf("\nResults: %d/%d passed\n", tests_passed, tests_run);

	return (tests_passed == tests_run) ? 0 : 1;
}
