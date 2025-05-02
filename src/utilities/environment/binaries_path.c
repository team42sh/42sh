/*
** EPITECH PROJECT, 2025
** src/utils/binaries_path
** File description:
** This file contains some function for searching binaries in the PATH env
*/

#include "core/minishell.h"

/*
 * Check if the binary with a path is existing or not. If not return false
 * If yes return true.
 */
static bool is_binary_existing(char full_path[4096], char *dir, char *command)
{
    my_strcpy(full_path, dir);
    if (full_path[my_strlen(full_path) - 1] == '/')
        my_strcat_list(full_path, command, NULL);
    else
        my_strcat_list(full_path, "/", command, NULL);
    if (!access(full_path, X_OK))
        return true;
    return false;
}

/*
 * Try to find the path of the binary in all PATH env variable.
 */
static char *find_binary_in_paths(IN char *path_var, IN char *command)
{
    char *dir = my_strtok(path_var, ':');
    char *result = NULL;
    char full_path[4096];

    while (dir) {
        if (is_binary_existing(full_path, dir, command)) {
            free(dir);
            result = my_strdup(full_path);
            break;
        }
        free_null_check(dir);
        dir = my_strtok(NULL, ':');
        if (dir == NULL)
            break;
    }
    free(path_var);
    return result;
}

/*
 * First check if it's a relative path or absolute path.
 * Starting with : '/' or '.'
 * If the PATH env variable exist then start finding it in the env PATH.
 */
char *get_binary_path(IN char *command)
{
    char *path = concat_strarray(var_search("path"), ":");

    if ((command[0] == '.' || command[0] == '/') && access(command, X_OK) == 0)
        return my_strdup(command);
    if (!path)
        return NULL;
    return find_binary_in_paths(path, command);
}
