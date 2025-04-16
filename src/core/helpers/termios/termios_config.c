/*
** EPITECH PROJECT, 2025
** src/core/helpers/termios/termios_config
** File description:
** This file contains functions for termios config
*/

#include "core/minishell.h"

/**
 * @brief Enable the row mode for the termios configuration
 *        That means : no echo, no escape characters, no waiting for \n.
 *
 * @param shell         The shell structure
 */
void enable_raw_mode(OUT shell_t *shell)
{
    if (shell == NULL)
        return;
    shell->_term_info->_current_termios.c_lflag &= ~(ECHO | ICANON);
    shell->_term_info->_current_termios.c_cc[VMIN] = 1;
    shell->_term_info->_current_termios.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &shell->_term_info->_current_termios);
}

/**
 * @brief Setup the shell termios information.
 *
 * @return The final structure with all values initialized.
 */
term_info_t *setup_shell_term_info(void)
{
    term_info_t *term_info = malloc(sizeof(term_info_t));

    if (term_info == NULL)
        return NULL;
    term_info->_cursor_index = 0;
    term_info->_buffer_len = 0;
    term_info->_sig_buffer_reset = false;
    for (int i = 0; i < BUFFER_TERMIOS_SIZE; i++)
        term_info->_buffer[i] = 0;
    term_info->_history_index = 0;
    return term_info;
}

/**
 * @brief Init termios is a function for putting the right terminal
 *        configuration after entering a command, that is useful because
 *        termios can break sometime if spamming some weird stuff.
 *
 */
void init_termios(void)
{
    shell_t *shell = get_shell();
    struct termios original_termios = {0};

    tcgetattr(STDIN_FILENO, &original_termios);
    shell->_term_info->_original_termios = original_termios;
    shell->_term_info->_current_termios = original_termios;
    enable_raw_mode(shell);
}
