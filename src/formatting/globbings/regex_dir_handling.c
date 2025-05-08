/*
** EPITECH PROJECT, 2025
** src/formatting/globbings/regex_dir_handling
** File description:
** file where the funtions related to dir handlig are processed
*/

#include "core/minishell.h"

/**
 * @brief Check if directory exists
 *
 * @param path Path to check
 * @return int 1 if exists, 0 otherwise
 */
int dir_exists(IN char *path)
{
    DIR *dir = NULL;

    dir = opendir(path);
    if (!dir)
        return 0;
    closedir(dir);
    return 1;
}

/**
 * @brief Check if path is a directory and matches next args
 *
 * @param path Path to check
 * @return int 1 if directory, 0 otherwise
 */
int is_valid_directory(IN char *path)
{
    DIR *dir = NULL;

    dir = opendir(path);
    if (!dir)
        return 0;
    closedir(dir);
    return 1;
}

/**
 * @brief Get directory part of a path
 *
 * @param path Full path
 * @return char* Directory part (caller must free)
 */
char *get_dir_path(IN char *path)
{
    int last_slash = 0;
    char *dir_path = NULL;

    if (!path)
        return NULL;
    last_slash = -1;
    for (int i = 0; path[i] != '\0'; i++)
        if (path[i] == '/')
            last_slash = i;
    if (last_slash == -1)
        return my_strdup("./");
    dir_path = malloc(sizeof(char) * (last_slash + 2));
    if (!dir_path)
        return NULL;
    my_strncpy(dir_path, path, last_slash + 1);
    dir_path[last_slash + 1] = '\0';
    return dir_path;
}
