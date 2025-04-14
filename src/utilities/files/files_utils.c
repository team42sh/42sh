/*
** EPITECH PROJECT, 2025
** files_utils.c
** File description:
** This file contains some functions used to get informations for files
*/

#include "core/minishell.h"

/*
 * With a path and type which is the bit mask in man 2 stat.
 * Know if the file is the same type as type given in parameter.
 * If the file is not found then return false.
 */
bool is_file_type(char *path_file, mode_t type)
{
    struct stat stat_data;

    if (stat(path_file, &stat_data) != -1)
        return stat_data.st_mode & type;
    return false;
}

/*
 * Check with stat if the file using his path is existing or not.
 * Return true or false.
 */
bool is_file_exist(char *path_file)
{
    struct stat stat_data;

    if (stat(path_file, &stat_data) == -1)
        return false;
    return true;
}
