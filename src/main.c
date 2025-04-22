/*
** EPITECH PROJECT, 2025
** src/main
** File description:
** Main file for the minishell project
*/

#include "core/minishell.h"

/*
 * Main function for minishell project
 * Show the prompt and execute the command when input in the buffer are been
 * added.
 *
 * Calling the function get_shell() one time before the loop to init all values
 * Then call setup_shell_signals to put the pointer of function for signals
 * like Ctrl + D or Ctrl + C.
 */
int main(void)
{
    if (setup_shell() == OK_OUTPUT)
        shell_loop();
    return exit_shell();
}
