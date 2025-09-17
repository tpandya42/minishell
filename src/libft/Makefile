# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: albetanc <albetanc@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/05 17:34:53 by albetanc          #+#    #+#              #
#    Updated: 2025/09/15 15:04:29 by albetanc         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc

CFLAGS = -Wall -Wextra -Werror

NAME = libft.a

#main directory
SRC_DIR = libft

#subdirectories
PRINTF_DIR = ft_printf
GNL_DIR = gnl

# Output directory for .o files
OBJ_DIR = obj

SRC_LIBFT =  \
ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c ft_strlen.c \
ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c ft_strlcpy.c ft_strlcat.c \
ft_toupper.c ft_tolower.c ft_strchr.c ft_strrchr.c ft_strncmp.c ft_memchr.c \
ft_memcmp.c ft_strnstr.c ft_atoi.c ft_calloc.c ft_strdup.c ft_split.c ft_substr.c \
ft_strjoin.c ft_strtrim.c ft_itoa.c ft_strmapi.c ft_striteri.c ft_putchar_fd.c \
ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd_sig.c ft_putnbr_fd_un.c \
ft_putnbr_base_sig.c ft_putnbr_base_un.c safe_write.c ft_strncpy.c ft_strcpy.c \
ft_strcat.c

SRC_PRINTF = $(addprefix $(PRINTF_DIR)/, \
ft_printf.c ft_printf_char.c ft_printf_str.c ft_printf_int.c \
ft_printf_hexa.c ft_printf_ptr.c ft_printf_unsign.c)

SRC_GNL = $(addprefix $(GNL_DIR)/, \
get_next_line.c get_next_line_utils.c)

SRC_OTHERS = open_fd.c close_fd.c free_array.c ft_strcmp.c

SRC = $(SRC_LIBFT) $(SRC_PRINTF) $(SRC_GNL) $(SRC_OTHERS)

#  OBJ_LIBFT = $(SRC_LIBFT:.c=.o)
#  OBJ_PRINTF = $(SRC_PRINTF:.c=.o)
#  OBJ_GNL = $(SRC_GNL:.c=.o)
#  OBJ_OTHERS = $(SRC_OTHERS:.c=.o)

# OBJ = $(OBJ_LIBFT) $(OBJ_PRINTF) $(OBJ_GNL) $(OBJ_OTHERS)

OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

all: $(NAME)

$(NAME): $(OBJ)
	@ar rcs $@ $^

#compiles each .c with it's corresponding .o in /obj
$(OBJ_DIR)/%.o:%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
