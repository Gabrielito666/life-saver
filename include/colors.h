/**
 * @file
 * @source ./include/colors.h
 * @description ANSI color macros for terminal output
 */

#ifndef LIFE_SAVER__COLORS_H
#define LIFE_SAVER__COLORS_H

#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_RESET   "\033[0m"

#define ERR_PREFIX  COLOR_RED "[life-saver ERROR]: " COLOR_RESET
#define OK_PREFIX   COLOR_GREEN "[life-saver]: " COLOR_RESET

#endif
