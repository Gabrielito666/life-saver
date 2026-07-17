/**
 * @file
 * @source ./src/parse-args.c
 * @description Implementation of life-saver CLI argument parser
 */

#include <stdio.h>
#include <string.h>
#include "parse-args.h"

/**
 * @brief Check if a string matches a long option
 * @param arg The argument to check
 * @param short_opt The short option character
 * @param long_opt The long option string (without --)
 * @return 1 if matches, 0 otherwise
 */
static int matches_option(const char *arg, char short_opt, const char *long_opt)
{
	if (arg[0] != '-')
		return (0);

	if (arg[1] == short_opt && arg[2] == '\0')
		return (1);

	if (arg[1] == '-' && strcmp(arg + 2, long_opt) == 0)
		return (1);

	return (0);
}

/**
 * @brief Create an error result
 * @param msg Error message
 * @return ParseResult with ok=0 and error set
 */
static ParseResult make_error(const char *msg)
{
	ParseResult result;
	result.ok = 0;
	strncpy(result.error, msg, LIFE_SAVER__ERROR_MSG_SIZE - 1);
	result.error[LIFE_SAVER__ERROR_MSG_SIZE - 1] = '\0';
	return (result);
}

/**
 * @brief Parse CLI arguments into a ParseResult
 * @param argc Argument count
 * @param argv Argument vector
 * @return ParseResult with args populated on success, error message on failure
 */
ParseResult life_saver__parseArgs(int argc, char *argv[])
{
	ParseResult result;

	result.ok = 1;
	result.error[0] = '\0';
	result.args.input_count = 0;
	result.args.raw_count = 0;
	result.args.output = NULL;
	result.args.help = 0;

	for (int i = 1; i < argc; i++)
	{
		if (matches_option(argv[i], 'i', "input"))
		{
			if (i + 1 >= argc)
				return (make_error("-i/--input requires an argument"));
			if (result.args.input_count >= LIFE_SAVER__MAX_INPUTS)
				return (make_error("too many -i arguments"));
			result.args.inputs[result.args.input_count++] = argv[++i];
		}
		else if (matches_option(argv[i], 'r', "raw-input"))
		{
			if (i + 1 >= argc)
				return (make_error("-r/--raw-input requires an argument"));
			if (result.args.raw_count >= LIFE_SAVER__MAX_INPUTS)
				return (make_error("too many -r arguments"));
			result.args.raw_inputs[result.args.raw_count++] = argv[++i];
		}
		else if (matches_option(argv[i], 'o', "output"))
		{
			if (i + 1 >= argc)
				return (make_error("-o/--output requires an argument"));
			result.args.output = argv[++i];
		}
		else if (matches_option(argv[i], 'h', "help"))
		{
			result.args.help = 1;
		}
		else
		{
			char error_msg[64];
			snprintf(error_msg, sizeof(error_msg), "unknown option '%s'", argv[i]);
			return (make_error(error_msg));
		}
	}

	return (result);
}
