/*
** EPITECH PROJECT, 2025
** src/core/shell_variables
** File description:
** This file contains all the logic for shell variables
*/

#include "core/minishell.h"

/*
 * Create the structure variables and init all usefull variables.
 * For the moment the only variables stocked are :
 * - HOME
 * - PWD
 * - OLDPWD
 */
shell_variables_t *create_shell_vars(void)
{
    shell_variables_t *vars = malloc(sizeof(shell_variables_t));

    if (env_search("HOME") == NULL)
        vars->home_var = NULL;
    else
        vars->home_var = my_strdup(env_search("HOME"));
    vars->pwd_var = getcwd(NULL, 0);
    vars->oldpwd_var = NULL;
    if (env_search("PATH") == NULL)
        vars->path_var = my_strdup("/usr/bin:/bin");
    else
        vars->path_var = my_strdup(env_search("PATH"));
    add_path_variable(vars);
    vars->history_lines_count = count_number_lines_history();
    vars->github_repository = get_github_repository_name();
    return vars;
}

/*
 * Free all variables on the shell variables structure.
 * Then free the structure of the shell.
 */
void free_shell_vars(void)
{
    shell_variables_t *vars = get_shell()->vars;

    free_null_check(vars->pwd_var);
    free_null_check(vars->home_var);
    free_null_check(vars->oldpwd_var);
    free_null_check(vars->path_var);
    free_null_check(vars->github_repository);
    free(vars);
}

/*
 * This function is a DEBUG function used to print the content of the shell
 * variables.
 */
void print_shells_vars(void)
{
    shell_variables_t *vars = get_shell()->vars;

    my_printf("\033[1;39m=======================\033[0m\n");
    if (vars->home_var != NULL)
        my_printf("HOME = \033[1:33m%s\033[0m\n", vars->home_var);
    if (vars->pwd_var != NULL)
        my_printf("PWD = \033[1:33m%s\033[0m\n", vars->pwd_var);
    if (vars->oldpwd_var != NULL)
        my_printf("OLDPWD = \033[1:33m%s\033[0m\n", vars->oldpwd_var);
    if (vars->path_var != NULL)
        my_printf("PATH = \033[1:33m%s\033[0m\n", vars->path_var);
    my_printf("\033[1;39m=======================\033[0m\n");
}
