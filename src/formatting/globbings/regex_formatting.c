/*
** EPITECH PROJECT, 2025
** src/formatting/globbings/regex_formatting
** File description:
** file where the functions related to regex path formatting are processed
*/

#include "core/minishell.h"

/**
 * @brief Count occurrences of a character in a string
 *
 * @param str String to search in
 * @param c Character to count
 * @return int Number of occurrences
 */
int count_char_occurrences(IN char *str, IN char c)
{
    int occurrences = 0;

    if (!str)
        return 0;
    for (int i = 0; str[i] != '\0'; i++)
        if (str[i] == c)
            occurrences++;
    return occurrences;
}

/**
 * @brief Find index of a character in a string until delimiter
 *
 * @param str String to search in
 * @param c Character to find
 * @param delim Delimiter to stop search at
 * @return int Index of character or -1 if not found
 */
int char_in_str_until_delim(IN char *str, IN char c, IN char delim)
{
    if (!str)
        return -1;
    for (int i = 0; str[i] != '\0' && str[i] != delim; i++)
        if (str[i] == c)
            return i;
    return -1;
}

/**
 * @brief Check if a string contains any globbing characters
 *
 * @param str String to check
 * @return int 1 if globbing chars present, 0 otherwise
 */
int has_globbing_chars(IN char *str)
{
    if (!str)
        return 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (char_in_str(GLOBBING_CHARS, str[i]) != -1)
            return 1;
    }
    return 0;
}

/**
 * @brief Count extra characters needed for regex conversion
 *
 * @param pattern Shell pattern with globbing chars
 * @return int Number of extra characters needed
 */
static int count_extra_chars(IN char *pattern)
{
    int extra_chars = 0;
    int i = 0;

    while (pattern[i] != '\0') {
        if (char_in_str(REGEX_SPECIAL_CHARS, pattern[i]) != -1)
            extra_chars++;
        if (pattern[i] == '*')
            extra_chars++;
        i++;
    }
    return extra_chars;
}

/**
 * @brief Process a single character from pattern to regex
 *
 * @param pattern Original pattern
 * @param regex Target regex string
 * @param i Index in pattern
 * @param j_ptr Pointer to index in regex
 */
static void process_char(IN char *pattern, OUT char *regex, IN int i,
    OUT int *j)
{
    if (char_in_str(REGEX_SPECIAL_CHARS, pattern[i]) != -1) {
        regex[*j] = '\\';
        (*j)++;
    }
    if (pattern[i] == '*') {
        regex[*j] = '.';
        regex[*j + 1] = '*';
        *j += 2;
        return;
    }
    if (pattern[i] == '?') {
        regex[*j] = '.';
        (*j)++;
        return;
    }
    regex[*j] = pattern[i];
    (*j)++;
}

/**
 * @brief Convert a shell globbing pattern to a regex pattern
 *
 * @param pattern Shell pattern with globbing chars
 * @return char* Equivalent regex pattern (caller must free)
 */
char *pattern_to_regex(IN char *pattern)
{
    int extra_chars = 0;
    char *regex = NULL;
    int j = 0;
    int i = 0;

    if (!pattern)
        return NULL;
    extra_chars = count_extra_chars(pattern);
    regex = malloc(sizeof(char) * (my_strlen(pattern) + extra_chars + 3));
    regex[j] = '^';
    j++;
    while (pattern[i] != '\0') {
        process_char(pattern, regex, i, &j);
        i++;
    }
    regex[j] = '$';
    regex[j + 1] = '\0';
    return regex;
}
