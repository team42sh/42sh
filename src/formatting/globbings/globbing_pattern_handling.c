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
 * @brief Free some variables used to determine
 *        the type of path handling to do
 * 
 * @param before_star 
 * @param after_star 
 */
static void free_comparaison_variables(IN char *before_star,
    IN char *after_star)
{
    if (before_star)
        free(before_star);
    if (after_star)
        free(after_star);
}

/**
 * @brief Match string against pattern with * wildcard
 * 
 * @param str 
 * @param pattern 
 * @return int 
 */
int match_pattern(IN char *str, IN char *pattern)
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
            my_strlen(after_star))) && (str[0] != '.' || pattern[0] == '.');
    free_comparaison_variables(before_star, after_star);
    return result;
}

/**
 * @brief Count matching entries in array
 * 
 * @param array 
 * @param pattern 
 * @return int 
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
 * 
 * @param array 
 * @param pattern 
 * @param result 
 */
void fill_matches(IN char **array, IN char *pattern, OUT char **result)
{
    int result_index = 0;
    int len_until_slash = index_of_last_occurrence(pattern, '/') + 1;
    char *base_pattern = NULL;
    char *path = my_strncpy_alloc(pattern, len_until_slash);
    char *final_path = NULL;

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
    free(path);
}

/**
 * @brief Checks the validity of a path and the dir
 * 
 * @param path 
 * @param dir 
 * @param count 
 * @return int 
 */
int check_path_validity(IN char *path, IN DIR **dir, OUT int *count)
{
    if (!path)
        return 1;
    *dir = opendir(path);
    if (!*dir) {
        free(path);
        return 1;
    }
    *count = count_files(path);
    if (*count <= 0) {
        free(path);
        closedir(*dir);
        return 1;
    }
    return 0;
}

/**
 * @brief Process a single globbing pattern
 */
char **process_globbing_pattern(OUT char **argv, IN int globbing_index)
{
    char *path = NULL;
    DIR *dir = NULL;
    char **temp_tab = NULL;
    char **filtered_tab = NULL;
    int count = 0;

    path = create_path_to_dir(argv[globbing_index]);
    if (check_path_validity(path, &dir, &count) == 1) 
        return argv;
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
