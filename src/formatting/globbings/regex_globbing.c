/*
** EPITECH PROJECT, 2025
** src/formatting/globbings/regex_globbing
** File description:
** test file for regex globbing implementation
*/

#include "core/minishell.h"
#include <regex.h>

/**
 * This function is for testing if regex is a banned function, it does nothing.
 */
void dummy_regex_test(void)
{
    regex_t regex;
    char error_buffer[128];
    int result;

    result = regcomp(&regex, ".*", REG_EXTENDED);
    result = regexec(&regex, "test", 0, NULL, 0);
    regerror(result, &regex, error_buffer, sizeof(error_buffer));
    regfree(&regex);
}
