/*
** EPITECH PROJECT, 2025
** src/formatting/globbings/globbing_copy
** File description:
** file for globbing copy functions
*/

#include "core/minishell.h"

/**
 * @brief Copy elements before globbing index
 */
void copy_before_globbing(char **argv, char **result, int globbing_index)
{
    for (int i = 0; i < globbing_index; i++)
        result[i] = my_strdup(argv[i]);
}

/**
 * @brief Copy matched entries to result
 */
void copy_matches(char **matches, char **result, int globbing_index)
{
    int result_index = globbing_index;

    for (int i = 0; matches[i] != NULL; i++) {
        result[result_index] = my_strdup(matches[i]);
        globbing_index++;
    }
}

/**
 * @brief Copy elements after globbing index
 */
void copy_after_globbing(char **argv, char **result, int start_pos,
    int result_index)
{
    for (int i = start_pos; argv[i] != NULL; i++) {
        result[result_index] = my_strdup(argv[i]);
        result_index++;
    }
    result[result_index] = NULL;
}
