/*
** EPITECH PROJECT, 2025
** src/commands/exit
** File description:
** Exit COMMAND
*/

#include "core/minishell.h"

/*
 * EXIT COMMAND - BUILT-IN
 * This command will exit the shell.
 * If a argument is passed and it's a number. Then return the specified
 * exit status.
 * If argv == NULL then it's called in the getline when he got an EOF.
 */
exitcode_t exit_command(char **argv)
{
    if (argv != NULL && argv[1] != NULL && !is_only_numbers(argv[1]))
        return print_err("%s: Expression Syntax.\n", argv[0]);
    get_shell()->should_exit = 1;
    if (!get_shell()->is_piped_shell)
        write(1, "exit\n", 5);
    if (argv == NULL) {
        return get_shell()->last_exit_code;
    }
    if (argv[1] == NULL) {
        get_shell()->last_exit_code = 0;
        return get_shell()->last_exit_code;
    }
    get_shell()->last_exit_code = my_atoi(argv[1]);
    return get_shell()->last_exit_code;
}
