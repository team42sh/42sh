/*
** EPITECH PROJECT, 2025
** src/formatting/globbings/autocomplete
** File description:
** Functions to retrieve the files/bins needed for autocomplete
*/

#include "core/minishell.h"
#include "core/parser.h"

/**
 * @brief Count words in a string based on a delimiter
 *
 * @param str
 * @param delimiter
 * @return int
 */
int count_words(IN char *str, IN char delimiter)
{
    int count = 0;
    int in_word = 0;
    int i = 0;

    if (str == NULL)
        return 0;
    for (i = 0; str[i] != '\0'; i++) {
        if (str[i] != delimiter && !in_word) {
            in_word = 1;
            count++;
        } 
        if (str[i] == delimiter) {
            in_word = 0;
        }
    }
    return count;
}

/**
 * @brief Extract a word from string and allocate memory for it
 *
 * @param str Source string
 * @param start Start index
 * @param end End index
 * @return char* Extracted word
 */
static char *extract_word(char *str, int start, int end)
{
    int word_len = end - start;
    
    return my_strncpy_alloc(&str[start], word_len);
}

/**
 * @brief Split a string into an array of words based on a delimiter
 *
 * @param str
 * @param delimiter
 * @return char**
 */
char **my_str_to_word_array(IN char *str, IN char delimiter)
{
    char **result = NULL;
    int word_count = 0;
    int start = 0;
    int i = 0;
    int word_index = 0;

    if (str == NULL)
        return NULL;
    word_count = count_words(str, delimiter);
    result = malloc(sizeof(char *) * (word_count + 1));
    if (!result)
        return NULL;
    for (i = 0; str[i] != '\0'; i++) {
        if (str[i] == delimiter || str[i + 1] == '\0') {
            int end = (str[i] == delimiter ? i : i + 1);
            result[word_index] = extract_word(str, start, end);
            word_index++;
            start = i + 1;
        }
    }
    result[word_count] = NULL;
    return result;
}

/**
 * @brief Check if a file is executable
 *
 * @param filename
 * @return int
 */
int is_executable_file(IN char *filename)
{
    struct stat file_stat;

    if (filename == NULL)
        return 0;
    if (stat(filename, &file_stat) == -1)
        return 0;
    return (file_stat.st_mode & S_IXUSR) != 0;
}

/**
* @brief Concatenate two strings into a newly allocated string
 *
 * @param str1
 * @param str2
 * @return char*
 */
char *my_strcat_alloc(IN char *str1, IN char *str2)
{
    char *result = NULL;
    int len1 = 0;
    int len2 = 0;

    len1 = str1 ? strlen(str1) : 0;
    len2 = str2 ? strlen(str2) : 0;
    result = malloc(sizeof(char) * (len1 + len2 + 1));
    if (!result)
        return NULL;
    if (str1)
        strcpy(result, str1);
    else
        result[0] = '\0';
    if (str2)
        strcat(result, str2);
    return result;
}

/**
 * @brief Format file or directory name based on its type
 *
 * @param path
 * @param name
 * @return char*
 */
static char *format_entry(char *path, char *name)
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
 * @brief Check if a file should be displayed (exclude hidden files and special entries)
 *
 * @param name
 * @return int
 */
static int is_visible_file(char *name)
{
    if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) {
        return 0;
    }
    return (name[0] != '.');
}

/**
 * @brief Append a string to a dynamically allocated array
 *
 * @param array
 * @param str
 * @return char**
 */
static char **append_to_array(char **array, char *str)
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
        new_array[i] = strdup(array[i]);
    new_array[count] = strdup(str);
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
static char **process_directory_entries(DIR *dir, char *path, char *prefix)
{
    char **result = NULL;
    struct dirent *entry = NULL;
    char *formatted_entry = NULL;
    int prefix_len = 0;

    prefix_len = prefix ? strlen(prefix) : 0;
    while ((entry = readdir(dir)) != NULL) {
        if (!is_visible_file(entry->d_name))
            continue;
        if (prefix && strncmp(entry->d_name, prefix, prefix_len) != 0)
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
static char **list_directory_files(char *path, char *prefix)
{
    char **result = NULL;
    DIR *dir = NULL;
    char *dir_path = NULL;

    dir_path = path ? path : ".";
    dir = opendir(dir_path);
    if (!dir) {
        printf("Error: Cannot open directory %s\n", dir_path);
        return NULL;
    }
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
static char **process_binaries_in_dir(char *dir, const char *prefix, char **binaries)
{
    DIR *dir_stream = NULL;
    struct dirent *entry = NULL;
    char full_path[PATH_MAX];
    int prefix_len = 0;
    
    prefix_len = strlen(prefix);
    dir_stream = opendir(dir);
    if (dir_stream == NULL)
        return binaries;
    while ((entry = readdir(dir_stream)) != NULL) {
        if (strncmp(entry->d_name, prefix, prefix_len) != 0)
            continue;
        snprintf(full_path, PATH_MAX, "%s/%s", dir, entry->d_name);
        if (is_executable_file(full_path))
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
static char **list_binaries_with_prefix(const char *prefix)
{
    char **binaries = NULL;
    char *path_env = NULL;
    char *path_copy = NULL;
    char *dir = NULL;

    path_env = getenv("PATH");
    if (path_env == NULL)
        return NULL;
    path_copy = strdup(path_env);
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
static char **combine_results(char **result, char **binaries)
{
    int i = 0;
    
    if ((!result || result[0] == NULL) && binaries) {
        free(result);
        return binaries;
    }
    if (binaries) {
        for (i = 0; binaries[i] != NULL; i++) {
            result = append_to_array(result, binaries[i]);
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
static char **process_path_input(char *input)
{
    char **result = NULL;
    char *last_slash = NULL;
    char *dir_path = NULL;
    char *prefix = NULL;
    
    printf("Input contains '/': Treating as a path\n");
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
static char **process_non_path_input(char *input)
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
char **fill_autocomplete(char *input)
{
    char **result = NULL;

    if (input == NULL || strchr(input, '/') == NULL) {
        result = process_non_path_input(input);
    } else {
        result = process_path_input(input);
    }
    return result;
}
