/*
** EPITECH PROJECT, 2025
** include/core/minishell
** File description:
** Header file for mini shell project
*/

#ifndef MINISHELL_H
    #define MINISHELL_H

    #include <stdio.h>
    #include <stdbool.h>
    #include <stdlib.h>
    #include <stddef.h>
    #include <stdarg.h>
    #include <unistd.h>
    #include <limits.h>
    #include <signal.h>
    #include <sys/wait.h>
    #include <sys/stat.h>
    #include <errno.h>
    #include <fcntl.h>
    #include <termios.h>
    #include <string.h>

    #include "builtins.h"
    #include "macros/math_macros.h"
    #include "macros/misc_macros.h"
    #include "core/types.h"
    #include "core/signals.h"
    #include "core/parser.h"
    #include "my_printf.h"

/*
 * Environment structure used in a linked list.
 * This method is faster than a normal array aka extern char **environ
 * Have directly key and value and the next env var in the same place.
 */
typedef struct env_node_s {
    char *key;
    char *value;
    struct env_node_s *next;
} env_node_t;

/*
 * Passwd structure
 * This structure contains stuff of a line in /etc/passwd file
 * - user = Name of the user.
 * - pass = Contain info if a password for this user exist or not.
 * - uid, gid = UID and GID of the user.
 * - comment = If there is any comments in the line.
 * - home = Get the home of the user (path).
 * - shell = Get the shell of the user (path).
 */
typedef struct passwd_s {
    char *user;
    char *pass;
    int uid;
    int gid;
    char *comment;
    char *home;
    char *shell;
} passwd_t;

/*
 * Alias structure
 * This structure contains stuff for aliases.
 * - original_string = The original string to be replaced.
 * - alias_string = The final string which will appear for the final output.
 */
typedef struct alias_s {
    char *original_string;
    char *alias_string;
    struct alias_s *next;
} alias_t;

/*
 * Shell variables structure
 * This structure contains all variables used in the shell for exemple HOME,
 * OLDPWD, PWD.
 * We don't use them directly in the *env because if the user delete them
 * you need to make sure you can still use them. So we have this structure
 * to make sure you have the information in any case.
 */
typedef struct shell_variables_s {
    char *home_var;
    char *oldpwd_var;
    char *pwd_var;
    char *path_var;
    char *github_repository;
    int history_lines_count;
} shell_variables_t;

/*
 * Simple string linked list.
 * Use to stock some values for pretty much everything.
 * Used actually for stocking every values to be swapped like $PATH
 */
typedef struct string_s {
    char *string;
    struct string_s *next;
} string_t;

/**
 * @brief Terminal structure informations
 *        Contains stuff for the input line with cursor.
 */
typedef struct term_info_s {
    struct termios _original_termios;
    struct termios _current_termios;
    char _buffer[4096];
    size_t _buffer_len;
    size_t _cursor_index;
    bool _sig_buffer_reset;
} term_info_t;

/*
 * Shell structure
 * - env = Linked list with all environment variables.
 * - last_process_status = The last processus exit code. Used with echo $?.
 * - vars = Contains some variables in the shell like HOME, PWD, OLDPWD.
 * - should_exit = If the BUILTIN exit is called then make this true to return.
 * - user_input_buffer = The original input buffer in case you need it.
 */
typedef struct shell_s {
    term_info_t *_term_info;
    env_node_t *env;
    alias_t *aliases;
    shell_variables_t *vars;
    exitcode_t last_exit_code;
    char *last_input_buffer;
    pid_t current_child_pid;
    fd_t saved_stdout;
    fd_t saved_stdin;
    bool is_piped_shell;
    bool should_exit;
} shell_t;

/*
 * Shell manager functions
 */
shell_t *get_shell(void);
exitcode_t exit_shell(void);
exitcode_t shell_execute(token_list_t *list_tokens);

/*
 * Shell variables functions
 */
shell_variables_t *create_shell_vars(void);
void print_shells_vars(void);
void free_shell_vars(void);

/*
 * Command handling functions
 */
