
CC = cc
# -g is to debug
# -lreadline to link with the readline library
# -Iinclude to include the header files in the include directory
CFLAGS = -Wall -Wextra -g -Iinclude -fsanitize=address -fno-omit-frame-pointer 

LIBFT_DIR = ./libft
LIBFT_LIB = $(LIBFT_DIR)/libft.a
SRC_DIR = src # NEW

NAME = minishell

SRC = $(shell find $(SRC_DIR) -name "*.c") # NEW
#SRC = $(wildcard $(SRC_DIR)/**/*.c) $(wildcard $(SRC_DIR)/*.c)

OBJ_DIR = ./obj
# OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC)) # NEW

all: $(NAME)

# Link object files and libft to create the executable
$(NAME): $(OBJ) $(LIBFT_LIB)
	$(CC) $(CFLAGS) $(OBJ) -o $@ -L$(LIBFT_DIR) -lft -lreadline

# Rule to build libft if it doesn't exist
$(LIBFT_LIB): $(LIBFT_DIR)/Makefile
	$(MAKE) -C $(LIBFT_DIR)

#The obj dir will be created if it doesn't exist
#$(OBJ_DIR)/%.o: %.c
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
#	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(dir $@) 
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -rf $(OBJ_DIR)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)
	rm -rf $(OBJ_DIR)

re: fclean all

# debug rule
# will create all the stdout and stderr in a tmp file at the end it will print wahtever is needed
# at the end it will delete the tmp file
debug: $(NAME)
	@echo "Running $(NAME) under valgrind..."
	valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes ./$(NAME)
	@echo "Running automated tests..."
	@tmpfile=$$(mktemp); \
	if bash test/run_all.sh > $$tmpfile 2>&1; then \
		echo -e "\033[0;32m🎉 All tests passed! 🎉\033[0m"; \
	else \
		echo -e "\033[0;31m❌ Some tests failed! ❌\033[0m"; \
		echo "Errors summary:"; \
		cat $$tmpfile; \
	fi; \
	rm -f $$tmpfile
.PHONY: all clean fclean re debug

