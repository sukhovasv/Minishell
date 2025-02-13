CC = cc
CFLAGS = -DDEBUG -Wall -Wextra -Werror

# Directories
INCLUDE_DIR = ./include
LIBFT_DIR = ./lib/libft
OBJ_DIR = ./obj
BIN_DIR = ./bin
SRC_DIR = ./src

# Source files
SRC_FILES = \
	$(SRC_DIR)/main.c $(SRC_DIR)/environment/env_variables.c \
	$(SRC_DIR)/executor/executor.c $(SRC_DIR)/executor/external_executor.c \
	$(SRC_DIR)/executor/path_utils.c $(SRC_DIR)/executor/pipe_executor.c \
	$(SRC_DIR)/heredoc/heredoc_handlers.c $(SRC_DIR)/heredoc/heredoc_init.c \
	$(SRC_DIR)/heredoc/heredoc_cleanup.c $(SRC_DIR)/heredoc/heredoc_utils.c \
	$(SRC_DIR)/heredoc/heredoc_process.c  \
	$(SRC_DIR)/signals/signals.c \
	$(SRC_DIR)/lexer/token_handlers.c $(SRC_DIR)/lexer/tokenizer.c \
	$(SRC_DIR)/builtins/builtins_cd_pwd_echo.c $(SRC_DIR)/builtins/builtins_env.c \
	$(SRC_DIR)/builtins/builtins_export.c $(SRC_DIR)/builtins/builtins_utils.c \
	$(SRC_DIR)/builtins/builtins_unset.c $(SRC_DIR)/builtins/builtins_env_utils.c \
	$(SRC_DIR)/builtins/builtins_export_no_arg.c $(SRC_DIR)/builtins/builtins.c \
	$(SRC_DIR)/parser/ast_builder.c $(SRC_DIR)/parser/ast_cleanup.c \
	$(SRC_DIR)/parser/ast_nodes.c $(SRC_DIR)/parser/ast_args.c \
	$(SRC_DIR)/parser/ast_redirects.c \
	$(SRC_DIR)/utils/fd_utils.c $(SRC_DIR)/utils/quote_handlers.c \
	$(SRC_DIR)/utils/quote_utils.c $(SRC_DIR)/utils/redirect_handlers.c \

# Object files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Libraries
LIBFT = $(LIBFT_DIR)/libft.a
LIBS = -L$(LIBFT_DIR) -lft -lreadline

# Targets
all: directories $(BIN_DIR)/minishell

run: all
	./$(BIN_DIR)/minishell

directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

$(BIN_DIR)/minishell: $(OBJ_FILES) $(LIBFT)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Rules
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo Building $@
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(HEADER) -I$(INCLUDE_DIR) -c $< -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	$(RM) -r $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) -r $(BIN_DIR)
	$(RM) ./minishell
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re directories run
