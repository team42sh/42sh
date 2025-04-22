/*
** EPITECH PROJECT, 2025
** where.c
** File description:
** where builtin function
*/

#include "core/builtins.h"
#include "core/minishell.h"
#include "core/types.h"
#include "macros/misc_macros.h"
#include "my_printf.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

/**
 * @brief Check if the binary exist, and print its path.
 *
 * @param full_path The full path.
 * @param dir The directory we are checking.
 * @param command the command.
 * @return true if the binary exist.
 * @return false if the binary doesn't exist.
 */
static bool is_binary_existing(OUT char full_path[4096],
    IN char *dir, IN char *command)
{
    my_strcpy(full_path, dir);
    if (full_path[my_strlen(full_path) - 1] == '/')
        my_strcat_list(full_path, command, NULL);
    else
        my_strcat_list(full_path, "/", command, NULL);
    if (!access(full_path, X_OK)) {
        my_printf("%s\n", full_path);
        return true;
    }
    return false;
}

/**
 * @brief search the command in all the path in the PATH env.
 *
 * @param path_var The path variable.
 * @param command The command to check.
 * @return exitcode_t if no function was found ERROR, OK otherwise.
 */
static exitcode_t find_binary_in_paths(IN char *path_var, IN char *command)
{
    char *path_copy = my_strdup(path_var);
    char *dir = my_strtok(path_copy, ':');
    exitcode_t result = ERROR_OUTPUT;
    char full_path[4096];

    while (dir) {
        if (is_binary_existing(full_path, dir, command))
            result = OK_OUTPUT;
        free_null_check(dir);
        dir = my_strtok(NULL, ':');
        if (dir == NULL)
            break;
    }
    free(path_copy);
    return result;
}

/**
 * @brief It check if the argument is an binary in the path.
 * If so, print the absolute path to the stdout.
 *
 * @param arg The argument.
 * @return exitcode_t If it is a function in the path.
 */
static exitcode_t check_path(IN char *arg)
{
    char *path_var = get_shell()->vars->path_var;

    if (arg == NULL || path_var == NULL || arg[0] == '\0')
        return ERROR_OUTPUT;
    return find_binary_in_paths(path_var, arg);
}

/**
 * @brief The function check if the argument is an builtin or not.
 * If so, it print it to the stdout.
 *
 * @param arg The argument.
 * @return exitcode_t If it is an builtin or not.
 */
static exitcode_t check_builtin(IN char *arg)
{
    exitcode_t return_status = ERROR_OUTPUT;

    if (arg == NULL)
        return ERROR_OUTPUT;
    for (int i = 0; BUILTINS[i].command_name != NULL; i++) {
        if (my_strcmp(BUILTINS[i].command_name, arg) == 0) {
            my_printf("%s is a shell built-in\n", arg);
            return_status = OK_OUTPUT;
        }
    }
    return return_status;
}

/**
 * @brief The function check if the argument is an alias or not.
 * If so, it print it to the stdout.
 *
 * @param arg The argument.
 * @return exitcode_t If it is an alias or not.
 */
static exitcode_t check_alias(IN char *arg)
{
    alias_t *alias = get_shell()->aliases;
    exitcode_t return_status = ERROR_OUTPUT;

    if (arg == NULL)
        return ERROR_OUTPUT;
    while (alias != NULL) {
        if (my_strcmp(arg, alias->original_string) == 0) {
            my_printf("%s is aliased to %s\n", alias->original_string,
                alias->alias_string);
            return_status = OK_OUTPUT;
        }
        alias = alias->next;
    }
    return return_status;
}

/**
 * @brief Loop to do all the check for each arguments given.
 *
 * @param argv The arguments.
 * @return exitcode_t OK if all is good and ERROR if one argument has given
 * no result.
 */
static exitcode_t check_loop(IN char **argv)
{
    exitcode_t return_status = OK_OUTPUT;
    int error = 0;

    while (*argv != NULL) {
        error = 0;
        error = (check_alias(*argv) == ERROR_OUTPUT) ? (error + 1) : (error);
        error = (check_builtin(*argv) == ERROR_OUTPUT) ? (error + 1) : (error);
        error = (check_path(*argv) == ERROR_OUTPUT) ? (error + 1) : (error);
        if (error == 3)
            return_status = ERROR_OUTPUT;
        argv++;
    }
    return return_status;
}

/**
 * @brief Handle the error before doing the actual function.
 *
 * @param argv The arguments.
 * @return exitcode_t Status if there is an error or not.
 */
static exitcode_t error_handling(IN char **argv)
{
    while (*argv != NULL) {
        if (strstr(*argv, "/") != NULL) {
            dprintf(2, "where: / in command makes no sense\n");
            return ERROR_OUTPUT;
        }
        argv++;
    }
    return OK_OUTPUT;
}

/**
 * @brief The where function that find occurence of the command.
 *
 * @param argv The arguments.
 * @return exitcode_t The status.
 */
exitcode_t where_function(IN char **argv)
{
    if (argv == NULL)
        return ERROR_OUTPUT;
    if (argv[0] == NULL || argv[1] == NULL) {
        dprintf(2, "where: Too few arguments.\n");
        return ERROR_OUTPUT;
    }
    if (error_handling(&argv[1]) == ERROR_OUTPUT)
        return ERROR_OUTPUT;
    return check_loop(&argv[1]);
}
