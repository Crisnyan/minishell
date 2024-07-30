# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/24 18:34:41 by vperez-f          #+#    #+#              #
#    Updated: 2024/07/30 19:33:13 by vperez-f         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CFILES = minishell.c environment.c

OFILES = $(CFILES:%.c=%.o)

OBJ = $(addprefix $(OBJ_DIR),$(OFILES))

HEADERS = includes/minishell.h

DIR_LIBFT = libft/ 

PATH_LFT = libft/libft.a

DIR_PTF = printf/

PATH_PTF = printf/libftprintf.a

OBJ_DIR = obj/

OBJ_PATH = obj

READLINE_FLAGS = -L./readline -lreadline -lhistory -lncurses

CFLAGS = -Wall -Wextra -Werror #-fsanitize=address -fsanitize=leak

CC = cc

RM = rm -f

all: extra_make $(NAME)
	@printf "\n$(NAME) COMPILED!\n"

extra_make:
	@printf "EXTRAS: COMPILING...\n$(END)"
	@$(MAKE) -C libft/ --no-print-directory
	@printf "\nLIBFT: COMPILED!\n"
	@$(MAKE) -C printf/ --no-print-directory
	@printf "\nFT_PRINTF: COMPILED!\n"

$(NAME): $(OBJ) $(PATH_LFT)
	@$(CC) $(CFLAGS) $(OBJ) $(PATH_LFT) $(PATH_PTF) $(READLINE_FLAGS) -o $(NAME)

$(OBJ_DIR)%.o: src/%.c $(DIR_LFT) $(DIR_PTF) $(HEADERS) Makefile
	@mkdir -p $(OBJ_PATH)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(MAKE) -C libft/ clean --no-print-directory
	@$(MAKE) -C printf/ clean --no-print-directory
	@$(RM) $(OBJ)

fclean:	clean
	@$(MAKE) -C libft/ fclean --no-print-directory
	@$(MAKE) -C printf/ fclean --no-print-directory
	@$(RM) $(NAME)
	@rm -rf $(OBJ_PATH)

re:	fclean all

.PHONY: all clean fclean re