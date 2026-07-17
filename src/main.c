/**
 * @file
 * @source ./src/main.c
 * @description Punto de entrada de life-saver
 */

#include <stdio.h>
#include "parse-args.h"
#include "command_help.h"
#include "command-saver.h"
#include "colors.h"

/**
 * @brief Punto de entrada
 * @param argc Cantidad de argumentos
 * @param argv Vector de argumentos
 * @return 0 en caso de éxito, 1 en caso de error
 */
int main(int argc, char *argv[])
{
	ParseResult parse_result = life_saver__parseArgs(argc, argv);

	if (!parse_result.ok)
	{
		fprintf(stderr, ERR_PREFIX "%s\n", parse_result.error);
		return 1;
	}

	if (parse_result.args.help)
	{
		life_saver__commandHelp();
		return 0;
	}

	if (parse_result.args.input_count == 0 && parse_result.args.raw_count == 0)
	{
		fprintf(stderr, ERR_PREFIX "at least one -i/--input or -r/--raw-input is required\n");
		return 1;
	}

	SaverResult saver_result = life_saver__commandSaver(parse_result.args);

	if (!saver_result.ok)
	{
		fprintf(stderr, ERR_PREFIX "%s\n", saver_result.error);
		return 1;
	}

	return 0;
}
