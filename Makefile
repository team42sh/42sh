##
## EPITECH PROJECT, 2025
## Makefile
## File description:
## MINISHELL_1
##

SRC		=	./src/main.c \
			./src/core/shell_manager.c \
			./src/core/shell_variables.c \
			./src/core/shell_executer.c \
			./src/core/helpers/command_helper.c \
			./src/core/helpers/prompt_helper.c \
			./src/core/helpers/passwd_helper.c \
			./src/core/helpers/myshrc_helper.c \
			./src/core/helpers/mysh_history_helper.c \
			./src/core/helpers/pipes_helper.c \
			./src/core/helpers/redirections_helper.c \
			./src/core/helpers/github_helper.c \
			./src/core/handlers/signal_handler.c \
			./src/core/handlers/env_handler.c \
			./src/core/handlers/aliases_handler.c \
			./src/core/parsing/tokenizer.c \
			./src/core/parsing/ast.c \
			./src/core/parsing/ast_print.c \
			./src/core/parsing/ast_array.c \
			./src/core/parsing/ast_free.c \
			./src/core/parsing/ast_actions.c \
			./src/core/parsing/token_free.c \
			./src/core/parsing/token_error.c \
			./src/core/parsing/token_utils.c \
			./src/core/parsing/string_until_operator.c \
			./src/utilities/strings/strtok.c \
			./src/utilities/strings/strtok_reverse.c \
			./src/utilities/strings/str_array.c \
			./src/utilities/strings/str_utils.c \
			./src/utilities/strings/strcpy.c \
			./src/utilities/strings/str_replace.c \
			./src/utilities/strings/str_err.c \
			./src/utilities/strings/str_ascii.c \
			./src/utilities/strings/strcat_valist.c \
			./src/utilities/strings/strings_linked_list.c \
			./src/utilities/strings/strcat.c \
			./src/utilities/parsing/atoi.c \
			./src/utilities/parsing/itoa.c \
			./src/utilities/parsing/readline.c \
			./src/utilities/math/pow.c \
			./src/utilities/environment/binaries_path.c \
			./src/utilities/environment/env_transfomer.c \
			./src/utilities/files/files_utils.c \
			./src/utilities/arrays/array_counter.c \
			./src/utilities/arrays/add_array_index.c \
			./src/utilities/arrays/print_array.c \
			./src/formatting/quotes_handler.c \
			./src/formatting/formatter_handler.c \
			./src/formatting/variables_handler.c \
			./src/utilities/syscalls/free.c \

BUILTINS	= 	./src/builtins/exit.c \
			./src/builtins/env.c \
			./src/builtins/setenv.c \
			./src/builtins/unsetenv.c \
			./src/builtins/cd.c \
			./src/builtins/alias.c \
			./src/builtins/unalias.c \

OBJ_SRC		=	$(SRC:.c=.o)
OBJ_BUILTINS	=	$(BUILTINS:.c=.o)

NAME		=	42sh

CFLAGS		=	-Wall -Wextra -Wpedantic -g

LDFLAGS		=	-L./lib/ -lmy_printf

all: $(NAME)

$(NAME): $(OBJ_SRC) $(OBJ_BUILTINS)
	cd ./lib/my_printf && make
	gcc -o $(NAME) $(OBJ_SRC) $(OBJ_BUILTINS) $(LDFLAGS) $(CFLAGS)

clean:
	cd ./lib/my_printf && make clean
	rm -f $(OBJ_SRC) $(OBJ_BUILTINS)

fclean: clean
	cd ./lib/my_printf && make fclean
	rm -f $(NAME) $(OBJ_SRC) $(OBJ_BUILTINS)

re:	fclean all
