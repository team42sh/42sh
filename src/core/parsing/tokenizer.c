/*
** EPITECH PROJECT, 2025
** src/core/parsing/tokenizer
** File description:
** This file will create a list of every tokens in order
*/

#include "core/minishell.h"


const char *tokens_name[] = {"COMMAND", "PIPE", "LEFT_REDIRECTION",
    "LEFT_APPEND", "RIGHT_REDIRECTION", "RIGHT_APPEND", "AND", "OR",
    "BACKGROUND", "SEMI-COLON", "FILE", "END"};

/**
 * @brief Create a token with the type passed as parameter.
 *
 * @param type   The type
 *
 * @return The token.
 */
static token_t *
create_token(token_type_t type)
{
    token_t *token = malloc(sizeof(token_t));

    if (token == NULL)
        return NULL;
    token->next = NULL;
    token->token_type = type;
    return token;
}

/**
 * @brief Create an empty token list.
 *
 * @return The list.
 */
token_list_t *
create_token_list(void)
{
    token_list_t *list = malloc(sizeof(token_list_t));

    list->count = 0;
    list->head = NULL;
    return list;
}

/**
 * @brief Add the created token into the list.
 *
 * @param list          The list
 * @param token         The token to add
 *
 * @return An exit code. Success or failure.
 */
exitcode_t
add_token(token_list_t *list, token_t *token)
{
    token_t *tmp_head = list->head;

    if (token == NULL)
        return ERROR_OUTPUT;
    list->count++;
    if (tmp_head == NULL) {
        list->head = token;
        return OK_OUTPUT;
    }
    while (tmp_head->next != NULL)
        tmp_head = tmp_head->next;
    tmp_head->next = token;
    return OK_OUTPUT;
}

/**
 * @brief Create command token.
 *
 * @param line          The command line
 * @param i             The index in the line
 *
 * @return The token created.
 */
static token_t *
create_command_token(char *line, int *i)
{
    token_t *token = create_token(TOKEN_COMMAND);
    char *tmp_command = NULL;

    if (token == NULL)
        return NULL;
    tmp_command = get_string_until_operator(line);
    token->data._argv = my_strarray(tmp_command);
    (*i) += my_strlen(tmp_command);
    free_null_check(tmp_command);
    return token;
}

/**
 * @brief Create a operator redirection token.
 *
 * @param line          The command line
 * @param i             The index in the line
 *
 * @return The token created.
 */
static token_t *
create_redirect_token(char *line, int *i)
{
    token_t *token = create_token(get_redirect_token(line[*i], line[*i + 1]));
    char *file_name = NULL;
    char **rest_of_line = NULL;

    if (is_token_redirection_append(token))
        (*i) += 2;
    if (is_token_redirection_simple(token))
        (*i)++;
    while (line[*i] && is_input_delimiter(line[*i]))
        (*i)++;
    file_name = get_string_until_operator(&line[*i]);
    rest_of_line = my_strarray(file_name);
    if (rest_of_line != NULL) {
        token->data._file = my_strdup(rest_of_line[0]);
        (*i) += my_strlen(rest_of_line[0]);
    } else
        (*i)++;
    free_null_check(file_name);
    free_array_string(rest_of_line);
    return token;
}

/**
 * @brief Create an opeartor token.
 *
 * @param operator      The current character
 * @param next_char     The next character
 * @param i             The index in the line
 *
 * @return The token created.
 */
static token_t *
create_operator_token(char operator, char next_char, int *i)
{
    token_t *token = create_token(get_operator_token(operator, next_char));

    if (token == NULL)
        return NULL;
    if (token->token_type == TOKEN_SEMI_COLON ||
        token->token_type == TOKEN_BACKGROUND ||
        token->token_type == TOKEN_PIPE)
        (*i)++;
    if (token->token_type == TOKEN_OR ||
        token->token_type == TOKEN_AND)
        (*i) += 2;
    return token;
}

/**
 * @brief Tokenize the whole line and get the tokens.
 *
 * @param line   The line to tokenize
 *
 * @return The list of tokens.
 */
token_list_t *
tokenize_line(char *line)
{
    token_list_t *list = create_token_list();
    int line_size = my_strlen(line);

    for (int i = 0; i < line_size;) {
        if (line[i] == '>' || line[i] == '<') {
            add_token(list, create_redirect_token(line, &i));
            continue;
        }
        if (is_operator(line[i])) {
            add_token(list, create_operator_token(line[i], line[i + 1], &i));
            continue;
        }
        if (is_input_delimiter(line[i])) {
            i++;
            continue;
        }
        add_token(list, create_command_token(&line[i], &i));
    }
    return list;
}

/**
 * @brief Print the list of tokens.
 *
 * @param list  The token list
 */
void
print_tokens(token_list_t *list)
{
    for (token_t *tmp_head = list->head; tmp_head != NULL;
        tmp_head = tmp_head->next) {
        my_printf("\n\033[1;33m[ TOKEN ]\033[0m %s\n",
            tokens_name[tmp_head->token_type]);
        if (tmp_head->token_type == TOKEN_COMMAND) {
            my_printf("  \033[1;32m↳ Command:\033[0m ");
            print_array(tmp_head->data._argv);
        }
        if (tmp_head->token_type != TOKEN_LEFT_APPEND &&
            tmp_head->token_type != TOKEN_LEFT_REDIRECTION &&
            tmp_head->token_type != TOKEN_RIGHT_REDIRECTION &&
            tmp_head->token_type != TOKEN_RIGHT_APPEND)
            continue;
        if (tmp_head->data._file == NULL)
            my_printf("  \033[1;32m↳ File:\033[0m \033[1mNONE\033[0m\n");
        else
            my_printf("  \033[1;32m↳ File:\033[0m \033[1m%s\033[0m\n",
                tmp_head->data._file);
    }
    my_printf("\n");
}
