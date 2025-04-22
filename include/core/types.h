/*
** EPITECH PROJECT, 2025
** include/types
** File description:
** This header file include all custom types used in the project
*/

#ifndef TYPES_H
    #define TYPES_H

/*
 * Defines for code clarity, IN for value entered as parameter and not
 * Modidified, OUT if thhe variable is modified in the function
 */
    #define IN
    #define OUT
/*
 * Some define use to not use magic number in code.
 * Put your defines here to make it more clean.
 */
    #define CURRENTLY_CHILD -1
    #define COMMAND_ERROR -2
    #define ERROR_OUTPUT 1
    #define OK_OUTPUT 0

    #define PIPE_WRITE 1
    #define PIPE_READ 0

    #define LEFT 0
    #define RIGHT 1

    #define MAX_WORDS_COUNT 2048
    #define MAX_WORD_LEN 2048
    #define BUFFER_TERMIOS_SIZE 4096

    #define CTRL_D_VALUE 4
    #define CTRL_A_VALUE '\x01'
    #define CTRL_E_VALUE '\x05'
    #define BACKSPACE_VALUE 127
    #define COMMENT_CHAR '#'

/*
 * Types used in the shell to facilitize the comprenssion of the code.
 */
typedef int exitcode_t;
typedef int signal_t;
typedef int fd_t;

#endif /* ifndef TYPES_H */
