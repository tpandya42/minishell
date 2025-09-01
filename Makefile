CC = cc
# -g is to debug
# -lreadline to link with the readline library
# -Iinclude to include the header files in the include directory
CFLAGS = -Wall -Wextra -g -Iinclude #-fsanitize=address -fno-omit-frame-pointer 

LIBFT_DIR = ./libft
LIBFT_LIB = $(LIBFT_DIR)/libft.a
SRC_DIR = src

NAME = minishell

SRC = $(shell find $(SRC_DIR) -name "*.c") # NEW -> (tpandya_mac)

OBJ_DIR = ./obj
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC)) # NEW

all: $(NAME)

# Link object files and libft to create the executable
$(NAME): $(OBJ) $(LIBFT_LIB)
	$(CC) $(CFLAGS) $(OBJ) -o $@ -L$(LIBFT_DIR) -lft -lreadline

# debug rule
debug: CFLAGS += -DDEBUG
debug : $(NAME)

# Rule to build libft if it doesn't exist
$(LIBFT_LIB): $(LIBFT_DIR)/Makefile
	$(MAKE) -C $(LIBFT_DIR)

#The obj dir will be created if it doesn't exist
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
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

.PHONY: all clean fclean re debug

