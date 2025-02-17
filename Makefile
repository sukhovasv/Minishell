NAME		= minishell

CC 			= cc
#DEBUG_FLAGS	:= -O0 -g3 -gdwarf-3 -fsanitize=address -fsanitize=undefined
DEBUG_FLAGS	:= -O0 -g3 -gdwarf-3
CFLAGS 		= -DDEBUG -Wall -Wextra -Werror

# Directories
INCLUDE_DIR = ./include
LIBFT_DIR 	= ./lib/libft
OBJ_DIR		= ./obj
SRC_DIR		= ./src

# Source files
SRC_FILES	= \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/executor/external_executor.c $(SRC_DIR)/executor/pipe_node_executor.c \
	$(SRC_DIR)/executor/ast_executor.c $(SRC_DIR)/executor/redirections_executor.c \
	$(SRC_DIR)/executor/pipe_command_executor.c \
	$(SRC_DIR)/heredoc/heredoc_handlers.c $(SRC_DIR)/heredoc/heredoc_init.c \
	$(SRC_DIR)/heredoc/heredoc_utils.c $(SRC_DIR)/heredoc/heredoc_utils_2.c \
	$(SRC_DIR)/heredoc/heredoc_process.c $(SRC_DIR)/heredoc/heredoc_content.c \
	$(SRC_DIR)/heredoc/heredoc_temp_file.c $(SRC_DIR)/heredoc/heredoc_cleanup.c \
	$(SRC_DIR)/signals/signals.c \
	$(SRC_DIR)/lexer/token_handlers.c $(SRC_DIR)/lexer/tokenizer.c \
	$(SRC_DIR)/lexer/token_basic.c $(SRC_DIR)/lexer/token_creation.c \
	$(SRC_DIR)/lexer/token_env.c $(SRC_DIR)/lexer/token_operator.c \
	$(SRC_DIR)/lexer/token_utils.c $(SRC_DIR)/lexer/token_word.c \
	$(SRC_DIR)/builtins/builtins_echo_exit.c $(SRC_DIR)/builtins/builtins_cd_pwd.c \
	$(SRC_DIR)/builtins/builtins_env.c $(SRC_DIR)/builtins/builtins_env_init.c \
	$(SRC_DIR)/builtins/builtins_export.c $(SRC_DIR)/builtins/builtins_utils.c \
	$(SRC_DIR)/builtins/builtins_unset.c $(SRC_DIR)/builtins/builtins_env_utils.c \
	$(SRC_DIR)/builtins/builtins_export_no_arg.c $(SRC_DIR)/builtins/builtins.c \
	$(SRC_DIR)/builtins/builtins_utils_2.c \
	$(SRC_DIR)/parser/ast_builder.c $(SRC_DIR)/parser/ast_cleanup.c \
	$(SRC_DIR)/parser/ast_nodes.c $(SRC_DIR)/parser/ast_args.c \
	$(SRC_DIR)/parser/ast_redirects.c \
	$(SRC_DIR)/utils/fd_utils.c $(SRC_DIR)/utils/quote_handlers.c \
	$(SRC_DIR)/utils/quote_utils.c $(SRC_DIR)/utils/redirect_handlers.c \
	$(SRC_DIR)/utils/free_minishell.c \

# Object files
OBJ_FILES	= $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Libraries
LIBFT		= $(LIBFT_DIR)/libft.a
LIBS		= -L$(LIBFT_DIR) -lft -lreadline

# Targets
all: $(NAME)

$(NAME): $(LIBFT) $(OBJ_FILES)
		$(CC) $(CFLAGS) -o $@ $^ $(DEBUG_FLAGS) $(LIBS)

run: all
		./minishell

# Rules
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo Building $@
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(HEADER) $(DEBUG_FLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR) --no-print-directory

clean:
	@$(RM) -r $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) --no-print-directory clean

fclean: clean
	@$(RM) ./minishell
	@$(MAKE) -C $(LIBFT_DIR) --no-print-directory fclean

re: fclean all

.PHONY: all clean fclean re run
