SRC_DIR=src
BIN_DIR=bin
OBJ_DIR=$(BIN_DIR)/obj

exec = $(BIN_DIR)/jsh

source = $(shell find $(SRC_DIR) -name *.c)
objects = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(source))

cflags = -g
lflags = -g -ggdb -fsanitize=address -lncurses

$(exec): $(objects)
	gcc $(lflags) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c mkdirs
	@mkdir -p $(@D)
	gcc $(cflags) -c -o $@ $<

mkdirs:
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)

clean:
	@rm -rf $(BIN_DIR)
