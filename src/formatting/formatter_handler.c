/*
** EPITECH PROJECT, 2025
** src/strings/formatter_handler
** File description:
** This file will modify every argument in the char **argv and will put
** for example the $? with the last exit code and modify all quotes by
** removing them or asking to fisnish the string if needed
*/

#include "core/minishell.h"

/**
 * @brief Format a ast with command formatter.
 *
 * @param ast   The AST node
 */
void
format_ast(ast_node_t *ast)
{
    token_t *token = NULL;

    if (ast == NULL)
        return;
    token = ast->token;
    if (token == NULL)
        return;
    if (token->token_type == TOKEN_COMMAND)
        token->data._argv = command_formatter(token->data._argv);
    if (token->token_type == TOKEN_PIPE || token->token_type == TOKEN_AND ||
        token->token_type == TOKEN_OR) {
        format_ast(ast->right);
        format_ast(ast->left);
    }
    if (token->token_type == TOKEN_LEFT_APPEND ||
        token->token_type == TOKEN_LEFT_REDIRECTION ||
        token->token_type == TOKEN_RIGHT_REDIRECTION ||
        token->token_type == TOKEN_RIGHT_APPEND)
        format_ast(ast->left);
}

/*
 * This func will modify every argument in the char **argv and will put
 * for example the $? with the last exit code and modify all quotes by
 * removing them.
 *
 * Can do more in the future.
 * TODO: Waiting the user to fill the quotes if one is opened but not closed.
 * TODO: Replace all \ char by the real one. \" -> " for example.
 * THIS NEED TO BE DONE AFTER HANDLE QUOTES ARRAY !!!!!!!!!!!!!!
 *
 * What it does in order :
 * - Replace variables such as : $?, all environments variable ($SHELL, ...)
 * - Remove every useless quotes such as : ["foo bar"] => [foo bar] if not '\'
 * - Replace every ASCII character typed with a '\' before NOT DONE
 */
char **command_formatter(char **argv)
{
    if (argv == NULL)
        return NULL;
    if (replace_variables(&argv) == ERROR_OUTPUT ||
        handle_quotes_array(argv) == NULL ||
        handle_inhibitors_array(argv) == NULL) {
        free_array_string(argv);
        return NULL;
    }
    argv = replaces_all_aliases(argv);
    argv = change_star_to_list_of_files(argv);
    return argv;
}
