/**
 * @file
 * @source ./test/test-command-saver.c
 * @description Tests for life_saver__commandSaver
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "command-saver.h"

static int tests_run = 0;
static int tests_passed = 0;

/**
 * @brief Helper to check if file exists
 * @param path Path to check
 * @return 1 if exists, 0 otherwise
 */
static int file_exists(const char *path)
{
	struct stat st;
	return (stat(path, &st) == 0);
}

/**
 * @brief Helper to create directory structure
 * @param path Directory path to create
 * @return void
 */
static void create_dir(const char *path)
{
	char cmd[256];
	snprintf(cmd, sizeof(cmd), "mkdir -p %s", path);
	system(cmd);
}

/**
 * @brief Helper to create a LifeSaverArgs with defaults
 * @return LifeSaverArgs with all fields zeroed
 */
static LifeSaverArgs make_args(void)
{
	LifeSaverArgs args;
	args.input_count = 0;
	args.raw_count = 0;
	args.output = NULL;
	args.help = 0;
	return (args);
}

/**
 * @brief Test with no inputs (should error: no items found)
 * @return void
 */
static void test_empty(void)
{
	LifeSaverArgs args = make_args();

	SaverResult result = life_saver__commandSaver(args);

	assert(!result.ok);
	assert(strlen(result.error) > 0);

	tests_passed++;
	tests_run++;
	printf("  PASS: test_empty\n");
}

/**
 * @brief Test with input containing .git directory
 * @return void
 */
static void test_with_git_input(void)
{
	create_dir("/tmp/test-cs-git/.git");
	create_dir("/tmp/test-cs-git/src");

	LifeSaverArgs args = make_args();
	args.inputs[0] = "/tmp/test-cs-git";
	args.input_count = 1;
	args.output = "/tmp/test-cs-git-output.tar.gz";

	SaverResult result = life_saver__commandSaver(args);

	assert(result.ok);
	assert(file_exists("/tmp/test-cs-git-output.tar.gz"));

	/* Cleanup */
	unlink("/tmp/test-cs-git-output.tar.gz");
	system("rm -rf /tmp/test-cs-git");
	tests_passed++;
	tests_run++;
	printf("  PASS: test_with_git_input\n");
}

/**
 * @brief Test with raw input directory
 * @return void
 */
static void test_with_raw_input(void)
{
	create_dir("/tmp/test-cs-raw/data");

	LifeSaverArgs args = make_args();
	args.raw_inputs[0] = "/tmp/test-cs-raw";
	args.raw_count = 1;
	args.output = "/tmp/test-cs-raw-output.tar.gz";

	SaverResult result = life_saver__commandSaver(args);

	assert(result.ok);
	assert(file_exists("/tmp/test-cs-raw-output.tar.gz"));

	/* Cleanup */
	unlink("/tmp/test-cs-raw-output.tar.gz");
	system("rm -rf /tmp/test-cs-raw");
	tests_passed++;
	tests_run++;
	printf("  PASS: test_with_raw_input\n");
}

/**
 * @brief Test with combined inputs and raw_inputs
 * @return void
 */
static void test_combined(void)
{
	create_dir("/tmp/test-cs-combined/.git");
	create_dir("/tmp/test-cs-combined-raw/content");

	LifeSaverArgs args = make_args();
	args.inputs[0] = "/tmp/test-cs-combined";
	args.input_count = 1;
	args.raw_inputs[0] = "/tmp/test-cs-combined-raw";
	args.raw_count = 1;
	args.output = "/tmp/test-cs-combined-output.tar.gz";

	SaverResult result = life_saver__commandSaver(args);

	assert(result.ok);
	assert(file_exists("/tmp/test-cs-combined-output.tar.gz"));

	/* Cleanup */
	unlink("/tmp/test-cs-combined-output.tar.gz");
	system("rm -rf /tmp/test-cs-combined /tmp/test-cs-combined-raw");
	tests_passed++;
	tests_run++;
	printf("  PASS: test_combined\n");
}

/**
 * @brief Test default output path when no -o specified
 * @return void
 */
static void test_default_output(void)
{
	create_dir("/tmp/test-cs-default/.git");

	LifeSaverArgs args = make_args();
	args.inputs[0] = "/tmp/test-cs-default";
	args.input_count = 1;
	/* No output specified */

	SaverResult result = life_saver__commandSaver(args);

	assert(result.ok);
	/* Default output includes timestamp: life-saver-backup-YYYY-MM-DD-HHMMSS.tar.gz */
	assert(system("ls life-saver-backup-*.tar.gz > /dev/null 2>&1") == 0);

	/* Cleanup */
	system("rm -f life-saver-backup-*.tar.gz");
	system("rm -rf /tmp/test-cs-default");
	tests_passed++;
	tests_run++;
	printf("  PASS: test_default_output\n");
}

/**
 * @brief Test error on non-existent input directory
 * @return void
 */
static void test_error_input_not_found(void)
{
	LifeSaverArgs args = make_args();
	args.inputs[0] = "/tmp/non-existent-dir-cs-12345";
	args.input_count = 1;

	SaverResult result = life_saver__commandSaver(args);

	assert(!result.ok);
	assert(strlen(result.error) > 0);

	tests_passed++;
	tests_run++;
	printf("  PASS: test_error_input_not_found\n");
}

/**
 * @brief Test error on non-existent raw_input directory
 * @return void
 */
static void test_error_raw_not_found(void)
{
	LifeSaverArgs args = make_args();
	args.raw_inputs[0] = "/tmp/non-existent-raw-cs-12345";
	args.raw_count = 1;

	SaverResult result = life_saver__commandSaver(args);

	assert(!result.ok);
	assert(strlen(result.error) > 0);

	tests_passed++;
	tests_run++;
	printf("  PASS: test_error_raw_not_found\n");
}

/**
 * @brief Main test runner
 * @return 0 on success, 1 on failure
 */
int main(void)
{
	setbuf(stdout, NULL);
	printf("Running command-saver tests...\n\n");

	test_empty();
	test_with_git_input();
	test_with_raw_input();
	test_combined();
	test_default_output();
	test_error_input_not_found();
	test_error_raw_not_found();

	printf("\nResults: %d/%d passed\n", tests_passed, tests_run);

	return (tests_passed == tests_run) ? 0 : 1;
}
