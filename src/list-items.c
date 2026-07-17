/**
 * @file
 * @source ./src/list-items.c
 * @description List items from inputs (find .git) and raw_inputs
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "list-items.h"

#define INITIAL_CAPACITY 16
#define CMD_SIZE 512
#define LINE_SIZE 256

/**
 * @brief Create an error result
 * @param msg Error message
 * @return ListItemsResult with ok=0 and error set
 */
static ListItemsResult make_error(const char *msg)
{
	ListItemsResult result;
	result.items = NULL;
	result.count = 0;
	result.capacity = 0;
	result.ok = 0;
	strncpy(result.error, msg, LIFE_SAVER__LIST_ITEMS_ERROR_SIZE - 1);
	result.error[LIFE_SAVER__LIST_ITEMS_ERROR_SIZE - 1] = '\0';
	return (result);
}

/**
 * @brief Check if path exists and is a directory
 * @param path Path to check
 * @return 1 if directory exists, 0 otherwise
 */
static int is_directory(const char *path)
{
	struct stat st;

	if (stat(path, &st) != 0)
		return (0);

	return (S_ISDIR(st.st_mode));
}

/**
 * @brief Ensure capacity for items array
 * @param result Pointer to ListItemsResult
 * @param needed Minimum capacity needed
 * @return void
 */
static void ensure_capacity(ListItemsResult *result, int needed)
{
	if (needed <= result->capacity)
		return;

	int new_capacity = (result->capacity == 0) ? INITIAL_CAPACITY : result->capacity * 2;
	while (new_capacity < needed)
		new_capacity *= 2;

	result->items = realloc(result->items, new_capacity * sizeof(char *));
	result->capacity = new_capacity;
}

/**
 * @brief Add an item to the result
 * @param result Pointer to ListItemsResult
 * @param item String to add (will be strdup'd)
 * @return void
 */
static void add_item(ListItemsResult *result, const char *item)
{
	ensure_capacity(result, result->count + 1);
	result->items[result->count++] = strdup(item);
}

/**
 * @brief Find .git directories in input path
 * @param result Pointer to ListItemsResult to add items to
 * @param input Directory path to scan
 * @return ListItemsResult with error if find fails, or items added
 */
static ListItemsResult find_git_dirs(ListItemsResult *result, const char *input)
{
	char cmd[CMD_SIZE];
	snprintf(cmd, sizeof(cmd), "find %s -type d -name .git -print", input);

	FILE *fp = popen(cmd, "r");
	if (!fp)
		return (make_error("failed to execute find command"));

	char line[LINE_SIZE];
	while (fgets(line, sizeof(line), fp))
	{
		line[strcspn(line, "\n")] = '\0';
		if (strlen(line) > 0)
			add_item(result, line);
	}

	int status = pclose(fp);
	if (status != 0)
		return (make_error("find command failed"));

	/* Return empty result to signal success (caller checks ok) */
	ListItemsResult ok_result;
	ok_result.items = NULL;
	ok_result.count = 0;
	ok_result.capacity = 0;
	ok_result.ok = 1;
	ok_result.error[0] = '\0';
	return (ok_result);
}

/**
 * @brief List items from inputs and raw_inputs
 * @param inputs Array of input directories to scan for .git
 * @param input_count Number of inputs
 * @param raw_inputs Array of raw directories to add directly
 * @param raw_count Number of raw_inputs
 * @return ListItemsResult with ok=1 on success, error message on failure
 */
ListItemsResult life_saver__listItems(
	const char **inputs, int input_count,
	const char **raw_inputs, int raw_count)
{
	ListItemsResult result;
	result.items = NULL;
	result.count = 0;
	result.capacity = 0;
	result.ok = 1;
	result.error[0] = '\0';

	for (int i = 0; i < input_count; i++)
	{
		if (!is_directory(inputs[i]))
		{
			char error_msg[128];
			snprintf(error_msg, sizeof(error_msg), "input directory not found: %s", inputs[i]);
			/* Free any items already collected */
			life_saver__freeListItems(&result);
			return (make_error(error_msg));
		}

		ListItemsResult find_result = find_git_dirs(&result, inputs[i]);
		if (!find_result.ok)
		{
			life_saver__freeListItems(&result);
			return (find_result);
		}
	}

	for (int i = 0; i < raw_count; i++)
	{
		if (!is_directory(raw_inputs[i]))
		{
			char error_msg[128];
			snprintf(error_msg, sizeof(error_msg), "raw input directory not found: %s", raw_inputs[i]);
			life_saver__freeListItems(&result);
			return (make_error(error_msg));
		}
		add_item(&result, raw_inputs[i]);
	}

	return (result);
}

/**
 * @brief Free a ListItemsResult
 * @param result Pointer to ListItemsResult to free
 * @return void
 */
void life_saver__freeListItems(ListItemsResult *result)
{
	if (!result->items)
		return;

	for (int i = 0; i < result->count; i++)
		free((void *)result->items[i]);

	free(result->items);
	result->items = NULL;
	result->count = 0;
	result->capacity = 0;
}
