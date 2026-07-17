/**
 * @file
 * @source ./src/pack.c
 * @description Pack directories into a tar.gz archive
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "pack.h"

#define TEMP_DIR_template "/tmp/life-saver-XXXXXX"
#define CMD_SIZE 1024
#define PATH_SIZE 1024

/**
 * @brief Create an error result
 * @param msg Error message
 * @return PackResult with ok=0 and error set
 */
static PackResult make_error(const char *msg)
{
	PackResult result;
	result.ok = 0;
	strncpy(result.error, msg, LIFE_SAVER__PACK_ERROR_SIZE - 1);
	result.error[LIFE_SAVER__PACK_ERROR_SIZE - 1] = '\0';
	return (result);
}

/**
 * @brief Create a success result
 * @return PackResult with ok=1
 */
static PackResult make_ok(void)
{
	PackResult result;
	result.ok = 1;
	result.error[0] = '\0';
	return (result);
}

/**
 * @brief Transform a path for safe storage in temp directory
 *
 * Rules:
 *   ./xxx  → dot_xxx
 *   ../xxx → dotdot_xxx
 *   ~/xxx  → HOME_xxx
 *   /xxx   → ROOT_xxx
 *
 * @param input Source path
 * @param output Buffer for transformed path
 * @param output_size Size of output buffer
 * @return void
 */
static void transform_path(const char *input, char *output, int output_size)
{
	const char *p = input;
	int pos = 0;

	/* Transform leading special prefixes only */
	while (p[0] == '.' && p[1] == '.' && p[2] == '/')
	{
		pos += snprintf(output + pos, output_size - pos, "dotdot_");
		p += 3;
	}

	if (p[0] == '.' && p[1] == '/')
	{
		pos += snprintf(output + pos, output_size - pos, "dot_");
		p += 2;
	}
	else if (p[0] == '~' && p[1] == '/')
	{
		pos += snprintf(output + pos, output_size - pos, "HOME_");
		p += 2;
	}
	else if (p[0] == '/')
	{
		pos += snprintf(output + pos, output_size - pos, "ROOT_");
		p += 1;
	}
	else if (pos == 0)
	{
		pos += snprintf(output + pos, output_size - pos, "dot_");
	}

	/* Copy rest as-is, only replacing ../ and ./ in the middle */
	while (*p)
	{
		if (p[0] == '.' && p[1] == '.' && p[2] == '/')
		{
			pos += snprintf(output + pos, output_size - pos, "dotdot_");
			p += 3;
		}
		else if (p[0] == '.' && p[1] == '/')
		{
			pos += snprintf(output + pos, output_size - pos, "dot_");
			p += 2;
		}
		else
		{
			output[pos++] = *p;
			p++;
		}
	}
	output[pos] = '\0';
}

/**
 * @brief Create directory structure recursively
 * @param path Directory path to create
 * @return 0 on success, -1 on failure
 */
static int mkdirs(const char *path)
{
	char tmp[PATH_SIZE];
	char *p = NULL;

	snprintf(tmp, sizeof(tmp), "%s", path);

	for (p = tmp + 1; *p; p++)
	{
		if (*p == '/')
		{
			*p = '\0';
			mkdir(tmp, 0755);
			*p = '/';
		}
	}

	return (mkdir(tmp, 0755));
}

/**
 * @brief Copy directory recursively using cp
 * @param src Source path
 * @param dst Destination path
 * @return 0 on success, -1 on failure
 */
static int copy_recursive(const char *src, const char *dst)
{
	char cmd[CMD_SIZE];

	/* Create parent directory of dst */
	char dst_parent[PATH_SIZE];
	snprintf(dst_parent, sizeof(dst_parent), "%s", dst);
	char *last_slash = strrchr(dst_parent, '/');
	if (last_slash)
	{
		*last_slash = '\0';
		mkdirs(dst_parent);
	}

	snprintf(cmd, sizeof(cmd), "cp -r %s %s", src, dst);
	return (system(cmd));
}

/**
 * @brief Pack directories into a tar.gz archive
 * @param paths Array of directory paths to pack
 * @param path_count Number of paths
 * @param output Output tar.gz filename
 * @return PackResult with ok=1 on success, error message on failure
 */
PackResult life_saver__pack(
	const char **paths, int path_count,
	const char *output)
{
	char temp_template[] = TEMP_DIR_template;
	char *temp_dir = mkdtemp(temp_template);

	if (!temp_dir)
		return (make_error("failed to create temporary directory"));

	for (int i = 0; i < path_count; i++)
	{
		char transformed[PATH_SIZE];
		char dst[PATH_SIZE];

		transform_path(paths[i], transformed, sizeof(transformed));
		snprintf(dst, sizeof(dst), "%s/%s", temp_dir, transformed);

		if (copy_recursive(paths[i], dst) != 0)
		{
			char error_msg[256];
			snprintf(error_msg, sizeof(error_msg), "failed to copy: %s", paths[i]);
			rmdir(temp_dir);
			return (make_error(error_msg));
		}
	}

	/* Create tar.gz using bash */
	char cmd[CMD_SIZE];
	snprintf(cmd, sizeof(cmd), "tar czf %s -C %s .", output, temp_dir);

	int status = system(cmd);

	/* Cleanup temp directory */
	char cleanup_cmd[CMD_SIZE];
	snprintf(cleanup_cmd, sizeof(cleanup_cmd), "rm -rf %s", temp_dir);
	system(cleanup_cmd);

	if (status != 0)
		return (make_error("failed to create tar archive"));

	return (make_ok());
}
