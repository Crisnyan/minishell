# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vpf <vpf@student.42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/24 18:34:41 by vperez-f          #+#    #+#              #
#    Updated: 2024/09/14 15:12:04 by vpf              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CFILES = minishell/process_handling.c	\
		 minishell/user_and_signals.c	\
		 minishell/parsing.c			\
		 minishell/minishell.c			\
		 env/environment.c 				\
		 env/hash_table.c 				\
		 env/hash_table_utils.c 		\
		 built-in/pwd.c 				\
		 built-in/unset.c 				\
		 built-in/cd.c 					\
		 built-in/env.c 				\
		 built-in/export.c 				\
		 built-in/export_utils.c 		\
		 built-in/checker.c				\
		 built-in/exit.c				\
		 built-in/echo.c				\
		 tokenizer/is_funcs.c			\
		 tokenizer/tokens.c				\
		 tokenizer/create_tokens.c		\
		 tokenizer/minishplit.c			\
		 expansor/expand_string.c		\
		 expansor/tokjoin.c				\
		 executor/childs_management.c	\
		 executor/cmd_utils.c			\
		 executor/error_management.c	\
		 executor/exec_utils.c			\
		 executor/heredoc_utils.c		\
		 executor/memory_management.c	\
		 executor/redirections_utils.c	\
		 executor/redirections.c		\
		 executor/exec.c

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

CFLAGS = -Wall -Wextra -Werror -fsanitize=address -fsanitize=leak

CC = cc

RM = rm -f

DEF_COLOR = \033[0m
GREEN = \033[32;1m
RED = \033[31;1m

all: extra_make $(NAME)
	@printf "\n$(GREEN)$(NAME) COMPILED!$(DEF_COLOR)\n\n"

extra_make:
	@printf "EXTRAS: COMPILING...\n"
	@$(MAKE) -C libft/ --no-print-directory
	@printf "\n$(GREEN)LIBFT: COMPILED!$(DEF_COLOR)\n\n"
	@$(MAKE) -C printf/ --no-print-directory
	@printf "\n$(GREEN)FT_PRINTF: COMPILED!$(DEF_COLOR)\n\n"

$(NAME): $(OBJ) $(PATH_LFT)
	@$(CC) $(CFLAGS) $(OBJ) $(PATH_LFT) $(PATH_PTF) $(READLINE_FLAGS) -o $(NAME)

$(OBJ_DIR)%.o: src/%.c $(DIR_LFT) $(DIR_PTF) $(HEADERS) Makefile
	@mkdir -p $(@D)
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
