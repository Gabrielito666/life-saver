/**
 * @file
 * @source ./src/command-saver.c
 * @description Saver command — orchestrate list-items and pack
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "command-saver.h"
#include "list-items.h"
#include "pack.h"

#define LIFE_SAVER__DEFAULT_OUTPUT_PREFIX "life-saver-backup-"
#define LIFE_SAVER__OUTPUT_BUFFER_SIZE 128

/**
 * @brief Create an error result
 * @param msg Error message
 * @return SaverResult with ok=0 and error set
 */
static SaverResult make_error(const char *msg)
{
	SaverResult result;
	result.ok = 0;
	strncpy(result.error, msg, LIFE_SAVER__SAVER_ERROR_SIZE - 1);
	result.error[LIFE_SAVER__SAVER_ERROR_SIZE - 1] = '\0';
	return (result);
}

/**
 * @brief Generate default output filename with timestamp
 * @param buffer Buffer to store the generated filename
 * @param size Size of the buffer
 * @return void
 */
static void generate_default_output(char *buffer, int size)
{
	time_t now = time(NULL);
	struct tm *t = localtime(&now);

	strftime(buffer, size, LIFE_SAVER__DEFAULT_OUTPUT_PREFIX "%Y-%m-%d-%H%M%S.tar.gz", t);
}

/**
 * @brief Run the saver command: list items, print log, pack into tar.gz
 * @param args Parsed CLI arguments (inputs, raw_inputs, output)
 * @return SaverResult with ok=1 on success, error message on failure
 */
SaverResult life_saver__commandSaver(LifeSaverArgs args)
{
	/* Step 1: List items from inputs and raw_inputs */
	ListItemsResult items = life_saver__listItems(
		args.inputs, args.input_count,
		args.raw_inputs, args.raw_count
	);

	if (!items.ok)
	{
		SaverResult result = make_error(items.error);
		life_saver__freeListItems(&items);
		return (result);
	}

	if (items.count == 0)
	{
		life_saver__freeListItems(&items);
		return (make_error("no items found to pack"));
	}

	/* Step 2: Print items log */
	printf("Items found (%d):\n", items.count);
	for (int i = 0; i < items.count; i++)
		printf("  [%d] %s\n", i, items.items[i]);

	/* Step 3: Pack items */
	char default_output[LIFE_SAVER__OUTPUT_BUFFER_SIZE];
	const char *output;

	if (args.output)
	{
		output = args.output;
	}
	else
	{
		generate_default_output(default_output, sizeof(default_output));
		output = default_output;
	}

	PackResult pack_result = life_saver__pack(
		items.items, items.count, output
	);

	life_saver__freeListItems(&items);

	if (!pack_result.ok)
		return (make_error(pack_result.error));

	/* Step 4: Print output path */
	printf("Output: %s\n", output);

	return ((SaverResult){.ok = 1, .error = {0}});
}
