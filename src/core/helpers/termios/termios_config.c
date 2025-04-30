/*
** EPITECH PROJECT, 2025
** src/core/helpers/termios/termios_config
** File description:
** This file contains functions for termios config
*/

#include "core/minishell.h"

/**
 * @brief Setup the shell termios information.
 *
 * @return The final structure with all values initialized.
 */
term_info_t *setup_shell_term_info(void)
{
    term_info_t *term_info = calloc(1, sizeof(term_info_t));

    if (term_info == NULL)
        return NULL;
    term_info->_cursor_index = 0;
    term_info->_buffer_len = 0;
    term_info->_sig_buffer_reset = false;
    for (int i = 0; i < BUFFER_TERMIOS_SIZE; i++) {
        term_info->_buffer[i] = 0;
        term_info->_yank_buffer[i] = 0;
    }
    term_info->_history_index = 0;
    return term_info;
}

/**
 * @brief Init termios is a function for putting the right terminal config.
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

/**
 * @brief Setup a new prompt, this function need to be called after a
 * newline.
 *
 * @param ti            Terminal information structure
 */
void setup_new_prompt(OUT term_info_t *ti)
{
    if (ti == NULL)
        return;
    enable_raw_mode(get_shell());
    reset_buffer_termios(ti);
    ti->_history_index = -1;
    print_shell_prompt();
    get_cursor_position(&ti->_cursor_start_pos[POS_Y],
        &ti->_cursor_start_pos[POS_X]);
    get_cursor_position(&ti->_cursor_pos[POS_Y],
        &ti->_cursor_pos[POS_X]);
    set_cursor_position(ti->_cursor_start_pos[POS_Y],
        ti->_cursor_start_pos[POS_X]);
}

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
