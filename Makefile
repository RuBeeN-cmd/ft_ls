_END="\033[0m"
_RED="\033[0;31m"
_GREEN="\033[0;32m"
_YELLOW="\033[0;33m"
_CYAN="\033[0;36m"

NAME = ft_ls

SRC = main.c \
		flags.c \
		sort.c
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

CC = gcc
CFLAGS = -Wall -Werror -Wextra -g3
INC = -Iincludes -I$(dir $(LIB))/includes

LIB = libft/libft.a
LIBFLAGS = -Llibft -lft

SRC_DIR = srcs
OBJ_DIR = objs

all: $(NAME)

$(NAME): test $(LIB) $(OBJ_DIR) $(OBJ)
	@echo $(_GREEN)Compiling $(OBJ)...$(END)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFLAGS) -o $@

test:
	@rm -rf a .a A aa z .z Z zz

$(LIB):
	@make -C $(dir $@)

$(OBJ_DIR):
	@mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo $(_CYAN)Compiling $<...$(END)
	@$(CC) -o $@ -c $< $(CFLAGS) $(INC)

clean:
	@echo $(_YELLOW)Cleaning $(OBJ)...$(END)
	@make -C $(dir $(LIB)) fclean
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo $(_RED)Cleaning $(NAME)...$(END)
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re