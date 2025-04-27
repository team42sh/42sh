/*
** EPITECH PROJECT, 2025
** include/core/builtins/history
** File description:
** This file contains all the stuff for the history built-in
*/

#ifndef HISTORY_H_
    #define HISTORY_H_

    #define HISTORY_FILE ".42sh_history"

typedef struct history_args_s {
    int lines_count;
    int clear;
    int reverse;
    int simple;
} history_args_t;

//args_manager.c
history_args_t *history_parse_arguments(IN char **argv);

//file_parse.c
void free_all_lines(char **lines, size_t stored, int limit);
char **parse_history_file(IN char *file_path, int lines_count);

#endif /* ifndef HISTORY_H_ */
