/*
** EPITECH PROJECT, 2025
** src/formatting/globbings/regex_init
** File description:
** file to initialise some functions
*/

#include "core/minishell.h"

/**
 * @brief Init and compile regex for pattern
 *
 * @param pattern Pattern to compile
 * @param regex_ptr Pointer to regex_t
 * @return char* Regex pattern string or NULL
 */
char *init_regex(IN char *pattern, IN regex_t *regex)
{
    char *regex_pattern = NULL;

    regex_pattern = pattern_to_regex(pattern);
    if (!regex_pattern)
        return NULL;
    if (regcomp(regex, regex_pattern, REG_EXTENDED) != 0) {
        free(regex_pattern);
        return NULL;
    }
    return regex_pattern;
}

/**
 * @brief Initialize matches array
 *
 * @return char** New matches array or NULL
 */
char **init_matches(void)
{
    char **matches = NULL;

    matches = malloc(sizeof(char *) * 10);
    if (!matches)
        return NULL;
    return matches;
}
