/**
 * @file
 * @source ./include/command-saver.h
 * @description Saver command — orchestrate list-items and pack
 */

#ifndef LIFE_SAVER__COMMAND_SAVER_H
#define LIFE_SAVER__COMMAND_SAVER_H

#include "parse-args.h"

#define LIFE_SAVER__SAVER_ERROR_SIZE 256

/**
 * @brief Result of the saver command
 */
typedef struct SaverResult
{
	int ok;
	char error[LIFE_SAVER__SAVER_ERROR_SIZE];
}
SaverResult;

/**
 * @brief Run the saver command: list items, print log, pack into tar.gz
 * @param args Parsed CLI arguments (inputs, raw_inputs, output)
 * @return SaverResult with ok=1 on success, error message on failure
 */
SaverResult life_saver__commandSaver(LifeSaverArgs args);

#endif
