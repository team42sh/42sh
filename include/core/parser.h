/*
** EPITECH PROJECT, 2025
** include/core/parser
** File description:
** This header contains all stuff for parsing the line
*/

#ifndef PARSING_H
    #define PARSING_H

    #include "./types.h"

/*
 * Use the get the operators
 */
    #define OPERATOR_STRINGS ";&|<>"


/**
 * @brief Every tokens availabes.
 */
typedef enum {
    TOKEN_COMMAND,
    TOKEN_PIPE,
    TOKEN_LEFT_REDIRECTION,
    TOKEN_LEFT_APPEND,
    TOKEN_RIGHT_REDIRECTION,
    TOKEN_RIGHT_APPEND,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_BACKGROUND,
    TOKEN_SEMI_COLON,
    TOKEN_FILE,
    TOKEN_END,
} token_type_t;

/**
 * @brief Every errors of AST
 */
typedef enum {
    REDIRECT_NO_NAME,
    REDIRECT_NO_COMMAND,
    AMBIGUOUS_OUTPUT,
    AMBIGUOUS_INPUT,
    PIPE_NO_COMMAND,
    INVALID_COMMAND,
    NO_ERROR,
} syntax_ast_error_t;

/**
 * @brief A token.
 */
typedef struct token_s {
    token_type_t token_type;
    union {
        char **_argv;
        char *_file;
    } data;
    struct token_s *next;
} token_t;

/**
 * @brief The list of every tokens in the line.
 */
typedef struct token_list_s {
    token_t *head;
    int count;
} token_list_t;


/**
 * @brief A node in the abstract syntax tree for parsing.
 */
typedef struct ast_node_s {
    token_t *token;
    struct ast_node_s *left;
    struct ast_node_s *right;
} ast_node_t;

/**
 * @brief Each ast_command_t is a command seperated by ';'
 */
typedef struct ast_command_s {
    int count;
    ast_node_t **commands;
} ast_command_t;

/*
 * Print debug for tokens
 */
extern const char *tokens_name[];

/*
 * Tokenizer functions
 */
token_list_t *create_token_list(void);
exitcode_t add_token(token_list_t *list, token_t *token);
void print_tokens(token_list_t *list);
token_list_t *tokenize_line(char *line);
bool find_token_error(token_list_t *tokens);

/*
 * String utils functions for parsing
 */
bool is_operator(char c);
char *get_string_until_operator(char *line);
token_type_t get_redirect_token(char start_char, char next_char);
token_type_t get_operator_token(char start_char, char next_char);
bool is_redirection(token_type_t type);

/*
 * AST Utils functions
 */
void print_ast(ast_node_t *ast);
void print_asts(ast_command_t *asts);

ast_command_t *create_tree(token_list_t *list);
ast_command_t *ast_priority_process(ast_command_t *ast);
ast_command_t *create_ast_array(void);
ast_command_t *add_ast_array(ast_command_t *asts);

/*
 * AST functions
 */
ast_node_t *create_ast_pipe(ast_node_t *ast, token_t *token);
ast_node_t *create_ast_redirect(ast_node_t *ast, token_t *token);
ast_node_t *create_ast_semi_colon(ast_node_t *ast, token_t *token);
ast_node_t *create_ast_command(ast_node_t *ast, token_t *token);
ast_node_t *create_ast_and(ast_node_t *ast, token_t *token);
ast_node_t *create_ast_or(ast_node_t *ast, token_t *token);

/*
 * AST Free functions
 */
void free_asts(ast_command_t *asts);
void free_ast(ast_node_t *ast);
void free_token(token_t *token);
void free_token_list(token_list_t *list);

/*
 * Tokens utils functions
 */
bool is_token_redirection_simple(token_t *token);
bool is_token_redirection_append(token_t *token);

#endif /* ifndef PARSING_H */
