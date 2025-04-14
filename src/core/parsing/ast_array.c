/*
** EPITECH PROJECT, 2025
** src/core/parsing/ast_array
** File description:
** This file contains functions for handling AST arrays
*/

#include "core/minishell.h"

/**
 * @brief Create an empty AST array wiht 1 element allocated but NULL.
 *
 * @return The AST array.
 */
ast_command_t *create_ast_array(void)
{
    ast_command_t *ast_command = malloc(sizeof(ast_command_t));

    ast_command->commands = malloc(sizeof(ast_node_t *));
    ast_command->commands[0] = NULL;
    ast_command->count = 1;
    return ast_command;
}

/**
 * @brief Add a AST in the array.
 *
 * @param asts          The AST array
 *
 * @return The array of AST.
 */
ast_command_t *add_ast_array(ast_command_t *asts)
{
    size_t new_len = asts->count + 1;
    ast_node_t **new_commands = malloc(sizeof(ast_node_t *) * (new_len + 1));

    if (new_commands == NULL)
        return asts;
    for (size_t i = 0; i < new_len; i++)
        new_commands[i] = NULL;
    for (int i = 0; i < asts->count; i++)
        new_commands[i] = asts->commands[i];
    free(asts->commands);
    asts->commands = new_commands;
    asts->count++;
    return asts;
}
