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
	$(SRC_DIR)/main.c $(SRC_DIR)/builtins/builtins.c \
	$(SRC_DIR)/execute/execute_command.c $(SRC_DIR)/execute/execute_external.c \
	$(SRC_DIR)/execute/path_utils.c $(SRC_DIR)/execute/redirections.c \
	$(SRC_DIR)/execute/redirect_handlers.c $(SRC_DIR)/execute/here_doc.c \
	$(SRC_DIR)/execute/here_doc_cleanup.c $(SRC_DIR)/execute/here_doc_utils.c \
	$(SRC_DIR)/execute/here_doc_init.c $(SRC_DIR)/execute/here_doc_process.c \
	$(SRC_DIR)/execute/utils_fd.c $(SRC_DIR)/execute/ast_executor.c\
	$(SRC_DIR)/signals.c \
	$(SRC_DIR)/builtins/builtins_cd_pwd_echo.c $(SRC_DIR)/builtins/builtins_env.c \
	$(SRC_DIR)/builtins/builtins_export.c $(SRC_DIR)/builtins/builtins_utils.c \
	$(SRC_DIR)/builtins/builtins_unset.c $(SRC_DIR)/builtins/builtins_env_utils.c \
	$(SRC_DIR)/parser/parse_quotes.c $(SRC_DIR)/tokenize.c \
	$(SRC_DIR)/parser/ast_node_creation.c $(SRC_DIR)/parser/ast_args.c \
	$(SRC_DIR)/parser/ast_parser_utilities.c $(SRC_DIR)/parser/ast_parser.c \
	$(SRC_DIR)/parser/ast_utils.c $(SRC_DIR)/parser/ast_redirect.c \
	$(SRC_DIR)/parser/env_handler.c \
#	$(SRC_DIR)/parser/handle_unclosed_quotes.c \
#	$(SRC_DIR)/tokens/tokens.c \
#	$(SRC_DIR)/tokens/special_char_token.c $(SRC_DIR)/tokens/tokenise.c \
#	$(SRC_DIR)/tokens/tokenise_phrase.c \
#	$(SRC_DIR)/validation/input_helper.c $(SRC_DIR)/validation/input_validator.c
	


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
