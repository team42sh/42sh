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
 * @brief Count all lines in the lines array.
 *
 * @param lines The array of lines to count.
 * @return int The number of lines in the array.
 * @note The lines array must be NULL terminated.
 */
static int count_lines(char **lines)
{
    int count = 0;

    for (int i = 0; lines[i] != NULL; i++)
        count++;
    return count;
}

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
static exitcode_t parse_history_line(IN char *line, IN int simple)
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
    if (simple)
        printf("%s\n", command_str);
    else
        printf("%5s\t%s\t%s\n", str_index, str_time, command_str);
    return OK_OUTPUT;
}

/**
 * @brief Display the history file.
 *
 * @param file_path
 * @param history_args
 * @return exitcode_t
 */
static exitcode_t display_history(IN char *file_path,
    history_args_t *history_args)
{
    char **lines = NULL;

    if (history_args->lines_count <= 0 && history_args->lines_count != -1) {
        my_printf("history: Invalid number of lines.\n");
        return ERROR_OUTPUT;
    }
    lines = parse_history_file(file_path, history_args->lines_count);
    if (!lines) {
        my_printf("history: Failed to read history file.\n");
        return ERROR_OUTPUT;
    }
    if (history_args->reverse)
        for (int i = count_lines(lines) - 1; i >= 0; i--)
            parse_history_line(lines[i], history_args->simple);
    else
        for (int i = 0; lines[i] != NULL; i++)
            parse_history_line(lines[i], history_args->simple);
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
