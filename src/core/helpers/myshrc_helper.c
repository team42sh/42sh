/*
** EPITECH PROJECT, 2025
** src/core/helpers/myshrc_helper
** File description:
** This file contains function for handling the myshrc file
*/

#include "../../../include/core/minishell.h"
#include <sys/fcntl.h>

/*
 * Get the home env path and return the path of the myshrc, where it should be.
 */
static char *get_shrc_path(void)
{
    char *file = ".myshrc";
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
 * Execute every lines in the myshrc file.
 * Pass the lines starting with '#' which is identified of comments.
 */
static int execute_lines_shrc(int sh_fd)
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

/*
 * Load the file myshrc.
 * The file needs to be in the home folder in the ENV variable.
 * TODO: Use getuid to know the starter and check in passwd. (Banned function)
 */
int load_myshrc(void)
{
    char *path_shrc = get_shrc_path();
    int sh_fd = -1;

    if (path_shrc == NULL)
        return ERROR_OUTPUT;
    sh_fd = open(path_shrc, O_RDONLY);
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
