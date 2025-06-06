/*
** EPITECH PROJECT, 2025
** src/commands/setenv
** File description:
** Setenv COMMAND
*/

#include "core/minishell.h"
#include "core/types.h"
#include <stdio.h>

static exitcode_t modify_shell_vars(char *key, char *value)
{
    if (!my_strcmp(key, "PATH")) {
        free_null_check(get_shell()->vars->path_var);
        get_shell()->vars->path_var = my_strdup(value);
        update_var_path();
    }
    return OK_OUTPUT;
}

/**
 * @brief Handle the error, if the setenv is given more that 2 argument
 *
 * @param array the argument of the setenv command
 * @return exitcode_t if there is an error or not
 */
static exitcode_t handle_error(IN char **array)
{
    int array_len = array_count_string(array);

    if (array_len >= 4) {
        print_err("setenv: Too many arguments.\n");
        return ERROR_OUTPUT;
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

    if (argv == NULL || handle_error(argv) == ERROR_OUTPUT)
        return ERROR_OUTPUT;
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
    add_env(key, value);
    modify_shell_vars(key, value);
    return OK_OUTPUT;
}
