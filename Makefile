# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: student <student@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/15 10:00:00 by student           #+#    #+#              #
#    Updated: 2024/01/15 10:00:00 by student          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS = main.c \
	utils.c \
	utils2.c \
	tokenizer.c \
	tokenizer4.c \
    tokenizer_helpers.c \
	parser.c \
	parser2.c \
    parser3.c \
	environment.c \
	environment2.c \
    environment3.c \
	executor.c \
	executor2.c \
	executor3.c \
	builtins.c \
	builtins2.c \
	builtins3.c \
    redirections3.c \
    redirections4.c \
    expansion3.c \
    expansion4.c \
	signals.c \
	error.c

OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lreadline

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

%.o: %.c minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re