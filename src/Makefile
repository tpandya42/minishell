MAKEFLAGS += --no-print-directory -s
CC = cc
CFLAGS = -Wall -Wextra -g -Iinclude

LIBFT_DIR = ./libft
LIBFT_LIB = $(LIBFT_DIR)/libft.a
SRC_DIR = src

NAME = minishell

SRC = src/main.c \
	src/builtin/builtin.c \
	src/builtin/builtin_utils.c \
	src/builtin/cd.c \
	src/builtin/cd_util.c \
	src/builtin/echo.c \
	src/builtin/env.c \
	src/builtin/env_utils.c \
	src/builtin/exit.c \
	src/builtin/export.c \
	src/builtin/export_utils.c \
	src/builtin/export_utils_01.c \
	src/builtin/export_utils_02.c \
	src/builtin/export_utils_03.c \
	src/builtin/pwd.c \
	src/builtin/unset.c \
	src/exec/child.c \
	src/exec/exec.c \
	src/exec/exec_utils_00.c \
	src/exec/exec_utils_01.c \
	src/exec/general_utils.c \
	src/exec/parent.c \
	src/exec/path.c \
	src/exec/path_utils.c \
	src/exec/pipe.c \
	src/exec/pipe_utils_00.c \
	src/exec/pipe_utils_01.c \
	src/exec/pipe_utils_02.c \
	src/exec/prexec.c \
	src/exec/prexec_util.c \
	src/exec/prexec_util_01.c \
	src/exec/redir.c \
	src/exec/redir_util.c \
	src/expansion/exp.c \
	src/expansion/exp_utils.c \
	src/expansion/heredoc.c \
	src/expansion/heredoc_init.c \
	src/expansion/heredoc_io.c \
	src/expansion/heredoc_util.c \
	src/lexer/lex.c \
	src/lexer/lex_quote.c \
	src/lexer/lex_token.c \
	src/lexer/lex_word.c \
	src/parser/parse.c \
	src/parser/parse_cmd_util.c \
	src/parser/parse_node.c \
	src/parser/parser_arg.c \
	src/parser/parser_cmd.c \
	src/parser/parser_token.c \
	src/signal/signal.c \
	src/signal/signal_util.c \
	src/utils/cleanup.c \
	src/utils/cleanup_fd.c \
	src/utils/cleanup_node.c \
	src/utils/cleanup_token.c \
	src/utils/init.c \
	src/utils/main_utils.c \
	src/utils/print.c

OBJ_DIR = ./obj
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT_LIB)
	@$(CC) $(CFLAGS) $(OBJ) -o $@ -L$(LIBFT_DIR) -lft -lreadline

$(LIBFT_LIB): $(LIBFT_DIR)/Makefile
	@$(MAKE) -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@) 
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(MAKE) -C $(LIBFT_DIR) clean
	@rm -rf $(OBJ_DIR)

fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@rm -f $(NAME)
	@rm -rf $(OBJ_DIR)

re: fclean all

.PHONY: all clean fclean re debug

