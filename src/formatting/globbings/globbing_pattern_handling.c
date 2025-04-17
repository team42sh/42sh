/*
** EPITECH PROJECT, 2025
** src/formatting/globbings/globbing_pattern_handling
** File description:
** handling of globbings patterns
*/

#include "core/minishell.h"

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
        base_pattern = strchr(pattern, '/');
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
        if (match_pattern(array[i], base_pattern)) {
            result[result_index] = my_strdup(array[i]);
            result_index++;
        }
    }
    result[result_index] = NULL;
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
    free_array_string(temp_tab);
    free_array_string(filtered_tab);
    return argv;
}
