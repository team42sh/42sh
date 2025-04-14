##
## EPITECH PROJECT, 2024
## my_printf
## File description:
## Makefile for my_printf
##

# Settings
CC = gcc
CFLAGS = -I./include -Wall -Wextra -Werror -g
TARGET = libmy_printf.a

SRCS := $(shell find src -name "*.c")
OBJS = $(SRCS:.c=.o)

# Colors
RED=\033[0;31m
GREEN=\033[0;32m
YELLOW=\033[0;33m
BLUE=\033[0;34m
PURPLE=\033[0;35m
CYAN=\033[0;36m
WHITE=\033[0;37m
RESET=\033[0m

PREIX=[$(PURPLE)my_printf$(RESET)]

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "$(PREIX)$(BLUE)Linking $(PURPLE)$(TARGET)$(RESET)"
	@ar rc ../$(TARGET) $(OBJS) 1>/dev/null
	@echo "$(PREIX)$(GREEN)Build successful!$(RESET)"

%.o: %.c
	@echo "$(PREIX)$(YELLOW)Compiling $(RESET)$(CYAN)$(dir $<)\
	$(GREEN)$(notdir $<)$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@ 1>/dev/null

dev: all

clean:
	@echo "$(PREIX)$(CYAN)Cleaning objects$(RESET)"
	@rm -f $(OBJS)

fclean: clean
	@echo "$(PREIX)$(CYAN)Cleaning complied libraries$(RESET)"
	@rm -f ../$(TARGET)

re: fclean all

.PHONY: all clean
