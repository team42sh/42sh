/*
** EPITECH PROJECT, 2025
** src/utilities/strings/str_ascii
** File description:
** This file contains some functions related of
** the ascii of a character or string
*/

#include "core/minishell.h"
#include <stdbool.h>

/*
 * Return true or false if the char c passed in parameter in a letter or not.
 */
bool is_letter(char c)
{
    if (c >= 'a' && c <= 'z')
        return 1;
    if (c >= 'A' && c <= 'Z')
        return 1;
    return 0;
}

/*
 * This function return true or false if the string is containing only numbers.
 * This function take + and - as numbers.
 */
bool is_only_numbers(char *string)
{
    char *str = string;

    if (string == NULL)
        return false;
    while (*str) {
        if ((*str < '0' || *str > '9') && *str != '+' && *str != '-')
            return false;
        str++;
    }
    return true;
}

/*
 * This function return true of false if the string is containing only alpha
 * and numeric characters.
 */
bool is_alpha_num(char *string)
{
    char *str = string;

    if (str == NULL)
        return false;
    while (*str) {
        if ((*str >= 'a' && *str <= 'z')
            || (*str >= 'A' && *str <= 'Z')
            || (*str >= '0' && *str <= '9')
            || (*str == '_')) {
            str++;
            continue;
        }
        return false;
    }
    return true;
}

/*
 * Calculate the index, to get the first char.
 * Not chars are space and tabs.
 */
int len_to_first_char(char *str)
{
    int i = 0;

    if (str == NULL)
        return -1;
    while (str[i] == ' ' || str[i] == '\t') {
        i++;
    }
    return i;
}
