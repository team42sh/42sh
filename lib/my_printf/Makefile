##
## EPITECH PROJECT, 2024
## my_printf
## File description:
## Makefile for my_printf
##

SRC	=	my_printf.c \
		my_putchar.c \
		my_strlen.c \
		my_put_nbr.c \
		my_put_double.c \
		my_put_double_no_trailing.c \
		my_put_exp_no_trailing.c \
		my_putstr.c \
		print_str.c \
		print_int.c \
		print_char.c \
		print_unsigned_int.c \
		print_double.c \
		print_double_exp.c \
		print_double_no_trailing.c \
		print_misc.c \
		print_address.c \
		print_float_hex.c \
		format_parser.c \
		format_utils.c \
		my_strstr.c \
		print_result.c

OBJ	=	$(SRC:.c=.o)

NAME	=	libmy_printf.a

all:	$(NAME)

$(NAME):	$(OBJ)
	ar rc $(NAME) $(OBJ)
	mv $(NAME) ../

clean:
	rm -f $(OBJ)

fclean: clean
	cd ../ && rm -f $(NAME)

re: fclean all
