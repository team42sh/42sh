/*
** EPITECH PROJECT, 2025
** src/core/helpers/myshrc_helper
** File description:
** This file contains function for handling the 42shrc file
*/

#include "core/minishell.h"

/**
 * @brief Get the path of the shrc.
 *
 * @return The string of the path.
 */
static char *get_shrc_path(void)
{
    char *file = ".42shrc";
    size_t path_len = 0;
    char *path = NULL;

    if (env_search("HOME") == NULL)
        return NULL;
    path_len = my_strlen(env_search("HOME")) + my_strlen(file) + 1;
    path = malloc(sizeof(char) * (path_len + 1));
    if (path == NULL)
        return NULL;
    path[0] = '\0';
    path = my_strcat_list(path, env_search("HOME"), "/", file, NULL);
    return path;
}

/**
 * @brief Execute each lines of the shrc.
 *
 * @param sh_fd         The FD of the file.
 *
 * @return Exit code.
 */
static int execute_lines_shrc(IN int sh_fd)
{
    char buffer[1024];
    size_t buffer_len = 1024;
    size_t len = read_line(sh_fd, buffer, buffer_len);
    exitcode_t exec_command_return = -1;
    token_list_t *list = NULL;

    while (len > 0) {
        remove_newline(buffer);
        if (buffer[len_to_first_char(buffer)] == COMMENT_CHAR) {
            len = read_line(sh_fd, buffer, buffer_len);
            continue;
        }
        list = tokenize_line(buffer);
        exec_command_return = shell_execute(list);
        if (exec_command_return == CURRENTLY_CHILD)
            return CURRENTLY_CHILD;
        len = read_line(sh_fd, buffer, buffer_len);
    }
    return OK_OUTPUT;
}

/**
 * @brief Load the 42shrc.
 *
 * @return The exit code.
 */
int load_myshrc(void)
{
    char *path_shrc = get_shrc_path();
    int sh_fd = -1;

    if (path_shrc == NULL)
        return ERROR_OUTPUT;
    sh_fd = open(path_shrc, O_RDONLY | O_CREAT, 0644);
    free(path_shrc);
    if (sh_fd == -1)
        return ERROR_OUTPUT;
    if (execute_lines_shrc(sh_fd) == CURRENTLY_CHILD) {
        close(sh_fd);
        return CURRENTLY_CHILD;
    }
    close(sh_fd);
    return OK_OUTPUT;
}
