/**
 * @file
 * @source ./test/test-pack.c
 * @description Tests for life_saver__pack
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "pack.h"

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
 * @brief Helper to extract tar and check contents
 * @param tar_path Path to tar.gz file
 * @param dir Directory to extract to
 * @return void
 */
static void extract_tar(const char *tar_path, const char *dir)
{
	char cmd[512];
	mkdir(dir, 0755);
	snprintf(cmd, sizeof(cmd), "tar xzf %s -C %s", tar_path, dir);
	system(cmd);
}

/**
 * @brief Test pack with no paths (should create empty archive)
 * @return void
 */
static void test_empty(void)
{
	const char *paths[] = {};
	const char *output = "/tmp/test-empty.tar.gz";

	PackResult result = life_saver__pack(paths, 0, output);

	assert(result.ok);
	assert(file_exists(output));

	unlink(output);
	tests_passed++;
	tests_run++;
	printf("  PASS: test_empty\n");
}

/**
 * @brief Test pack with a .git directory
 * @return void
 */
static void test_single_git(void)
{
	const char *paths[] = { "./fixtures/repo1/.git" };
	const char *output = "/tmp/test-single-git.tar.gz";

	PackResult result = life_saver__pack(paths, 1, output);

	assert(result.ok);
	assert(file_exists(output));

	/* Extract and verify */
	extract_tar(output, "/tmp/test-single-git-extract");
	assert(file_exists("/tmp/test-single-git-extract/dot_fixtures/repo1/.git"));

	/* Cleanup */
	system("rm -rf /tmp/test-single-git-extract");
	unlink(output);
	tests_passed++;
	tests_run++;
	printf("  PASS: test_single_git\n");
}

/**
 * @brief Test pack with multiple .git directories
 * @return void
 */
static void test_multiple_git(void)
{
	const char *paths[] = {
		"./fixtures/repo1/.git",
		"./fixtures/repo2/.git",
		"./fixtures/deep/nested/.git"
	};
	const char *output = "/tmp/test-multiple-git.tar.gz";

	PackResult result = life_saver__pack(paths, 3, output);

	assert(result.ok);
	assert(file_exists(output));

	/* Extract and verify */
	extract_tar(output, "/tmp/test-multiple-git-extract");
	assert(file_exists("/tmp/test-multiple-git-extract/dot_fixtures/repo1/.git"));
	assert(file_exists("/tmp/test-multiple-git-extract/dot_fixtures/repo2/.git"));
	assert(file_exists("/tmp/test-multiple-git-extract/dot_fixtures/deep/nested/.git"));

	/* Cleanup */
	system("rm -rf /tmp/test-multiple-git-extract");
	unlink(output);
	tests_passed++;
	tests_run++;
	printf("  PASS: test_multiple_git\n");
}

/**
 * @brief Test pack with a raw directory
 * @return void
 */
static void test_raw_directory(void)
{
	const char *paths[] = { "./fixtures/raw-dir" };
	const char *output = "/tmp/test-raw.tar.gz";

	PackResult result = life_saver__pack(paths, 1, output);

	assert(result.ok);
	assert(file_exists(output));

	/* Extract and verify */
	extract_tar(output, "/tmp/test-raw-extract");
	assert(file_exists("/tmp/test-raw-extract/dot_fixtures/raw-dir/data.txt"));

	/* Cleanup */
	system("rm -rf /tmp/test-raw-extract");
	unlink(output);
	tests_passed++;
	tests_run++;
	printf("  PASS: test_raw_directory\n");
}

/**
 * @brief Test pack with mixed git and raw
 * @return void
 */
static void test_mixed(void)
{
	const char *paths[] = {
		"./fixtures/repo1/.git",
		"./fixtures/raw-dir"
	};
	const char *output = "/tmp/test-mixed.tar.gz";

	PackResult result = life_saver__pack(paths, 2, output);

	assert(result.ok);
	assert(file_exists(output));

	/* Extract and verify */
	extract_tar(output, "/tmp/test-mixed-extract");
	assert(file_exists("/tmp/test-mixed-extract/dot_fixtures/repo1/.git"));
	assert(file_exists("/tmp/test-mixed-extract/dot_fixtures/raw-dir/data.txt"));

	/* Cleanup */
	system("rm -rf /tmp/test-mixed-extract");
	unlink(output);
	tests_passed++;
	tests_run++;
	printf("  PASS: test_mixed\n");
}

/**
 * @brief Test pack with path without ./ prefix (should behave like ./*)
 * @return void
 */
static void test_no_dot_prefix(void)
{
	const char *paths[] = { "fixtures/repo1/.git" };
	const char *output = "/tmp/test-no-dot.tar.gz";

	PackResult result = life_saver__pack(paths, 1, output);

	assert(result.ok);
	assert(file_exists(output));

	/* Extract and verify — same as with ./ prefix */
	extract_tar(output, "/tmp/test-no-dot-extract");
	assert(file_exists("/tmp/test-no-dot-extract/dot_fixtures/repo1/.git"));

	/* Cleanup */
	system("rm -rf /tmp/test-no-dot-extract");
	unlink(output);
	tests_passed++;
	tests_run++;
	printf("  PASS: test_no_dot_prefix\n");
}

/**
 * @brief Test pack with multiple ../../ (dotdot_dotdot_dotdot_)
 * @return void
 */
static void test_multiple_dotdot(void)
{
	/* Create structure so that ../outer/deep/.git resolves */
	system("mkdir -p /tmp/test-pack-workspace/inner");
	system("mkdir -p /tmp/test-pack-workspace/outer/deep/.git");
	system("echo deep > /tmp/test-pack-workspace/outer/deep/info.txt");

	/* From inner/, ../outer/deep/.git resolves to outer/deep/.git */
	const char *paths[] = { "/tmp/test-pack-workspace/inner/../outer/deep/.git" };
	const char *output = "/tmp/test-multi-dotdot.tar.gz";

	PackResult result = life_saver__pack(paths, 1, output);

	assert(result.ok);
	assert(file_exists(output));

	/* Extract and verify */
	extract_tar(output, "/tmp/test-multi-dotdot-extract");
	assert(file_exists("/tmp/test-multi-dotdot-extract/ROOT_tmp/test-pack-workspace/inner/dotdot_outer/deep/.git"));

	/* Cleanup */
	system("rm -rf /tmp/test-pack-workspace /tmp/test-multi-dotdot-extract");
	unlink(output);
	tests_passed++;
	tests_run++;
	printf("  PASS: test_multiple_dotdot\n");
}

/**
 * @brief Test pack with non-existent path
 * @return void
 */
static void test_error_not_found(void)
{
	const char *paths[] = { "./fixtures/non-existent" };
	const char *output = "/tmp/test-not-found.tar.gz";

	PackResult result = life_saver__pack(paths, 1, output);

	assert(!result.ok);
	assert(strlen(result.error) > 0);

	unlink(output);
	tests_passed++;
	tests_run++;
	printf("  PASS: test_error_not_found\n");
}

/**
 * @brief Main test runner
 * @return 0 on success, 1 on failure
 */
int main(void)
{
	setbuf(stdout, NULL);
	printf("Running pack tests...\n\n");

	test_empty();
	test_single_git();
	test_multiple_git();
	test_raw_directory();
	test_mixed();
	test_no_dot_prefix();
	test_multiple_dotdot();
	test_error_not_found();

	printf("\nResults: %d/%d passed\n", tests_passed, tests_run);

	return (tests_passed == tests_run) ? 0 : 1;
}
