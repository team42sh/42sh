/*
** EPITECH PROJECT, 2025
** src/core/helpers/termios/termios_config
** File description:
** This file contains functions for termios config
*/

#include "core/minishell.h"

static void enable_raw_mode(shell_t *shell)
{
    shell->_current_termios.c_lflag &= ~(ECHO | ICANON);
    shell->_current_termios.c_cc[VMIN] = 1;
    shell->_current_termios.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &shell->_current_termios);
}

void init_termios(void)
{
    shell_t *shell = get_shell();
    struct termios original_termios = {0};

    tcgetattr(STDIN_FILENO, &original_termios);
    shell->_original_termios = original_termios;
    shell->_current_termios = original_termios;
    enable_raw_mode(shell);
}
