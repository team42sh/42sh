/*
** EPITECH PROJECT, 2025
** prompt_helper.c
** File description:
** This file contains functions for displaying a prompt on the screen
*/

#include "core/minishell.h"

/**
 * @brief Print the repository GitHub
 */
void print_github_repository(void)
{
    my_printf(" \033[32mgit:(\033[31m%s\033[32m)",
        get_shell()->vars->github_repository);
}

/**
 * @brief Print the shell prompt, priting the PWD and github repo if there is
 *        one.
 */
void print_shell_prompt(void)
{
    char *pwd = get_shell()->vars->pwd_var;
    char *home = get_shell()->vars->home_var;

    if (home != NULL && my_strcmp(pwd, home) == 0)
        pwd = "~";
    else if (my_strcmp(pwd, "/") != 0) {
        pwd = my_strtok_reverse(pwd, '/');
    }
    my_printf("\033[1;32m-  \033[1;34m%s \033[1;35m$", pwd);
    if (is_in_github_repository() == true)
        print_github_repository();
    my_printf("\033[0m ");
}
