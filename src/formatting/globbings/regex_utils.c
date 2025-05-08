/*
** EPITECH PROJECT, 2025
** src/formatting/globbings/regex_utils
** File description:
** utilitary functions for regex globbings
*/

#include "core/minishell.h"

static void compare_and_sort(OUT char *temp, OUT char **paths, IN int j)
{
    if (my_strcmp(paths[j], paths[j + 1]) > 0) {
        temp = paths[j];
        paths[j] = paths[j + 1];
        paths[j + 1] = temp;
    }
}

/**
 * @brief Sort matching paths alphabetically
 *
 * @param paths Array of paths to sort
 */
void sort_paths(OUT char **paths)
{
    int count = 0;
    char *temp = NULL;

    if (!paths)
        return;
    while (paths[count] != NULL)
        count++;
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            compare_and_sort(temp, paths, j);
        }
    }
}

/**
 * @brief Find index of char in string
 *
 * @param str
 * @param c
 * @return int
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
 * @brief Reallocate result array when needed for globbing expansion
 *
 * @param state Globbing state information
 * @param matches Array of glob matches
 * @param new_size New size for reallocation
 * @return char** Reallocated array or NULL on failure
 */
char **reallocate_result_array(IN glob_state_t *state, IN char **matches,
    IN int new_size)
{
    char **new_result = NULL;

    new_result = realloc(state->result, sizeof(char *) * (new_size + 1));
    if (!new_result) {
        free_array_string(matches);
        free_array_string(state->result);
        return NULL;
    }
    return new_result;
}
