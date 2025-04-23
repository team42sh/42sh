/*
** EPITECH PROJECT, 2025
** src/formatting/globbings/globbing_copy
** File description:
** file for globbing copy functions
*/

#include "core/minishell.h"

/**
 * @brief Copy elements before globbing index
 *
 * @param argv
 * @param result
 * @param globbing_index
 */
void copy_before_globbing(IN char **argv, OUT char **result,
    IN int globbing_index)
{
    for (int i = 0; i < globbing_index; i++)
        result[i] = my_strdup(argv[i]);
}

/**
 * @brief Copy matched entries to result
 *
 * @param matches
 * @param result
 * @param globbing_index
 */
void copy_matches(IN char **matches, OUT char **result, IN int globbing_index)
{
    int result_index = globbing_index;

    for (int i = 0; matches[i] != NULL; i++) {
        result[result_index] = my_strdup(matches[i]);
        globbing_index++;
    }
}

/**
 * @brief Copy elements after globbing index
 *
 * @param argv
 * @param result
 * @param start_pos
 * @param result_index
 */
void copy_after_globbing(IN char **argv, IN char **result, IN int start_pos,
    OUT int result_index)
{
    for (int i = start_pos; argv[i] != NULL; i++) {
        result[result_index] = my_strdup(argv[i]);
        result_index++;
    }
    result[result_index] = NULL;
}