int show_error_execve(int errno_val, char **argv);
exitcode_t execute_ast_node(ast_node_t *node);
void exec_binary(char **argv);
exitcode_t exec_built_in(char **argv);
exitcode_t exec_built_in(char **argv);
exitcode_t execute_command_fork(ast_node_t *ast);
int execute_pipes(ast_node_t *node);
int execute_redirection(ast_node_t *node);
int execute_and(ast_node_t *node);
int execute_or(ast_node_t *node);
exitcode_t get_wait_status(int wait_pid_result);

/*
 * Environment handling functions
 */
void add_env(char *key, char *value);
int remove_env(char *key);
char *env_search(char *key);
void clear_env(void);
void reset_initial_env(void);

/*
 * Termios helping functions
 */
term_info_t *setup_shell_term_info(void);
void init_termios(void);
char *termios_get_input(void);
void reset_buffer_termios(term_info_t *term_info);
void print_input_termios(term_info_t *term_info, bool show_cursor);

/*
 * Environment transformer functions
 */
char **env_node_to_array(env_node_t *env);

/*
 * Binary paths functions
 */
char *get_binary_path(char *command);

/*
 * String utils functions
 */
int my_strcmp(char *s1, char *s2);
int my_strncmp(char *s1, char *s2, size_t n);
char *my_strcpy(char *dest, char const *src);
char *my_strncpy(char *dest, char const *src, size_t n);
char *my_strncpy_alloc(char const *src, size_t n);
char *my_strdup(char const *src);
char *my_strtok(char *string, char until_char);
char *my_strtok_reverse(char *string, char until_char);
char **my_strarray(char const *src);
char *my_strcat(char *dest, char const *src);
char *my_strncat(char *dest, char const *src, size_t n);
char *my_strreplace(char *string, char *to_replace, char *replace_string);
char **my_strreplace_array(char **array, char *to_replace, char *replace_str);
char *my_strreplace_full(char *string, char *from, char *to);
char **my_strreplace_array_full(char **array, char *to_replace, char *replace);
char *my_strcat_list(char *dest, ...);
int is_input_delimiter(char c);
int is_quote_delimiter(char c, char prev_c);
string_t *add_string(string_t *head, char *string);
void free_strings(string_t *head);
void print_strings(string_t *head);

void remove_newline(char *str);
char **command_formatter(char **argv);
void format_ast(ast_node_t *ast);

char *handle_quotes(char *string);
char **handle_quotes_array(char **array);

int print_err(char *message, ...);

bool is_letter(char c);
bool is_only_numbers(char *string);
bool is_alpha_num(char *string);
int len_to_first_char(char *str);

/*
 * Files functions
 */
bool is_file_type(char *path_file, mode_t type);
bool is_file_exist(char *path_file);

/*
 * Prompt helper functions
 */
void print_shell_prompt(void);

/*
 * Aliases handler functions
 */
void add_alias(char *original_string, char *alias_string);
int remove_alias(char *original_string);
void clear_aliases(void);
char **replaces_all_aliases(char **input);

/*
 * Passwd helper functions
 */
passwd_t *get_passwd_from_user(char *username);
void print_passwd(passwd_t *passwd);
void free_passwd(passwd_t *passwd);

/*
 * Utils functions
 */
void print_array(char **array);
void free_array_string(char **array);

/*
 * Mathematics functions
 */
long my_powl(long number, int exponent);

/*
 * Parsing functions
 */
int my_atoi(char *string);
char *my_itoa(int number);
size_t read_line(int fd, char *buffer, size_t max_len);

/*
 * Array functions
 */
size_t array_count_string(char **string_array);
char **array_add_index_string(char **original, char *to_add, size_t index);
char **array_append_index_string(char **original, char **append, size_t index);
char **array_remove_index_string(char **original, size_t index);

/*
 * Modified system calls
 */
void free_null_check(void *pointer);

/*
 * myshrc functions
 */
int load_myshrc(void);

/*
 * my_sh_history functions
 */
int open_history_file(void);
int write_command_history(char *command);
int count_number_lines_history(void);

/*
 * Environments variables extracting formatting functions
 */
string_t *extract_vars_in_string(string_t *strings, char *input);
string_t *extract_vars_in_array(char **array);
int replace_env_variables(char ***argv);
int replace_variables(char ***argv);

/*
 * Github features functions
 */
bool is_in_github_repository(void);
char *get_github_repository_name(void);

#endif /* ifndef MINISHELL_H */
