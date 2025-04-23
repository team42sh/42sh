/*
** EPITECH PROJECT, 2025
** src/formatting/globbings/globbing_dir_formatting
** File description:
** file to process path creation and dir handling
*/

#include "core/minishell.h"

/**
 * @brief Handle slash before asterisk case
 *
 * @param globbing_string
 * @param globbing_index
 * @return char*
 */
char *handle_slash_before_star(IN char *globbing_string, IN int globbing_index)
{
    if (globbing_index > 0 && globbing_string[globbing_index - 1] == '/')
        return my_strncpy_alloc(globbing_string, globbing_index);
    return NULL;
}

/**
 * @brief Handle simple path case
 *
 * @param globbing_string
 * @return char*
 */
char *handle_simple_path(IN char *globbing_string)
{
    if (my_strcmp(globbing_string, "*") == 0)
        return my_strdup(".");
    return NULL;
}

/**
 * @brief Read directory entries to array
 *
 * @param dir
 * @param count
 * @return char**
 */
char **read_dir_entries(IN DIR *dir, IN int count)
{
    char **temp_tab;
    struct dirent *entry;
    int i = 0;

    temp_tab = malloc(sizeof(char *) * (count + 1));
    if (!temp_tab)
        return NULL;
    entry = readdir(dir);
    while (entry != NULL && i < count) {
        temp_tab[i] = my_strdup(entry->d_name);
        i++;
        entry = readdir(dir);
    }
    temp_tab[i] = NULL;
    return temp_tab;
}

/**
 * @brief Extract directory path from globbing string
 *
 * @param globbing_string
 * @return char*
 */
char *create_path_to_dir(IN char *globbing_string)
{
    int globbing_index;
    int last_slash_index;
    char *path;

    if (globbing_string == NULL)
        return NULL;
    globbing_index = char_in_str(globbing_string, '*');
    path = handle_simple_path(globbing_string);
    if (path != NULL)
        return path;
    path = handle_slash_before_star(globbing_string, globbing_index);
    if (path != NULL)
        return path;
    last_slash_index = index_of_last_occurrence(globbing_string, '/');
    if (last_slash_index == -1)
        return my_strdup(".");
    return my_strncpy_alloc(globbing_string, last_slash_index + 1);
}
