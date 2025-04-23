/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** history
*/

#include "core/minishell.h"
#include "core/builtins.h"

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

/**
 * @brief Parse the history file and prints it.
 *
 * @param file_path
 * @return exitcode_t
 */
static exitcode_t parse_history_file(IN char *file_path)
{
    FILE *file = fopen(file_path, "r");
    char *line = NULL;
    size_t len = 0;

    if (file == NULL)
        return ERROR_OUTPUT;
    while (getline(&line, &len, file) != -1) {
        if (parse_history_line(line) == ERROR_OUTPUT) {
            free(line);
            fclose(file);
            return ERROR_OUTPUT;
        }
    }
    free(line);
    fclose(file);
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

    if (argv[1] == NULL) {
        status = parse_history_file(HISTORY_PATH);
    } else if (my_strcmp(argv[1], "-c") == 0) {
        status = clear_history_file(HISTORY_PATH);
    }
    free(HISTORY_PATH);
    return status;
}
