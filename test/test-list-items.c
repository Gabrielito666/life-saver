/**
 * @file
 * @source ./test/test-list-items.c
 * @description Tests for life_saver__listItems
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include "list-items.h"

static int tests_run = 0;
static int tests_passed = 0;

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
 * @brief Test with no inputs and no raw_inputs
 * @return void
 */
static void test_empty(void)
{
	const char *inputs[] = {};
	const char *raw_inputs[] = {};

	ListItemsResult result = life_saver__listItems(inputs, 0, raw_inputs, 0);

	assert(result.ok);
	assert(result.count == 0);
	assert(result.items == NULL);

	life_saver__freeListItems(&result);
	tests_passed++;
	tests_run++;
	printf("  PASS: test_empty\n");
}

/**
 * @brief Test with raw_inputs only
 * @return void
 */
static void test_raw_only(void)
{
	const char *inputs[] = {};
	const char *raw_inputs[] = { "/tmp/test-raw1", "/tmp/test-raw2" };

	create_dir("/tmp/test-raw1");
	create_dir("/tmp/test-raw2");

	ListItemsResult result = life_saver__listItems(inputs, 0, raw_inputs, 2);

	assert(result.ok);
	assert(result.count == 2);
	assert(strcmp(result.items[0], "/tmp/test-raw1") == 0);
	assert(strcmp(result.items[1], "/tmp/test-raw2") == 0);

	life_saver__freeListItems(&result);
	system("rm -rf /tmp/test-raw1 /tmp/test-raw2");
	tests_passed++;
	tests_run++;
	printf("  PASS: test_raw_only\n");
}

/**
 * @brief Test with inputs that have .git directories
 * @return void
 */
static void test_inputs_with_git(void)
{
	create_dir("/tmp/test-input1/.git");
	create_dir("/tmp/test-input1/subdir");
	create_dir("/tmp/test-input2/.git");

	const char *inputs[] = { "/tmp/test-input1", "/tmp/test-input2" };
	const char *raw_inputs[] = {};

	ListItemsResult result = life_saver__listItems(inputs, 2, raw_inputs, 0);

	assert(result.ok);
	assert(result.count == 2);

	life_saver__freeListItems(&result);
	system("rm -rf /tmp/test-input1 /tmp/test-input2");
	tests_passed++;
	tests_run++;
	printf("  PASS: test_inputs_with_git\n");
}

/**
 * @brief Test with inputs without .git directories
 * @return void
 */
static void test_inputs_no_git(void)
{
	create_dir("/tmp/test-no-git/subdir");

	const char *inputs[] = { "/tmp/test-no-git" };
	const char *raw_inputs[] = {};

	ListItemsResult result = life_saver__listItems(inputs, 1, raw_inputs, 0);

	assert(result.ok);
	assert(result.count == 0);

	life_saver__freeListItems(&result);
	system("rm -rf /tmp/test-no-git");
	tests_passed++;
	tests_run++;
	printf("  PASS: test_inputs_no_git\n");
}

/**
 * @brief Test combined inputs and raw_inputs
 * @return void
 */
static void test_combined(void)
{
	create_dir("/tmp/test-combined/.git");
	create_dir("/tmp/test-raw-combined");

	const char *inputs[] = { "/tmp/test-combined" };
	const char *raw_inputs[] = { "/tmp/test-raw-combined" };

	ListItemsResult result = life_saver__listItems(inputs, 1, raw_inputs, 1);

	assert(result.ok);
	assert(result.count == 2);

	life_saver__freeListItems(&result);
	system("rm -rf /tmp/test-combined /tmp/test-raw-combined");
	tests_passed++;
	tests_run++;
	printf("  PASS: test_combined\n");
}

/**
 * @brief Test error on non-existent input directory
 * @return void
 */
static void test_error_input_not_found(void)
{
	const char *inputs[] = { "/tmp/non-existent-dir-12345" };
	const char *raw_inputs[] = {};

	ListItemsResult result = life_saver__listItems(inputs, 1, raw_inputs, 0);

	assert(!result.ok);
	assert(strlen(result.error) > 0);

	life_saver__freeListItems(&result);
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
	const char *inputs[] = {};
	const char *raw_inputs[] = { "/tmp/non-existent-raw-12345" };

	ListItemsResult result = life_saver__listItems(inputs, 0, raw_inputs, 1);

	assert(!result.ok);
	assert(strlen(result.error) > 0);

	life_saver__freeListItems(&result);
	tests_passed++;
	tests_run++;
	printf("  PASS: test_error_raw_not_found\n");
}

/**
 * @brief Test error when input is a file, not directory
 * @return void
 */
static void test_error_input_is_file(void)
{
	system("touch /tmp/test-file.txt");

	const char *inputs[] = { "/tmp/test-file.txt" };
	const char *raw_inputs[] = {};

	ListItemsResult result = life_saver__listItems(inputs, 1, raw_inputs, 0);

	assert(!result.ok);
	assert(strlen(result.error) > 0);

	life_saver__freeListItems(&result);
	system("rm -f /tmp/test-file.txt");
	tests_passed++;
	tests_run++;
	printf("  PASS: test_error_input_is_file\n");
}

/**
 * @brief Test error when raw_input is a file, not directory
 * @return void
 */
static void test_error_raw_is_file(void)
{
	system("touch /tmp/test-raw-file.txt");

	const char *inputs[] = {};
	const char *raw_inputs[] = { "/tmp/test-raw-file.txt" };

	ListItemsResult result = life_saver__listItems(inputs, 0, raw_inputs, 1);

	assert(!result.ok);
	assert(strlen(result.error) > 0);

	life_saver__freeListItems(&result);
	system("rm -f /tmp/test-raw-file.txt");
	tests_passed++;
	tests_run++;
	printf("  PASS: test_error_raw_is_file\n");
}

/**
 * @brief Main test runner
 * @return 0 on success, 1 on failure
 */
int main(void)
{
	setbuf(stdout, NULL);
	printf("Running list-items tests...\n\n");

	test_empty();
	test_raw_only();
	test_inputs_with_git();
	test_inputs_no_git();
	test_combined();
	test_error_input_not_found();
	test_error_raw_not_found();
	test_error_input_is_file();
	test_error_raw_is_file();

	printf("\nResults: %d/%d passed\n", tests_passed, tests_run);

	return (tests_passed == tests_run) ? 0 : 1;
}
