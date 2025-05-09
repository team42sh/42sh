/*
** EPITECH PROJECT, 2025
** src/core/helpers/history_helper
** File description:
** This file contains functions for helping the management of history
*/

#include "core/minishell.h"
#include "core/builtins/history.h"
#include <stdlib.h>
#include <unistd.h>

/*
 * Get the current time and return it as a string.
 * The format is HH:MM.
 * The string is allocated with malloc, so there is a need to free it.
 */
static char *get_str_time(void)
{
    time_t rawtime;
    struct tm * timeinfo;
    char *time_str = malloc(sizeof(char) * 6);

    if (!time_str)
        return NULL;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    snprintf(time_str, 6, "%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min);
    return time_str;
}

/*
 * Get the home env path and return the path of the mysh_history.
 * Where it should be.
 */
char *get_sh_history_path(void)
{
    size_t path_len = 0;
    char *path = NULL;

    if (env_search("HOME") == NULL)
        return NULL;
    path_len = my_strlen(env_search("HOME")) + my_strlen(HISTORY_FILE) + 1;
    path = malloc(sizeof(char) * (path_len + 1));
    if (path == NULL)
        return NULL;
    path[0] = '\0';
    path = my_strcat_list(path, env_search("HOME"), "/", HISTORY_FILE, NULL);
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
    char *time_str = get_str_time();

    get_shell()->vars->history_lines_count = count_number_lines_history();
    if (sh_fd == -1 || !command || !time_str || command[0] == '\0') {
        free(time_str);
        if (sh_fd != 1)
            close(sh_fd);
        return ERROR_OUTPUT;
    }
    dprintf(sh_fd, "%d;%s;%s\n", get_shell()->vars->history_lines_count,
        time_str, command);
    free(time_str);
    close(sh_fd);
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
