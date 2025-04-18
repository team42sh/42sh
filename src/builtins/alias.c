/*
** EPITECH PROJECT, 2025
** src/builtins/alias.c
** File description:
** ALIAS COMMAND
*/

#include "core/minishell.h"

/*
 * This function is when you execute alias command without argument.
 * You just show every aliases with original and \t and alias.
 */
static exitcode_t show_aliases(void)
{
    alias_t *aliases = get_shell()->aliases;

    while (aliases != NULL) {
        my_printf("%s\t(%s)\n", aliases->original_string,
            aliases->alias_string);
        aliases = aliases->next;
    }
    return OK_OUTPUT;
}

/*
 * This function is when you execute alias command with only 1 argument.
 * You just show the content of the alias passed as parameter.
 */
static exitcode_t show_one_alias(char *alias_name)
{
    alias_t *aliases = get_shell()->aliases;

    while (aliases != NULL) {
        if (!my_strcmp(alias_name, aliases->original_string)) {
            my_printf("%s\n", aliases->alias_string);
            break;
        }
        aliases = aliases->next;
    }
    return OK_OUTPUT;
}

/*
 * ALIAS COMMAND - BUILT-IN
 * No args = Show all aliases
 * 1 arg = Show the alias with the name passed
 * 2 args = Create the alias with the values
 * 3+ args = Error message.
 */
exitcode_t alias_command(char **argv)
{
    if (argv == NULL)
        return ERROR_OUTPUT;
    if (argv[1] == NULL)
        return show_aliases();
    if (argv[1] != NULL && argv[2] == NULL)
        return show_one_alias(argv[1]);
    add_alias(argv[1], &argv[2]);
    return OK_OUTPUT;
}
