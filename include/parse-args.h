/**
 * @file
 * @source ./include/parse-args.h
 * @description Argument parser for life-saver CLI
 */

#ifndef LIFE_SAVER__PARSE_ARGS_H
#define LIFE_SAVER__PARSE_ARGS_H

#define LIFE_SAVER__MAX_INPUTS 64
#define LIFE_SAVER__ERROR_MSG_SIZE 256

/**
 * @brief Parsed CLI arguments
 */
typedef struct LifeSaverArgs
{
	const char *inputs[LIFE_SAVER__MAX_INPUTS];
	int input_count;
	const char *raw_inputs[LIFE_SAVER__MAX_INPUTS];
	int raw_count;
	const char *output;
	int help;
}
LifeSaverArgs;

/**
 * @brief Result of parsing CLI arguments
 */
typedef struct ParseResult
{
	LifeSaverArgs args;
	int ok;
	char error[LIFE_SAVER__ERROR_MSG_SIZE];
}
ParseResult;

/**
 * @brief Parse CLI arguments into a ParseResult
 * @param argc Argument count
 * @param argv Argument vector
 * @return ParseResult with args populated on success, error message on failure
 */
ParseResult life_saver__parseArgs(int argc, char *argv[]);

#endif
