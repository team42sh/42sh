/*
** EPITECH PROJECT, 2025
** src/core/helpers/history_helper
** File description:
** This file contains functions for helping the management of history
*/

#include "core/minishell.h"

/*
 * Get the home env path and return the path of the mysh_history.
 * Where it should be.
 */
static char *get_sh_history_path(void)
{
    char *file = ".mysh_history";
    size_t path_len = 0;
    char *path = NULL;

    if (env_search("HOME") == NULL)
        return false;
    path_len = my_strlen(env_search("HOME")) + my_strlen(file) + 1;
    path = malloc(sizeof(char) * (path_len + 1));
    if (path == NULL)
        return NULL;
    path[0] = '\0';
    path = my_strcat_list(path, env_search("HOME"), "/", file, NULL);
    return path;
}

/*
 * This file will create the ~/.mysh_history
 * Located in the home of the user.
 * If the home is not specified then no history.
 * This do not close the file !! If you use this function please close(sh_fd).
 */
int open_history_file(void)
{
    char *path_history = get_sh_history_path();
    int sh_fd = -1;

    if (path_history == NULL)
        return sh_fd;
    sh_fd = open(path_history, O_CREAT | O_WRONLY | O_APPEND, 0644);
    free(path_history);
    if (sh_fd == -1)
        return sh_fd;
    return sh_fd;
}

/*
 * Write the command in the ~/.mysh_history file.
 * Write the command at the end of the file. And create the file if it doesn't
 * exist.
 */
int write_command_history(char *command)
{
    int sh_fd = open_history_file();
    char *line_number_str = NULL;

    if (sh_fd == -1 || command == NULL || command[0] == '\0')
        return ERROR_OUTPUT;
    line_number_str = my_itoa(get_shell()->vars->history_lines_count);
    write(sh_fd, line_number_str, my_strlen(line_number_str));
    write(sh_fd, " : ", 3);
    write(sh_fd, command, my_strlen(command));
    write(sh_fd, "\n", 1);
    close(sh_fd);
    get_shell()->vars->history_lines_count++;
    free(line_number_str);
    return OK_OUTPUT;
}

/*
 * Count the number of lines in the history file.
 * This is done 1 time as setup in the main function.
 */
int count_number_lines_history(void)
{
    char *path_file = get_sh_history_path();
    int sh_fd = -1;
    char buffer[1024];
    size_t buffer_len = 1024;
    size_t len = 0;
    int count = 0;

    if (path_file != NULL)
        sh_fd = open(path_file, O_RDONLY);
    free(path_file);
    if (sh_fd == -1)
        return 1;
    len = read_line(sh_fd, buffer, buffer_len);
    while (len > 0) {
        count++;
        len = read_line(sh_fd, buffer, buffer_len);
    }
    close(sh_fd);
    return count;
}
