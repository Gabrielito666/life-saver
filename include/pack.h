/**
 * @file
 * @source ./include/pack.h
 * @description Pack directories into a tar.gz archive
 */

#ifndef LIFE_SAVER__PACK_H
#define LIFE_SAVER__PACK_H

#define LIFE_SAVER__PACK_ERROR_SIZE 256

/**
 * @brief Result of packing directories
 */
typedef struct PackResult
{
	int ok;
	char error[LIFE_SAVER__PACK_ERROR_SIZE];
}
PackResult;

/**
 * @brief Pack directories into a tar.gz archive
 * @param paths Array of directory paths to pack
 * @param path_count Number of paths
 * @param output Output tar.gz filename
 * @return PackResult with ok=1 on success, error message on failure
 */
PackResult life_saver__pack(
	const char **paths, int path_count,
	const char *output
);

#endif
