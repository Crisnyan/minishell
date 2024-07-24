# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/24 18:34:41 by vperez-f          #+#    #+#              #
#    Updated: 2024/07/24 19:43:02 by vperez-f         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CFILES = minishell.c

OFILES = $(CFILES:%.c=%.o)

OBJ = $(addprefix $(OBJ_DIR),$(OFILES))

OBJ_DIR = obj/

OBJ_PATH = obj

READLINE_FLAGS = -L./readline -lreadline -lhistory -lncurses

CFLAGS = -Wall -Wextra -Werror #-O3 -fsanitize=address -fsanitize=leak

CC = cc

RM = rm -f

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) $(READLINE_FLAGS) -o $(NAME)
	@printf "\n$(NAME) COMPILED!\n"

$(OBJ_DIR)%.o: src/%.c Makefile
	@mkdir -p $(OBJ_PATH)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) $(OBJ)

fclean:	clean
	@$(RM) $(NAME)
	@rm -rf $(OBJ_PATH)

re:	fclean all

.PHONY: all clean fclean re