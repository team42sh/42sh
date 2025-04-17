/*
** EPITECH PROJECT, 2025
** prompt_helper.c
** File description:
** This file contains functions for displaying a prompt on the screen
*/

#include "core/minishell.h"

void print_github_repository(void)
{
    my_printf(" \033[32mgit:(\033[31m%s\033[32m)",
        get_shell()->vars->github_repository);
}

/*
 * This function show the shell prompt. No more to say about this.
 * Actual prompt : "⇢ <pwd> $ "
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
    my_printf("\033[1;32m➜  \033[1;34m%s \033[1;35m$", pwd);
    if (is_in_github_repository() == true)
        print_github_repository();
    my_printf("\033[0m ");
}

/**
 * @brief Calculate the total length of the shell prompt.
 *
 * @return The length of the prompt, including dynamic content like PWD
 *         and GitHub repository info.
 */
int get_len_prompt(void)
{
    char *pwd = get_shell()->vars->pwd_var;
    char *home = get_shell()->vars->home_var;
    int len = 0;

    len += 5;
    if (home != NULL && my_strcmp(pwd, home) == 0) {
        len += 1;
    } else if (my_strcmp(pwd, "/") != 0) {
        len += my_strlen(my_strtok_reverse(pwd, '/'));
    } else {
        len += 1;
    }
    if (is_in_github_repository() == true) {
        len += 5;
        len += my_strlen(get_shell()->vars->github_repository);
        len += 2;
    }
    return len;
}
