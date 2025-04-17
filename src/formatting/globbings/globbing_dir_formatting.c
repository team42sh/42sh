/*
** EPITECH PROJECT, 2025
** src/formatting/globbings/globbing_dir_formatting
** File description:
** file to process path creation and dir handling
*/

#include "core/minishell.h"

/**
 * @brief Read directory entries to array
 */
char **read_dir_entries(DIR *dir, int count)
{
    char **temp_tab;
    struct dirent *entry;
    int i = 0;

    temp_tab = malloc(sizeof(char *) * (count + 1));
    if (!temp_tab)
        return NULL;
    while ((entry = readdir(dir)) != NULL && i < count) {
        temp_tab[i++] = my_strdup(entry->d_name);
    }
    temp_tab[i] = NULL;
    return temp_tab;
}
