/*
** EPITECH PROJECT, 2025
** src/strings/my_strtok
** File description:
** Implementation of the strtok function
*/

#include "core/minishell.h"

/*
 * Count the number of character before the until_char.
 */
static int count_char(char *string, char until_char)
{
    int i = 0;

    while (string[i] != until_char && string[i] != '\0')
        i++;
    return i;
}

/*
 * Return the entire string before the until_char.
 * Example : my_strtok("foo=bar", '=') --> "foo".
 * This function return automatically a strdup function stored in the memory.
 */
char *my_strtok(char *string, char until_char)
{
    static char *last_string = NULL;
    int len_new;
    char *newstr;

    if (string == NULL)
        string = last_string;
    if (string == NULL)
        return NULL;
    len_new = count_char(string, until_char);
    newstr = malloc(sizeof(char) * (len_new + 1));
    if (newstr == NULL)
        return NULL;
    for (int i = 0; i < len_new; i++)
        newstr[i] = string[i];
    newstr[len_new] = '\0';
    if (string[len_new] == '\0' || string[len_new + 1] == '\0')
        last_string = NULL;
    else
        last_string = &string[len_new + 1];
    return newstr;
}
