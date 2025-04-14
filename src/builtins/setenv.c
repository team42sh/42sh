/*
** EPITECH PROJECT, 2025
** src/commands/setenv
** File description:
** Setenv COMMAND
*/

#include "../../include/core/minishell.h"

static exitcode_t modify_shell_vars(char *key, char *value)
{
    if (!my_strcmp(key, "PATH")) {
        free_null_check(get_shell()->vars->path_var);
        get_shell()->vars->path_var = my_strdup(value);
    }
    return OK_OUTPUT;
}

/*
 * SETENV COMMAND - BUILT-IN
 * This command will add or modify an environment variable using their key
 * and value.
 */
exitcode_t setenv_command(char **argv)
{
    char *key;
    char *value;

    if (argv[1] == NULL)
        return env_command(argv);
    if (!is_letter(argv[1][0]) && argv[1][0] != '_')
        return print_err("setenv: Variable name must begin with a letter.\n");
    if (!is_alpha_num(argv[1]))
        return print_err("setenv: Variable name must contain"
            " alphanumeric characters.\n");
    key = my_strdup(argv[1]);
    if (argv[2] == NULL)
        value = my_strdup("");
    else
        value = my_strdup(argv[2]);
    modify_shell_vars(key, value);
    add_env(key, value);
    return OK_OUTPUT;
}
