NAME := libft.a
CC := cc
CFLAGS := -Wall -Wextra -Werror
SRC_FILES := ft_atoi.c mem/ft_bzero.c mem/ft_calloc.c ft_isalnum.c ft_isalpha.c \
			ft_isascii.c ft_isdigit.c ft_isprint.c ft_itoa.c \
			mem/ft_memchr.c mem/ft_memcmp.c mem/ft_memcpy.c mem/ft_memmove.c \
			mem/ft_memset.c ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
			ft_putstr_fd.c str/ft_split.c str/ft_strchr.c str/ft_strdup.c str/ft_striteri.c \
			str/ft_strjoin.c str/ft_strlcat.c str/ft_strlcpy.c str/ft_strlen.c str/ft_strmapi.c \
			str/ft_strncmp.c  str/ft_strncpy.c str/ft_strnstr.c str/ft_strrchr.c str/ft_strtrim.c str/ft_substr.c \
			ft_tolower.c ft_toupper.c ft_isint.c\
			lst/ft_lstnew.c lst/ft_lstadd_front.c lst/ft_lstsize.c lst/ft_lstlast.c \
            lst/ft_lstadd_back.c lst/ft_lstdelone.c lst/ft_lstclear.c \
			lst/ft_lstiter.c lst/ft_lstmap.c \
			handlers/flag_handler.c helpers/num_helpers.c \
            helpers/print_helpers.c str/ft_str_utils.c str/ft_strndup.c\
            ft_printf.c dllist/dllist1.c dllist/dlnode.c dllist/dllist_free.c \
            gnl/get_next_line.c gnl/gnl_utils.c
OBJ_DIR 	:= obj
OBJ_FILES 	= $(addprefix $(OBJ_DIR)/,$(SRC_FILES:.c=.o))
HEADER 		:= -I ../../include/
LIB_COMMAND := ar rcsP

RM 			:= rm -f

all: $(NAME)

$(OBJ_DIR)/%.o: %.c
	@echo Building $@
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(HEADER) -c $^ -o $@

$(OBJ_FILES): | $(OBJ_DIR)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(NAME): $(OBJ_FILES)
	$(LIB_COMMAND) $(NAME) $(OBJ_FILES)

clean:
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
