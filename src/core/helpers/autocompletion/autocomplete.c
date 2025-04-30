/*
** EPITECH PROJECT, 2025
** src/formatting/globbings/autocomplete
** File description:
** Functions to retrieve the files/bins needed for autocomplete
*/

#include "core/minishell.h"
#include "core/parser.h"
#include "my_printf.h"

/**
 * @brief Format file or directory name based on its type
 *
 * @param path
 * @param name
 * @return char*
 */
static char *format_entry(IN char *path, IN char *name)
{
    struct stat file_stat;
    char full_path[PATH_MAX];
    char *formatted_name = NULL;

    snprintf(full_path, PATH_MAX, "%s/%s", path, name);
    if (lstat(full_path, &file_stat) == -1)
        return my_strdup(name);
    if (S_ISDIR(file_stat.st_mode)) {
        formatted_name = my_strcat_alloc(name, "/");
    }
    if (S_ISLNK(file_stat.st_mode)) {
        formatted_name = my_strcat_alloc(name, "@");
    }
    if (!S_ISDIR(file_stat.st_mode) && !S_ISLNK(file_stat.st_mode)) {
        formatted_name = my_strdup(name);
    }
    return formatted_name;
}

/**
 * @brief Append a string to a dynamically allocated array
 *
 * @param array
 * @param str
 * @return char**
 */
static char **append_to_array(IN char **array, IN char *str)
{
    int count = 0;
    char **new_array = NULL;
    int i = 0;

    while (array && array[count] != NULL)
        count++;
    new_array = malloc(sizeof(char *) * (count + 2));
    if (!new_array)
        return NULL;
    for (i = 0; i < count; i++)
        new_array[i] = array[i];
    new_array[count] = my_strdup(str);
    new_array[count + 1] = NULL;
    if (array)
        free(array);
    return new_array;
}

/**
 * @brief Process entries in a directory with filtering
 *
 * @param dir Directory handle
 * @param path Directory path
 * @param prefix Filter prefix
 * @return char** Array of matching entries
 */
static char **process_directory_entries(IN DIR *dir, IN char *path,
    IN char *prefix)
{
    struct dirent *entry;
    char **result = NULL;
    char *formatted_entry = NULL;
    int prefix_len = 0;

    prefix_len = prefix ? my_strlen(prefix) : 0;
    for (entry = readdir(dir); entry != NULL; entry = readdir(dir)) {
        if (!is_visible_file(entry->d_name))
            continue;
        if (prefix && my_strncmp(entry->d_name, prefix, prefix_len) != 0)
            continue;
        formatted_entry = format_entry(path, entry->d_name);
        result = append_to_array(result, formatted_entry);
        free(formatted_entry);
    }
    return result;
}

/**
 * @brief List all files in a directory, filtering and formatting them
 *
 * @param path The directory path. If NULL, defaults to current directory
 * @param prefix The prefix to filter files by, or NULL for no filtering
 * @return char**
 */
static char **list_directory_files(IN char *path, IN char *prefix)
{
    char **result = NULL;
    DIR *dir = NULL;
    char *dir_path = NULL;

    dir_path = path ? path : ".";
    dir = opendir(dir_path);
    if (!dir)
        return NULL;
    result = process_directory_entries(dir, dir_path, prefix);
    closedir(dir);
    return result;
}

/**
 * @brief Process binary files in a directory matching prefix
 *
 * @param dir Directory path
 * @param prefix Prefix to match
 * @param binaries Current binaries array
 * @return char** Updated binaries array
 */
static char **process_binaries_in_dir(IN char *dir, IN char *prefix,
    OUT char **binaries)
{
    DIR *dir_stream = NULL;
    char full_path[PATH_MAX];

    if (prefix == NULL)
        return NULL;
    dir_stream = opendir(dir);
    if (dir_stream == NULL)
        return binaries;
    for (struct dirent *entry = readdir(dir_stream); entry != NULL;
        entry = readdir(dir_stream)) {
        if (my_strncmp(entry->d_name, prefix, my_strlen(prefix)) != 0)
            continue;
        snprintf(full_path, PATH_MAX, "%s/%s", dir, entry->d_name);
        if (is_executable_file(full_path) &&
            str_in_tab(binaries, entry->d_name) == 0)
            binaries = append_to_array(binaries, entry->d_name);
    }
    closedir(dir_stream);
    return binaries;
}

/**
 * @brief List all binaries in PATH directories matching a prefix
 *
 * @param prefix The prefix to match against file names
 * @return char** Array of matching binaries, NULL if none found
 */
static char **list_binaries_with_prefix(IN char *prefix)
{
    char **binaries = NULL;
    char *path_copy = NULL;
    char *dir = NULL;

    if (getenv("PATH") == NULL)
        return NULL;
    path_copy = my_strdup(getenv("PATH"));
    dir = strtok(path_copy, ":");
    while (dir != NULL) {
        binaries = process_binaries_in_dir(dir, prefix, binaries);
        dir = strtok(NULL, ":");
    }
    free(path_copy);
    if (binaries == NULL) {
        binaries = malloc(sizeof(char *));
        if (binaries != NULL)
            binaries[0] = NULL;
    }
    return binaries;
}

/**
 * @brief Combine two arrays of results
 *
 * @param result Main result array
 * @param binaries Secondary array to add
 * @return char** Combined array
 */
static char **combine_results(OUT char **result, IN char **binaries)
{
    int i = 0;

    if ((!result || result[0] == NULL) && binaries) {
        free(result);
        return binaries;
    }
    if (binaries) {
        for (i = 0; binaries[i] != NULL; i++) {
            result = append_to_array(result, binaries[i]);
            free(binaries[i]);
        }
        free(binaries);
    }
    return result;
}

/**
 * @brief Process input containing path separator
 *
 * @param input Input string with path
 * @return char** Matching files
 */
static char **process_path_input(IN char *input)
{
    char **result = NULL;
    char *last_slash = NULL;
    char *dir_path = NULL;
    char *prefix = NULL;

    last_slash = strrchr(input, '/');
    dir_path = my_strdup(input);
    if (last_slash) {
        dir_path[last_slash - input + 1] = '\0';
        prefix = last_slash + 1;
    }
    result = list_directory_files(dir_path, prefix);
    free(dir_path);
    return result;
}

/**
 * @brief Process input without path separator
 *
 * @param input Input string without path
 * @return char** Matching files and binaries
 */
static char **process_non_path_input(IN char *input)
{
    char **result = NULL;
    char **binaries = NULL;

    result = list_directory_files(NULL, input);
    binaries = list_binaries_with_prefix(input);
    return combine_results(result, binaries);
}

/**
 * @brief Fill autocomplete suggestions based on input
 *
 * @param input Input string to autocomplete
 * @return char** Array of autocomplete suggestions
 */
char **fill_autocomplete(IN char *input)
{
    char **result = NULL;

    if (input == NULL || strchr(input, '/') == NULL) {
        result = process_non_path_input(input);
    } else {
        result = process_path_input(input);
    }
    return result;
}
