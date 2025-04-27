/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** history
*/

#include "core/minishell.h"
#include "core/builtins.h"
#include "core/builtins/history.h"

/**
 * @brief Get the home env path and return the path of the mysh_history.
 *
 * @param file_path
 * @return exitcode_t
 */
static exitcode_t clear_history_file(IN char *file_path)
{
    FILE *file = fopen(file_path, "w");

    if (file == NULL)
        return ERROR_OUTPUT;
    fclose(file);
    return OK_OUTPUT;
}

/**
 * @brief Parse a line of the history file and prints it.
 *
 * @param line
 * @return exitcode_t
 */
static exitcode_t parse_history_line(IN char *line)
{
    char str_index[10] = {0};
    char str_time[6] = {0};
    char command_str[1024] = {0};
    int result = sscanf(line, "%[^;];%[^;];%[^\n]", str_index, str_time,
        command_str);

    if (result != 3) {
        my_printf("Error: Invalid history line format\n");
        return ERROR_OUTPUT;
    }
    printf("%5s  %s   %s\n", str_index, str_time, command_str);
    return OK_OUTPUT;
}

static exitcode_t display_history(IN char *file_path,
    history_args_t *history_args)
{
    char **lines = parse_history_file(file_path, history_args->lines_count);
    (void)(lines);
    (void)(parse_history_line);
    printf("Displaying history...\n");
    printf("Lines count: %d\n", history_args->lines_count);
    printf("Clear: %d\n", history_args->clear);
    printf("Reverse: %d\n", history_args->reverse);
    printf("Simple: %d\n", history_args->simple);

    return OK_OUTPUT;
}

/**
 * @brief This function is the built-in history command.
 * It can be used with no arguments to show the history or with -c to clear it.
 *
 * @param argv
 * @return exitcode_t
 */
exitcode_t history_command(IN char **argv)
{
    char *HISTORY_PATH = get_sh_history_path();
    exitcode_t status = ERROR_OUTPUT;
    history_args_t *history_args = history_parse_arguments(argv);

    if (history_args == NULL) {
        free(HISTORY_PATH);
        return ERROR_OUTPUT;
    }
    if (history_args->clear) {
        status = clear_history_file(HISTORY_PATH);
    } else {
        status = display_history(HISTORY_PATH, history_args);
    }
    free(HISTORY_PATH);
    free(history_args);
    return status;
}
