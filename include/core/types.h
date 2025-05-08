/*
** EPITECH PROJECT, 2025
** include/types
** File description:
** This header file include all custom types used in the project
*/

#ifndef TYPES_H
    #define TYPES_H

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

    #define POS_X 1
    #define POS_Y 0

    #define MAX_WORDS_COUNT 2048
    #define MAX_WORD_LEN 2048
    #define BUFFER_TERMIOS_SIZE 4096

    #define CTRL_D_VALUE 4
    #define CTRL_A_VALUE '\x01'
    #define CTRL_E_VALUE '\x05'
    #define CTRL_H_VALUE '\x08'
    #define CTRL_K_VALUE '\x0B'
    #define CTRL_Y_VALUE '\x19'
    #define BACKSPACE_VALUE 127

    #define ENTER_CHAR '\n'
    #define AUTO_COMPLETE_CHAR '\t'
    #define ESCAPE_CHAR '\e'
    #define COMMENT_CHAR '#'
    #define NOT_INHIBITED "\"*|<>()=[]"

    #define REGEX_SPECIAL_CHARS "./^$+()[]{}"
    #define GLOBBING_CHARS "*?"

/*
 * Types used in the shell to facilitize the comprenssion of the code.
 */
typedef int exitcode_t;
typedef int signal_t;
typedef int fd_t;

#endif /* ifndef TYPES_H */
