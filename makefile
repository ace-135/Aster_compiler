# all:
# 	gcc main.c Src/lexer.c -IInclude -o main

CC = gcc
CFLAGS = -Wall -std=c17
SRC_DIR = Src
INC_DIR = Include
OBJ_DIR = obj
EXEC = compiler

SRC_FILES = main.c $(SRC_DIR)/lexer.c
OBJ_FILES = $(OBJ_DIR)/main.o $(OBJ_DIR)/lexer.o

# Create obj dir if doesn't exist
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# compile the project
$(EXEC): $(OBJ_DIR) $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ_FILES)

# compile main.c
$(OBJ_DIR)/main.o: main.c $(INC_DIR)/lexer.h
	$(CC) $(CFLAGS) -I$(INC_DIR) -c main.c -o $(OBJ_DIR)/main.o

# compile lexer.c
$(OBJ_DIR)/lexer.o: $(SRC_DIR)/lexer.c $(INC_DIR)/lexer.h
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $(SRC_DIR)/lexer.c -o $(OBJ_DIR)/lexer.o

clean:
	rm -rf $(OBJ_DIR) $(EXEC)

SRC = hello.ast
# execute
run: $(EXEC)
	./$(EXEC) $(SRC)
