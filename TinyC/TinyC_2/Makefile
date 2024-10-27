# Makefile for ass4_22CS10020_22CS10048 project

# Compiler and flags
CC = gcc
CFLAGS = -g -Wall

# Source files
LEX_SRC = ass4_22CS10020_22CS10048.l
YACC_SRC = ass4_22CS10020_22CS10048.y

# Generated files
LEX_OUTPUT = lex.yy.c
YACC_OUTPUT = ass4_22CS10020_22CS10048.tab.c
YACC_HEADER = ass4_22CS10020_22CS10048.tab.h

# Object files
OBJ_FILES = $(YACC_OUTPUT) $(LEX_OUTPUT)

# Target executable
TARGET = ass4_22CS10020_22CS10048

# Default target
all: $(TARGET) run

# Linking the final executable
$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ -lfl

# Rule to generate Yacc output
$(YACC_OUTPUT) $(YACC_HEADER): $(YACC_SRC)
	bison -d $(YACC_SRC)

# Rule to generate Lex output
$(LEX_OUTPUT): $(LEX_SRC) $(YACC_HEADER)
	flex $(LEX_SRC)

# Rule to run the parser and redirect output to abc.txt
run: $(TARGET)
	./$(TARGET) ass4_22CS10020_22CS10048_test.c > ass4_22CS10020_22CS10048.txt

# Clean up generated files
clean:
	rm -f $(TARGET) $(OBJ_FILES) $(YACC_HEADER) abc.txt

.PHONY: all clean run