/*
** EPITECH PROJECT, 2025
** src/formatting/globbings/everything_globbing
** File description:
** File to handle the everything globbing with UNIX style
*/

#include "core/minishell.h"
#include "core/parser.h"
#include "my_printf.h"
#include <stdlib.h>
#include <string.h>


/**
 * @brief Handle simple path case
 */
char *handle_simple_path(char *globbing_string)
{
    if (my_strcmp(globbing_string, "*") == 0)
        return my_strdup(".");
    return NULL;
}

/**
 * @brief Handle slash before asterisk case
 */
char *handle_slash_before_star(char *globbing_string, int globbing_index)
{
    if (globbing_index > 0 && globbing_string[globbing_index - 1] == '/')
        return my_strncpy_alloc(globbing_string, globbing_index);
    return NULL;
}

/**
 * @brief Check if string starts with prefix
 */
int starts_with(char *str, char *prefix, int prefix_len)
{
    if (!str || !prefix)
        return 0;
    return (my_strncmp(str, prefix, prefix_len) == 0);
}

/**
 * @brief Check if string ends with suffix
 */
int ends_with(char *str, char *suffix, int str_len, int suffix_len)
{
    if (!str || !suffix || str_len < suffix_len)
        return 0;
    return (my_strcmp(&str[str_len - suffix_len], suffix) == 0);
}

/**
 * @brief Match string against pattern with * wildcard
 */
int match_pattern(char *str, char *pattern)
{
    char *before_star = NULL;
    char *after_star = NULL;
    int star_pos = char_in_str(pattern, '*');
    int result = 0;

    if (str == NULL || pattern == NULL)
        return 0;
    if (my_strcmp(pattern, "*") == 0)
        return (str[0] != '.');
    if (star_pos == -1)
        return (my_strcmp(str, pattern) == 0);
    if (star_pos > 0)
        before_star = my_strncpy_alloc(pattern, star_pos);
    if (pattern[star_pos + 1] != '\0')
        after_star = my_strdup(&pattern[star_pos + 1]);
    result = (!before_star || starts_with(str, before_star, star_pos)) &&
            (!after_star || ends_with(str, after_star, my_strlen(str),
            my_strlen(after_star))) &&
            (str[0] != '.' || pattern[0] == '.');
    if (before_star)
        free(before_star);
    if (after_star)
        free(after_star);
    return result;
}

/**
 * @brief Count matching entries in array
 */
int count_matches(char **array, char *pattern)
{
    int count = 0;
    char *base_pattern;

    for (int i = 0; array[i] != NULL; i++) {
        base_pattern = strrchr(pattern, '/');
        base_pattern = base_pattern ? base_pattern + 1 : pattern;
        if (match_pattern(array[i], base_pattern))
            count++;
    }
    return count;
}

/**
 * @brief Fill result array with matching entries
 */
void fill_matches(char **array, char *pattern, char **result)
{
    int result_index = 0;
    char *base_pattern;

    for (int i = 0; array[i] != NULL; i++) {
        base_pattern = strrchr(pattern, '/');
        base_pattern = base_pattern ? base_pattern + 1 : pattern;
        if (match_pattern(array[i], base_pattern))
            result[result_index++] = my_strdup(array[i]);
    }
    result[result_index] = NULL;
}

/**
 * @brief Allocate new array for results
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
 * @brief Copy elements before globbing index
 */
static void copy_before_globbing(char **argv, char **result, int globbing_index)
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

    for (int i = 0; matches[i] != NULL; i++)
        result[result_index++] = my_strdup(matches[i]);
}

/**
 * @brief Copy elements after globbing index
 */
static void copy_after_globbing(char **argv, char **result, int start_pos,
    int result_index)
{
    for (int i = start_pos; argv[i] != NULL; i++)
        result[result_index++] = my_strdup(argv[i]);
    result[result_index] = NULL;
}

/**
 * @brief Replace globbing with matched files
 */
char **replace_globbing_with_matches(char **argv, int globbing_index, 
    char **matches)
{
    char **result;
    int argc = array_count_string(argv);
    int match_count = array_count_string(matches);
    int result_index;

    if (match_count == 0)
        return argv;
    result = allocate_result_array(argc, match_count);
    if (!result)
        return argv;
    copy_before_globbing(argv, result, globbing_index);
    result_index = globbing_index;
    for (int i = 0; matches[i] != NULL; i++)
        result[result_index++] = my_strdup(matches[i]);
    copy_after_globbing(argv, result, globbing_index + 1, result_index);
    for (int i = 0; argv[i] != NULL; i++)
        free(argv[i]);
    free(argv);
    return result;
}

/**
 * @brief Free all elements of string array
 */
static void free_tab(char **array)
{
    if (!array)
        return;
    for (int i = 0; array[i] != NULL; i++)
        free(array[i]);
    free(array);
}

/**
 * @brief Extract directory path from globbing string
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
    last_slash_index = index_of_last_ocurence(globbing_string, '/');
    if (last_slash_index == -1)
        return my_strdup(".");
    return my_strncpy_alloc(globbing_string, last_slash_index + 1);
}

/**
 * @brief Process a single globbing pattern
 */
char **process_globbing_pattern(char **argv, int globbing_index)
{
    char *path;
    DIR *dir;
    char **temp_tab = NULL;
    char **filtered_tab = NULL;
    int count;

    path = create_path_to_dir(argv[globbing_index]);
    if (!path)
        return argv;
    dir = opendir(path);
    if (!dir) {
        free(path);
        return argv;
    }
    count = count_files(path);
    if (count <= 0) {
        free(path);
        closedir(dir);
        return argv;
    }
    closedir(dir);
    dir = opendir(path);
    temp_tab = read_dir_entries(dir, count);
    closedir(dir);
    filtered_tab = filter_array(temp_tab, argv[globbing_index]);
    argv = replace_globbing_with_matches(argv, globbing_index, filtered_tab);
    free(path);
    free_tab(temp_tab);
    free_tab(filtered_tab);
    return argv;
}

/**
 * @brief Main function to handle globbing
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
    if (find_char_index_in_tab(argv, '*') != -1)
        return change_star_to_list_of_files(argv);
    return argv;
}
