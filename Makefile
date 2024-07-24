# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/24 18:34:41 by vperez-f          #+#    #+#              #
#    Updated: 2024/07/24 20:19:25 by vperez-f         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CFILES = minishell.c

OFILES = $(CFILES:%.c=%.o)

OBJ = $(addprefix $(OBJ_DIR),$(OFILES))

DIR_LIBFT = libft/ 

PATH_LFT = libft/libft.a

OBJ_DIR = obj/

OBJ_PATH = obj

READLINE_FLAGS = -L./readline -lreadline -lhistory -lncurses

CFLAGS = -Wall -Wextra -Werror #-O3 -fsanitize=address -fsanitize=leak

CC = cc

RM = rm -f

all: extra_make $(NAME)
	@printf "\n$(NAME) COMPILED!\n"

extra_make:
	@printf "LIBFT: COMPILING...\n$(END)"
	@$(MAKE) -C libft/ --no-print-directory
	@printf "\nLIBFT: COMPILED!\n"

$(NAME): $(OBJ) $(PATH_LFT)
	@$(CC) $(CFLAGS) $(OBJ) $(PATH_LFT) $(READLINE_FLAGS) -o $(NAME)

$(OBJ_DIR)%.o: src/%.c $(DIR_LFT) Makefile
	@mkdir -p $(OBJ_PATH)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(MAKE) -C libft/ clean --no-print-directory
	@$(RM) $(OBJ)

fclean:	clean
	@$(MAKE) -C libft/ fclean --no-print-directory
	@$(RM) $(NAME)
	@rm -rf $(OBJ_PATH)

re:	fclean all

.PHONY: all clean fclean re