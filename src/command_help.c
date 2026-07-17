/**
 * @file
 * @source ./src/command_help.c
 * @description Help command implementation for life-saver CLI
 */

#include <stdio.h>
#include "command_help.h"

/**
 * @brief Print help message to stdout
 * @return void
 */
void life_saver__commandHelp(void)
{
	printf("life-saver - Backup tool for git repositories\n\n");
	printf("Usage: life-saver [OPTIONS]\n\n");
	printf("Options:\n");
	printf("  -i, --input DIR       Directory to scan (repeatable)\n");
	printf("  -o, --output FILE     Output file (optional)\n");
	printf("  -r, --raw-input DIR   Raw directory to include (repeatable)\n");
	printf("  -h, --help            Show this help\n");
}
