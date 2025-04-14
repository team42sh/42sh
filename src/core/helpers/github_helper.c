/*
** EPITECH PROJECT, 2025
** src/core/helpers/github_helper
** File description:
** This file contains functions for helping with github features
*/

#include "core/minishell.h"

bool is_in_github_repository(void)
{
    shell_t *shell = get_shell();

    if (shell->vars->github_repository == NULL)
        return false;
    return true;
}

char *get_github_repository_name(void)
{
    int head_fd = open(".git/HEAD", O_RDONLY);
    char buffer[1024] = {0};
    char *head = NULL;

    if (head_fd == -1)
        return NULL;
    if (read_line(head_fd, buffer, 1024) > 0) {
        remove_newline(buffer);
        head = my_strtok_reverse(buffer, '/');
        close(head_fd);
        return my_strdup(head);
    }
    close(head_fd);
    return NULL;
}
