#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "expr.h"
#include "expr.tab.h"

extern int yyparse(void);
extern FILE* yyin;

#define MAX_REGISTERS 12
#define MAX_MEMORY 65536
#define DEBUG 1  // Set to 0 to disable debug output

int next_register = 2;
int next_memory = 0;
int temp_memory = 1000;
FILE *output_file;

typedef struct {
    char *name;
    int offset;
} Symbol;

Symbol symbol_table[100];
int symbol_count = 0;

void debug_print(const char* format, ...) {
    if (DEBUG) {
        va_list args;
        va_start(args, format);
        fprintf(stderr, "DEBUG: ");
        vfprintf(stderr, format, args);
        fprintf(stderr, "\n");
        va_end(args);
    }
}

void init_output_file() {
    output_file = fopen("intcode.c", "w");
    fprintf(output_file, "#include <stdio.h>\n");
    fprintf(output_file, "#include <stdlib.h>\n");
    fprintf(output_file, "#include \"aux.c\"\n\n");
    fprintf(output_file, "int main() {\n");
    fprintf(output_file, " int R[%d];\n", MAX_REGISTERS);
    fprintf(output_file, " int MEM[%d];\n\n", MAX_MEMORY);
    debug_print("Initialized output file");
}

void close_output_file() {
    fprintf(output_file, " return 0;\n");
    fprintf(output_file, "}\n");
    fclose(output_file);
    debug_print("Closed output file");
}

int get_symbol_offset(char *name) {
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_table[i].name, name) == 0) {
            return symbol_table[i].offset;
        }
    }
    symbol_table[symbol_count].name = strdup(name);
    symbol_table[symbol_count].offset = next_memory++;
    symbol_count++;
    debug_print("Added new symbol: %s at offset %d", name, symbol_table[symbol_count - 1].offset);
    return symbol_table[symbol_count - 1].offset;
}

int get_register() {
    if (next_register < MAX_REGISTERS) {
        debug_print("Allocated register R[%d]", next_register);
        return next_register++;
    }
    debug_print("No more registers available");
    return -1;
}

void release_register() {
    if (next_register > 2) {
        next_register--;
        debug_print("Released register R[%d]", next_register);
    }
}

int generate_expr(exprNode *node) {
    debug_print("Generating expression for node type: %d", node->type);
    if (node->type == CONST_NODE) {
        int reg = get_register();
        fprintf(output_file, " R[%d] = %d;\n", reg, node->value);
        debug_print("Generated constant: R[%d] = %d", reg, node->value);
        return reg;
    } else if (node->type == ID_NODE) {
        int offset = get_symbol_offset(node->name);
        int reg = get_register();
        fprintf(output_file, " R[%d] = MEM[%d];\n", reg, offset);
        debug_print("Generated ID access: R[%d] = MEM[%d] for %s", reg, offset, node->name);
        return reg;
    } else if (node->type == OP_NODE) {
        debug_print("Generating operation: %c", node->opNode.op);
        int left_reg = generate_expr(node->opNode.left);
        int right_reg = generate_expr(node->opNode.right);
        int result_reg = left_reg;
        switch (node->opNode.op) {
            case '+':
                fprintf(output_file, " R[%d] = R[%d] + R[%d];\n", result_reg, left_reg, right_reg);
                break;
            case '-':
                fprintf(output_file, " R[%d] = R[%d] - R[%d];\n", result_reg, left_reg, right_reg);
                break;
            case '*':
                fprintf(output_file, " R[%d] = R[%d] * R[%d];\n", result_reg, left_reg, right_reg);
                break;
            case '/':
                fprintf(output_file, " R[%d] = R[%d] / R[%d];\n", result_reg, left_reg, right_reg);
                break;
            case '%':
                fprintf(output_file, " R[%d] = R[%d] %% R[%d];\n", result_reg, left_reg, right_reg);
                break;
            case '^':
                fprintf(output_file, " R[%d] = pwr(R[%d], R[%d]);\n", result_reg, left_reg, right_reg);
                break;
        }
        debug_print("Generated operation: R[%d] = R[%d] %c R[%d]", result_reg, left_reg, node->opNode.op, right_reg);
        release_register();
        return result_reg;
    }
    debug_print("Error: Unknown node type");
    return -1;
}

void generate_set_stmt(char *id, exprNode *expr) {
    debug_print("Generating SET statement for %s", id);
    int offset = get_symbol_offset(id);
    int reg = generate_expr(expr);
    fprintf(output_file, " MEM[%d] = R[%d];\n", offset, reg);
    fprintf(output_file, " mprn(MEM, %d);\n", offset);
    debug_print("Generated SET: MEM[%d] = R[%d]", offset, reg);
    release_register();
}

void generate_expr_stmt(exprNode *expr) {
    debug_print("Generating expression statement");
    int reg = generate_expr(expr);
    fprintf(output_file, " eprn(R, %d);\n", reg);
    debug_print("Generated expression statement: eprn(R, %d)", reg);
    release_register();
}

int main(int argc, char **argv) {
    debug_print("Starting parser");
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (yyin == NULL) {
            debug_print("Cannot open file %s", argv[1]);
            printf("Cannot open file %s\n", argv[1]);
            return 1;
        }
        debug_print("Opened input file: %s", argv[1]);
    } else {
        yyin = stdin;
        debug_print("Reading from stdin");
    }
    
    init_output_file();
    
    yyparse();
    debug_print("Finished parsing");
    
    close_output_file();
    
    if (yyin != stdin) {
        fclose(yyin);
        debug_print("Closed input file");
    }
    
    debug_print("Parser completed successfully");
    return 0;
}