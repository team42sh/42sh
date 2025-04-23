/*
** EPITECH PROJECT, 2025
** src/formatting/globbings/everything_globbing
** File description:
** File to handle the everything globbing with UNIX style
*/

#include "core/minishell.h"

/**
 * @brief Check if string starts with prefix
 *
 * @param str
 * @param prefix
 * @param prefix_len
 * @return int
 */
int starts_with(char *str, char *prefix, int prefix_len)
{
    if (!str || !prefix)
        return 0;
    return (my_strncmp(str, prefix, prefix_len) == 0);
}

/**
 * @brief Check if string ends with suffix
 *
 * @param str
 * @param suffix
 * @param str_len
 * @param suffix_len
 * @return int
 */
int ends_with(char *str, char *suffix, int str_len, int suffix_len)
{
    if (!str || !suffix || str_len < suffix_len)
        return 0;
    return (my_strcmp(&str[str_len - suffix_len], suffix) == 0);
}

/**
 * @brief Allocate new array for results
 *
 * @param argc
 * @param match_count
 * @return char**
 */
static char **allocate_result_array(int argc, int match_count)
{
    char **result;

    if (match_count == 0)
        return NULL;
    result = malloc(sizeof(char *) * (argc + match_count));
    return result;
}

/**
 * @brief Replace globbing with matched files
 *
 * @param argv
 * @param globbing_index
 * @param matches
 * @return char**
 */
char **replace_globbing_with_matches(char **argv, int globbing_index,
    char **matches)
{
    char **result = NULL;
    int argc = 0;
    int match_count = array_count_string(matches);
    int result_index = 0;

    if (match_count == 0 || matches == NULL || argv == NULL)
        return argv;
    argc = array_count_string(argv);
    result = allocate_result_array(argc, match_count);
    if (!result)
        return argv;
    copy_before_globbing(argv, result, globbing_index);
    result_index = globbing_index;
    for (int i = 0; matches[i] != NULL; i++) {
        result[result_index] = my_strdup(matches[i]);
        result_index++;
    }
    copy_after_globbing(argv, result, globbing_index + 1, result_index);
    free_array_string(argv);
    return result;
}

/**
 * @brief Main function to handle globbing
 *
 * @param argv
 * @return char**
 */
char **change_star_to_list_of_files(IN char **argv)
{
    int globbing_index;

    if (!argv)
        return NULL;
    globbing_index = find_char_index_in_tab(argv, '*');
    if (globbing_index == -1)
        return argv;
    argv = process_globbing_pattern(argv, globbing_index);
    if (find_char_index_in_tab(argv, '*') > 0) {
        my_printf("%s: No match.\n", argv[0]);
        free_array_string(argv);
        return NULL;
    }
    if (find_char_index_in_tab(argv, '*') != -1)
        return argv;
    return argv;
}
