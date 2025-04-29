/*
** EPITECH PROJECT, 2025
** src/core/helpers/autocompletion/autocompletion_utils
** File description:
** Utilitary functions for the autocompletion handling
*/

#include "core/minishell.h"

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
 * @brief Check if a string is in the tab
 *
 * @param tab
 * @param str
 * @return int
 */
int str_in_tab(IN char **tab, IN char *str)
{
    if (tab == NULL || str == NULL)
        return 0;
    for (int i = 0; tab[i] != NULL; i++)
        if (my_strcmp(tab[i], str) == 0)
            return 1;
    return 0;
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

    len1 = str1 ? my_strlen(str1) : 0;
    len2 = str2 ? my_strlen(str2) : 0;
    result = malloc(sizeof(char) * (len1 + len2 + 1));
    if (!result)
        return NULL;
    if (str1)
        my_strcpy(result, str1);
    else
        result[0] = '\0';
    if (str2)
        my_strcat(result, str2);
    return result;
}

/**
 * @brief Check if a file should be displayed
 *        (exclude hidden files and special entries)
 *
 * @param name
 * @return int
 */
int is_visible_file(IN char *name)
{
    if (name == NULL || strcmp(name, ".") == 0 || strcmp(name, "..") == 0) {
        return 0;
    }
    return (name[0] != '.');
}

/**
 * @brief Count words in a string based on a delimiter
 *
 * @param str
 * @param delimiter
 * @return int
 */
static int count_words(IN char *str, IN char delimiter)
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
static char *extract_word(IN char *str, IN int start, IN int end)
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
    int i = 0;
    int word_index = 0;
    int args[3] = {0, 0, 0};

    if (str == NULL)
        return NULL;
    args[0] = count_words(str, delimiter);
    result = malloc(sizeof(char *) * (args[0] + 1));
    for (i = 0; str[i] != '\0'; i++) {
        if (str[i] == delimiter || str[i + 1] == '\0') {
            args[2] = (str[i] == delimiter ? i : i + 1);
            result[word_index] = extract_word(str, args[1], args[2]);
            word_index++;
            args[1] = i + 1;
        }
    }
    result[args[0]] = NULL;
    return result;
}
