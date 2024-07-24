CC = cc
CFLAGS = -Wall -Wextra -Werror

# Directories
INCLUDE_DIR = ./include
LIBFT_DIR = ./lib/libft
OBJ_DIR = ./obj
BIN_DIR = ./bin
SRC_DIR = ./src

# Source files
SRC_FILES = \
	$(SRC_DIR)/main.c $(SRC_DIR)/pipex.c $(SRC_DIR)/find_path.c $(SRC_DIR)/find_path_utils.c $(SRC_DIR)/utils.c $(SRC_DIR)/here_doc.c $(SRC_DIR)/handle_process.c

# Object files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Libraries
LIBFT = $(LIBFT_DIR)/libft.a
LIBS = -L$(LIBFT_DIR) -lft

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
	$(RM) ./push_swap
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re directories run
