/*
** EPITECH PROJECT, 2025
** command_helper.c
** File description:
** This file contains the functions used for handling
** all command typed in the SHELL
*/

#include "../../../include/core/minishell.h"

/**
 * @brief Get the return value of the wait pit.
 *
 * @param wait_pid_result       The result of the waitpid()
 *
 * @return The exit code of the waitpid.
 */
exitcode_t
get_wait_status(int wait_pid_result)
{
    if (WIFSIGNALED(wait_pid_result)) {
        print_signal(WTERMSIG(wait_pid_result), WCOREDUMP(wait_pid_result));
        return 128 + WTERMSIG(wait_pid_result);
    }
    if (WIFEXITED(wait_pid_result))
        return WEXITSTATUS(wait_pid_result);
    return CURRENTLY_CHILD;
}

/*
 * When executing a execve if this function is called, it will show the
 * appropriate error message.
 * Actually handle :
 * - Format not recognized
 * - Every others cases are handled as 'Permission denied'
 */
int
show_error_execve(int errno_val, char **argv)
{
    switch (errno_val) {
        case ENOEXEC:
            return print_err("%s: Exec format error."
                " Binary file not executable.\n", argv[0]);
        case ENOENT:
            return print_err("%s: Command not found.\n", argv[0]);
        default:
            return print_err("%s: Permission denied.\n", argv[0]);
    }
}

/**
 * @brief Return the index of the bultin in the array.
 *
 * @param argv   The argv parameters
 *
 * @return The index of the builtin.
 */
int
is_builtin(char **argv)
{
    char *command_name = NULL;

    for (int i = 0; BUILTINS[i].command_name != NULL; i++) {
        command_name = my_strdup(BUILTINS[i].command_name);
        if (my_strcmp(argv[0], command_name) == 0) {
            free(command_name);
            return i;
        }
        free(command_name);
    }
    return -1;
}

/**
 * @brief Try to execute a builtin. If the builtin is not existing
 *        Then executing a fork() followed by an exec_binary.
 *
 * @param argv          The ARGV
 *
 * @return The exit code of the command.
 */
exitcode_t
execute_command_fork(ast_node_t *ast)
{
    pid_t pid = 0;
    int status_wait = 0;
    int status = -1;

    status = exec_built_in(ast->token->data._argv);
    if (status != OK_OUTPUT && status != -1 && get_shell()->is_piped_shell)
        get_shell()->should_exit = 1;
    if (status == -1) {
        pid = fork();
        if (pid == -1)
            return ERROR_OUTPUT;
        if (pid == 0) {
            exec_binary(ast->token->data._argv);
            return CURRENTLY_CHILD;
        }
        waitpid(pid, &status_wait, 0);
        get_shell()->last_exit_code = get_wait_status(status_wait);
    }
    return get_shell()->last_exit_code;
}
