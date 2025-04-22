/*
** EPITECH PROJECT, 2025
** null_ptr_handling.c
** File description:
** This unit tests series tests all the fucntion capacity to accept null ptr
** These test does not verify the output, only if it crash or not
*/

#include "core/builtins.h"
#include "core/minishell.h"
#include "core/parser.h"
#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <criterion/internal/test.h>
#include <criterion/redirect.h>
#include <stdbool.h>

static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

TestSuite(null_ptr_handling, .description="\e[32mThis suite try to give NULL ptr"
    " to all the function.\n\e[32mIts only purpose is to check whether the function"
    " crash or not.\n\e[32mReturn value are not checked and may vary from function to function.\e[0m",
    .disabled = false, .timeout = 10., .init = redirect_all_std);

Test(null_ptr_handling, alias_command)
{
    alias_command(NULL);
}

Test(null_ptr_handling, cd_command)
{
    cd_command(NULL);
}

Test(null_ptr_handling, env_command)
{
    env_command(NULL);
}

Test(null_ptr_handling, exit_command)
{
    exit_command(NULL);
}

Test(null_ptr_handling, setenv_command)
{
    setenv_command(NULL);
}

Test(null_ptr_handling, unalias_command)
{
    unalias_command(NULL);
}

Test(null_ptr_handling, unsetenv_command)
{
    unsetenv_command(NULL);
}

Test(null_ptr_handling, aliases_handler)
{
    add_alias(NULL, NULL);
    remove_alias(NULL);
    replaces_all_aliases(NULL);
}

Test(null_ptr_handling, env_handler)
{
    env_search(NULL);
    add_env(NULL, NULL);
    remove_env(NULL);
}

Test(null_ptr_handling, command_helper)
{
    show_error_execve(0, NULL);
    is_builtin(NULL);
    execute_command_fork(NULL);
}

Test(null_ptr_handling, mysh_history_helper)
{
    write_command_history(NULL);
}

Test(null_ptr_handling, passwd_helper)
{
    get_passwd_from_user(NULL);
    free_passwd(NULL);
    print_passwd(NULL);
}

Test(null_ptr_handling, pipes_helper)
{
    execute_pipes(NULL);
}

Test(null_ptr_handling, redirections_helper)
{
    execute_redirection(NULL);
}

Test(null_ptr_handling, ast_actions)
{
    create_ast_pipe(NULL, NULL);
    create_ast_redirect(NULL, NULL);
    create_ast_command(NULL, NULL);
    create_ast_and(NULL, NULL);
    create_ast_or(NULL, NULL);
}

Test(null_ptr_handling, ast_array)
{
    add_ast_array(NULL);
}

Test(null_ptr_handling, ast_free)
{
    free_ast(NULL);
    free_asts(NULL);
}

Test(null_ptr_handling, ast_print)
{
    print_ast(NULL);
    print_asts(NULL);
}

Test(null_ptr_handling, ast)
{
    create_tree(NULL);
}

Test(null_ptr_handling, string_until_operator)
{
    get_string_until_operator(NULL);
}

Test(null_ptr_handling, find_token_error)
{
    find_token_error(NULL);
}

Test(null_ptr_handling, free_token)
{
    free_token(NULL);
    free_token_list(NULL);
}

Test(null_ptr_handling, token_utils)
{
    is_token_redirection_append(NULL);
    is_token_redirection_simple(NULL);
}

Test(null_ptr_handling, tokenizer)
{
    add_token(NULL, NULL);
    tokenize_line(NULL);
    print_tokens(NULL);
}

Test(null_ptr_handling, shell_executer)
{
    execute_ast_node(NULL);
    exec_binary(NULL);
    exec_built_in(NULL);
    shell_execute(NULL);
}

Test(null_ptr_handling, formatter_handler)
{
    format_ast(NULL);
    command_formatter(NULL);
}

Test(null_ptr_handling, quotes_handler)
{
    handle_quotes(NULL);
    handle_quotes_array(NULL);
}

Test(null_ptr_handling, variables_handler)
{
    extract_vars_in_array(NULL);
    extract_vars_in_string(NULL, NULL);
    replace_env_variables(NULL);
    replace_variables(NULL);
}

Test(null_ptr_handling, add_array_index)
{
    array_add_index_string(NULL, NULL, 0);
    array_append_index_string(NULL, NULL, 0);
    array_remove_index_string(NULL, 0);
}

Test(null_ptr_handling, array_counter)
{
    array_count_string(NULL);
}

Test(null_ptr_handling, print_array)
{
    print_array(NULL);
}

Test(null_ptr_handling, binaries_path)
{
    get_binary_path(NULL);
}

Test(null_ptr_handling, env_transfomer)
{
    env_node_to_array(NULL);
}

Test(null_ptr_handling, files_utils)
{
    is_file_type(NULL, 0);
    is_file_exist(NULL);
}

Test(null_ptr_handling, atoi)
{
    my_atoi(NULL);
}

Test(null_ptr_handling, str_array)
{
    free_array_string(NULL);
    my_strarray(NULL);
}

Test(null_ptr_handling, str_ascii)
{
    len_to_first_char(NULL);
    is_alpha_num(NULL);
    is_only_numbers(NULL);
}

Test(null_ptr_handling, str_err)
{
    print_err(NULL);
}

Test(null_ptr_handling, str_replace)
{
    my_strreplace(NULL, NULL, NULL);
    my_strreplace_array(NULL, NULL, NULL);
    my_strreplace_full(NULL, NULL, NULL);
    my_strreplace_array_full(NULL, NULL, NULL);
}

Test(null_ptr_handling, str_utils)
{
    remove_newline(NULL);
    my_strcmp(NULL, NULL);
    my_strncmp(NULL, NULL, 0);
    my_strdup(NULL);
}

Test(null_ptr_handling, strcat_valist)
{
    my_strcat_list(NULL);
}

Test(null_ptr_handling, str_cat)
{
    my_strncat(NULL, NULL, 0);
    my_strcat(NULL, NULL);
}

Test(null_ptr_handling, strcpy)
{
    my_strcpy(NULL, NULL);
    my_strncpy(NULL, NULL, 0);
    my_strncpy_alloc(NULL, 0);
}

Test(null_ptr_handling, strings_linked_list)
{
    add_string(NULL, NULL);
    free_strings(NULL);
    print_strings(NULL);
}

Test(null_ptr_handling, strtok_reverse)
{
    my_strtok_reverse(NULL, 0);
}

Test(null_ptr_handling, strtok)
{
    my_strtok(NULL, 0);
}

Test(null_ptr_handling, free)
{
    free_null_check(NULL);
}

Test(null_ptr_handling, and_or_helper)
{
    execute_or(NULL);
    execute_and(NULL);
}

Test(null_ptr_handling, ast_post)
{
    ast_priority_process(NULL);
}
