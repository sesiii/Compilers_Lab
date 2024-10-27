# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -g

# Source files
LEX_SRC = TinyC3_22CS10020_22CS10048.l
YACC_SRC = TinyC3_22CS10020_22CS10048.y
TRANSLATOR_SRC = TinyC3_22CS10020_22CS10048_translator.cxx

# Output files
LEX_OUTPUT = lex.yy.c
YACC_OUTPUT = TinyC3_22CS10020_22CS10048.tab.c
PARSER = parser

# Test files
TEST_FILES = TinyC3_22CS10020_22CS10048_test1.c TinyC3_22CS10020_22CS10048_test2.c TinyC3_22CS10020_22CS10048_test3.c TinyC3_22CS10020_22CS10048_test4.c TinyC3_22CS10020_22CS10048_test5.c

# Default target
all: $(PARSER)

# Build parser executable
$(PARSER): $(YACC_OUTPUT) $(LEX_OUTPUT) $(TRANSLATOR_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $(YACC_OUTPUT) $(LEX_OUTPUT) $(TRANSLATOR_SRC) -lfl

# Generate lexer and parser code
$(LEX_OUTPUT): $(LEX_SRC)
	flex $(LEX_SRC)

$(YACC_OUTPUT): $(YACC_SRC)
	bison -d $(YACC_SRC)

# Run tests
run_tests: $(PARSER)
	@for file in $(TEST_FILES); do \
		output_file="$${file%.c}_quads.out"; \
		./$(PARSER) < $$file > $$output_file; \
		echo "Processed $$file -> $$output_file"; \
	done

# Clean up
clean:
	rm -f $(PARSER) $(YACC_OUTPUT) $(LEX_OUTPUT) *.o *.out *tab.h