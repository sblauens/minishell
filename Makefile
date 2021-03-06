# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sblauens <sblauens@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/12/05 16:59:41 by sblauens          #+#    #+#              #
#    Updated: 2019/02/15 03:12:39 by sblauens         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRC_DIR = src

OBJ_DIR = obj

INC_DIR = include

LIB_DIR = .

SRC_FILES = minishell.c env.c setenv.c builtins.c builtins_env.c utils.c \
			parser.c exec.c

LIB_NAMES = libft

CC = clang

CFLAGS = -Wall -Wextra -Werror

GFLAGS := -g3 -fsanitize=address -fno-omit-frame-pointer
ifeq ($(shell uname -s), Linux)
	GFLAGS += -fsanitize=leak
endif

SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILES))

OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

LIB = $(addprefix $(LIB_DIR)/, $(LIB_NAMES))

CPPFLAGS = -I $(INC_DIR) $(addprefix -I , $(LIB))

LDFLAGS = $(addprefix -L , $(LIB))

LDLIBS = $(addprefix -, $(patsubst lib%, l%, $(LIB_NAMES)))

CMSG = "\r"

all: $(NAME)

dbg: CFLAGS = $(GFLAGS)
dbg: CMSG = "__debug__: $(GFLAGS)\n"
dbg: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $^ $(LDFLAGS) $(LDLIBS) -o $@
	@echo "minishell: linking"
	@echo "minishell: ready to fork !"
	@printf $(CMSG)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(LIB_NAMES) $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@
	@echo "minishell: compiling $@"

$(OBJ_DIR):
	@mkdir $@

$(LIB_NAMES):
	@make -C $(addprefix $(LIB_DIR)/, $@)

clean: clean_lib clean_minishell

clean_minishell:
	@rm -rf $(OBJ)
	@rmdir $(OBJ_DIR) 2> /dev/null || true
	@echo "minishell: objects removed."

clean_lib:
	@make clean -C $(LIB_NAMES)

fclean: fclean_lib fclean_minishell

fclean_minishell: clean_minishell
	@rm -rf $(NAME)
	@echo "minishell: exec removed."

fclean_lib:
	@make fclean -C $(LIB_NAMES)

re: fclean all

.PHONY: all dbg clean fclean re $(LIB_NAMES)
