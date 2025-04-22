/*
** EPITECH PROJECT, 2025
** src/formatting/globbings/globbing_pattern_handling
** File description:
** handling of globbings patterns
*/

#include "core/minishell.h"
#include "my_printf.h"
#include <stdlib.h>

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
    char *base_pattern = NULL;

    for (int i = 0; array[i] != NULL; i++) {
        base_pattern = strchr(pattern, '*');
        base_pattern = base_pattern ? base_pattern + 1 : pattern;
        if (match_pattern(array[i],
            &base_pattern[index_of_last_occurrence(base_pattern, '/')]))
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
    char *base_pattern = NULL;
    char *path = NULL;
    int len_until_slash = index_of_last_occurrence(pattern, '/') + 1;
    char *final_path = NULL;

    path = malloc(sizeof(char) * (len_until_slash + 1));
    path = my_strncpy(path, pattern, len_until_slash);
    path[len_until_slash] = '\0';
    base_pattern = strrchr(pattern, '/');
    base_pattern = base_pattern ? base_pattern + 1 : pattern;
    for (int i = 0; array[i] != NULL; i++) {
        if (match_pattern(array[i], base_pattern) && result != NULL) {
            final_path = malloc(sizeof(char) *
            (len_until_slash + my_strlen(array[i]) + 1));
            my_strcpy(final_path, path);
            my_strcat(final_path, array[i]);
            result[result_index] = my_strdup(final_path);
            result_index++;
            free(final_path);
        }
    }
    result[result_index] = NULL;
}

/**
 * @brief Process a single globbing pattern
 */
char **process_globbing_pattern(char **argv, int globbing_index)
{
    char *path = NULL;
    DIR *dir = NULL;
    char **temp_tab = NULL;
    char **filtered_tab = NULL;
    int count = 0;

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
