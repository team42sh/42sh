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

static bool is_binary_existing(char full_path[4096], char *dir, char *command)
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

static exitcode_t find_binary_in_paths(char *path_var, char *command)
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

static exitcode_t check_path(char *arg)
{
    char *path_var = get_shell()->vars->path_var;

    if (arg == NULL || path_var == NULL)
        return ERROR_OUTPUT;
    return find_binary_in_paths(path_var, arg);
}

static exitcode_t check_builtin(char *arg)
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

static exitcode_t check_alias(char *arg)
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

static exitcode_t error_handling(char **argv)
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
