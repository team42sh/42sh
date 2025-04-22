/*
** EPITECH PROJECT, 2025
** src/formatting/globbings/globbing_utils
** File description:
** utilitary functions for globbings
*/

#include "core/minishell.h"
#include "my_printf.h"

/**
 * @brief Count files in a directory
 */
int count_files(IN char *path)
{
    DIR *dir;
    struct dirent *entry;
    int i = 0;

    if (path == NULL)
        return -1;
    dir = opendir(path);
    if (dir == NULL)
        return -1;
    entry = readdir(dir);
    while (entry != NULL) {
        i++;
        entry = readdir(dir);
    }
    closedir(dir);
    return (i > 0) ? i : -1;
}

/**
 * @brief Find index of char in string
 */
int char_in_str(IN char *str, IN char c)
{
    if (str == NULL || c == '\0')
        return -1;
    for (int i = 0; str[i] != '\0'; i++)
        if (str[i] == c)
            return i;
    return -1;
}

/**
 * @brief Find index of tab element containing char
 */
int find_char_index_in_tab(IN char **tab, IN char c)
{
    if (tab == NULL || c == '\0')
        return -1;
    for (int i = 0; tab[i] != NULL; i++)
        if (char_in_str(tab[i], c) != -1)
            return i;
    return -1;
}

/**
 * @brief Find last occurrence index of char in string
 */
int index_of_last_occurrence(char *str, char c)
{
    int char_index = -1;

    if (str == NULL)
        return -1;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == c)
            char_index = i;
    }
    return char_index;
}

/**
 * @brief Filter array based on pattern
 */
char **filter_array(char **array, char *pattern)
{
    char **result = NULL;
    int count = 0;

    if (array == NULL || pattern == NULL)
        return NULL;
    count = count_matches(array, pattern);
    result = malloc(sizeof(char *) * (count + 1));
    if (!result)
        return NULL;
    result[count] = NULL;
    fill_matches(array, pattern, result);
    return result;
}
