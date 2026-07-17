/**
 * @file
 * @source ./include/list-items.h
 * @description List items from inputs (find .git) and raw_inputs
 */

#ifndef LIFE_SAVER__LIST_ITEMS_H
#define LIFE_SAVER__LIST_ITEMS_H

#define LIFE_SAVER__LIST_ITEMS_ERROR_SIZE 256

/**
 * @brief Result of listing items
 */
typedef struct ListItemsResult
{
	const char **items;
	int count;
	int capacity;
	int ok;
	char error[LIFE_SAVER__LIST_ITEMS_ERROR_SIZE];
}
ListItemsResult;

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
	const char **raw_inputs, int raw_count
);

/**
 * @brief Free a ListItemsResult
 * @param result Pointer to ListItemsResult to free
 * @return void
 */
void life_saver__freeListItems(ListItemsResult *result);

#endif
