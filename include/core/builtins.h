/*
** EPITECH PROJECT, 2025
** include/core/builtins
** File description:
** This file contains all the stuff for handling builtins
*/

#ifndef BUILTINS_H
    #define BUILTINS_H

    #include "types.h"

/*
 * ALL COMMANDS AVAILABLES BUILT-IN
 * - exit
 * - cd
 * - setenv
 * - unsetenv
 * - env
 */
exitcode_t exit_command(char **argv);
exitcode_t env_command(char **argv __attribute__((unused)));
exitcode_t set_command(char **argv);
exitcode_t setenv_command(char **argv);
exitcode_t unsetenv_command(char **argv);
exitcode_t cd_command(char **argv);
exitcode_t alias_command(char **argv);
exitcode_t unalias_command(char **argv);

/*
 * BUILTS IN STRUCTURE
 */
typedef struct builtin_s {
    char *command_name;
    int (*exec_command)(char **argv);
} builtin_t;

/*
 * Utils functions for builtins
 */
int is_builtin(char **argv);

/*
 * DECLARATE IN ./src/main.c
 */
extern const builtin_t BUILTINS[];

#endif /* ifndef BUILTINS_H */
