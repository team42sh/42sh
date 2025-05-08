/*
** EPITECH PROJECT, 2025
** src/formatting/globbings/regex_pattern
** File description:
** file where function to process pattern are done
*/

#include "core/minishell.h"

/**
 * @brief Get base name part of a path
 *
 * @param path Full path
 * @return char* Base name pattern (caller must free)
 */
char *get_base_pattern(IN char *path)
{
    int last_slash = -1;

    if (!path)
        return NULL;
    for (int i = 0; path[i] != '\0'; i++)
        if (path[i] == '/')
            last_slash = i;
    return my_strdup(&path[last_slash + 1]);
}

/**
 * @brief Check if entry should be skipped
 *
 * @param entry Directory entry
 * @param regex_pattern Regex pattern string
 * @return int 1 if should skip, 0 otherwise
 */
int should_skip_entry(struct dirent *entry, IN char *regex_pattern)
{
    int is_dot = 0;
    int is_dotdot = 0;
    int is_dot_pattern = 0;

    is_dot = (my_strncmp(entry->d_name, ".", 1) == 0);
    is_dotdot = (my_strcmp(entry->d_name, "..") == 0);
    if (!is_dot && !is_dotdot)
        return 0;
    is_dot_pattern = (my_strcmp(regex_pattern, "^\\.$") == 0);
    is_dot_pattern |= (my_strcmp(regex_pattern, "^\\.\\.$") == 0);
    return !is_dot_pattern;
}
