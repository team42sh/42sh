/*
** EPITECH PROJECT, 2025
** src/formatting/globbings/regex_path_handling
** File description:
** File where the functions related to path handling/formatting are processed
*/

#include "core/minishell.h"
#include <stdlib.h>

/**
 * @brief Counts the number of arguments in a path
 *
 * @param path Path to analyze
 * @return int Number of arguments
 */
static int count_path_args(IN char *path)
{
    int args_count = 1;
    int i = 0;

    while (path[i] != '\0') {
        if (path[i] == '/') {
            args_count++;
        }
        i++;
    }
    return args_count;
}

/**
 * @brief Tokenize path into args array
 *
 * @param args Target array for arguments
 * @param path_copy Copy of the path string
 * @return int Number of arguments processed
 */
static int tokenize_path(IN char **args, IN char *path_copy)
{
    char *token = NULL;
    int i = 0;

    if (path_copy)
        token = my_strtok(path_copy, '/');
    while (token != NULL) {
        args[i] = my_strdup(token);
        free(token);
        token = my_strtok(NULL, '/');
        i++;
    }
    args[i] = NULL;
    return i;
}

/**
 * @brief Split a path into args
 *
 * @param path Path to split
 * @return char** Array of path args
 */
char **split_path(IN char *path)
{
    int args_count = 0;
    char **args = NULL;
    char *path_copy = NULL;

    if (!path)
        return NULL;
    args_count = count_path_args(path);
    args = malloc(sizeof(char *) * (args_count + 1));
    if (!args)
        return NULL;
    path_copy = my_strdup(path);
    if (!path_copy) {
        free_array_string(args);
        return NULL;
    }
    tokenize_path(args, path_copy);
    free(path_copy);
    return args;
}

/**
 * @brief Join path args into a full path
 *
 * @param args Array of path args
 * @param end Index to stop at
 * @return char* Joined path
 */
char *join_path(IN char **args, IN int end)
{
    int total_len = 1;
    char *path = NULL;

    if (!args)
        return NULL;
    for (int i = 0; i < end && args[i] != NULL; i++)
        total_len += my_strlen(args[i]) + 1;
    path = malloc(sizeof(char) * total_len);
    if (!path)
        return NULL;
    path[0] = '\0';
    for (int i = 0; i < end && args[i] != NULL; i++) {
        my_strcat(path, args[i]);
        if (i < end - 1 && args[i + 1] != NULL)
            my_strcat(path, "/");
    }
    return path;
}

/**
 * @brief Create a path with separator if needed
 *
 * @param base_dir Base directory
 * @param args Path args
 * @return char* New path (caller must free)
 */
char *create_path(IN char *base_dir, IN char *args)
{
    char *new_path = NULL;
    int base_len = 0;
    int need_slash = 0;
    int total_len = 0;

    base_len = my_strlen(base_dir);
    need_slash = (base_len > 0 && base_dir[base_len - 1] != '/');
    total_len = base_len + my_strlen(args) + need_slash + 1;
    new_path = malloc(total_len);
    if (!new_path)
        return NULL;
    my_strcpy(new_path, base_dir);
    if (need_slash)
        my_strcat(new_path, "/");
    my_strcat(new_path, args);
    return new_path;
}
