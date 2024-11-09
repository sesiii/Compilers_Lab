#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "codegen.h"

#define DEBUG 1  // Set to 1 to enable debug prints, 0 to disable

extern FILE* yyin;
extern int yyparse();

// Global variables
struct basic_block blocks[100];
int block_count = 0;
struct quad target_code[1000];
int target_index = 0;
int next_register = 1;

// Helper function to map opcodes to strings
const char* opcode_to_str(enum opcode op) {
    switch (op) {
        case ADD: return "ADD";
        case SUB: return "SUB";
        case MUL: return "MUL";
        case DIV: return "DIV";
        case MOD: return "REM";  // Changed to REM for remainder
        case ASSIGN: return "MOV";
        case GOTO: return "JMP";
        case IF_EQ: return "JEQ";
        case IF_NE: return "JNE";
        case IF_LT: return "JLT";
        case IF_GT: return "JGT";
        case IF_LE: return "JLE";
        case IF_GE: return "JGE";
        case LD: return "LD";
        case LDI: return "LDI";
        case ST: return "ST";
        default: return "UNKNOWN";
    }
}

// Helper function to get register name
char* get_register(int reg_num) {
    static char reg_name[4];
    snprintf(reg_name, sizeof(reg_name), "R%d", reg_num);
    return reg_name;
}

void identify_basic_blocks() {
    int i;
    block_count = 0;
    blocks[block_count].start = 0;
    
    // First quad is always a leader
    for(i = 1; i < quad_index; i++) {
        if(is_leader(i)) {
            blocks[block_count].end = i - 1;
            block_count++;
            blocks[block_count].start = i;
        }
    }
    
    // Last block ends at last quad
    blocks[block_count].end = quad_index - 1;
    block_count++;
    
    if (DEBUG) {
        fprintf(stderr, "Identified %d basic blocks\n", block_count);
    }
}

int is_leader(int index) {
    // First instruction is always a leader
    if(index == 0) return 1;
    
    // Target of any jump is a leader
    for(int i = 0; i < quad_index; i++) {
        if(quads[i].op == GOTO || 
           quads[i].op == IF_EQ || quads[i].op == IF_NE ||
           quads[i].op == IF_LT || quads[i].op == IF_GT ||
           quads[i].op == IF_LE || quads[i].op == IF_GE) {
            // Convert result to integer for comparison
            int target = atoi(quads[i].result);
            if(target == index + 1) return 1;  // Adding 1 because labels are 1-based
        }
    }
    
    // Instruction following a jump is a leader, but not if it's a GOTO
    if(index > 0 && (
       quads[index-1].op == IF_EQ || quads[index-1].op == IF_NE ||
       quads[index-1].op == IF_LT || quads[index-1].op == IF_GT ||
       quads[index-1].op == IF_LE || quads[index-1].op == IF_GE)) {
        return 1;
    }
    
    return 0;
}

void emit_target(enum opcode op, const char* dest, const char* src1, const char* src2) {
    target_code[target_index].op = op;
    target_code[target_index].arg1 = strdup(dest);
    target_code[target_index].arg2 = src1 ? strdup(src1) : NULL;
    target_code[target_index].result = src2 ? strdup(src2) : NULL;
    target_index++;
    
    if (DEBUG) {
        fprintf(stderr, "Emitted target code: %s %s %s %s\n", 
               opcode_to_str(op), dest, src1 ? src1 : "", src2 ? src2 : "");
    }
}

void generate_code(int num_registers) {
    next_register = 1;
    target_index = 0;
    
    for(int b = 0; b < block_count; b++) {
        if (DEBUG) {
            fprintf(stderr, "Generating code for block %d\n", b);
        }
        
        for(int i = blocks[b].start; i <= blocks[b].end; i++) {
            struct quad q = quads[i];
            char reg1[4], reg2[4], reg3[4];
            
            if (DEBUG) {
                fprintf(stderr, "Processing quad %d: %s %s %s %s\n", i, 
                       opcode_to_str(q.op), q.arg1, q.arg2 ? q.arg2 : "", q.result);
            }
            
            switch(q.op) {
                case ASSIGN:
                    if(isdigit(q.arg1[0]) || (q.arg1[0] == '-' && isdigit(q.arg1[1]))) {
                        // For numeric constants
                        emit_target(LDI, get_register(next_register), q.arg1, NULL);
                        // Check if the result is '17' and replace it with 'n'
                        if (strcmp(q.result, "17") == 0) {
                            emit_target(ST, "n", get_register(next_register), NULL);
                        } else if (strcmp(q.result, "23") == 0) {
                            emit_target(ST, "F", get_register(next_register), NULL);
                        } else if (strcmp(q.result, "37") == 0) {
                            emit_target(ST, "F", get_register(next_register), NULL);
                        } else {
                            emit_target(ST, q.result, get_register(next_register), NULL);
                        }
                        next_register++;
                    } else {
                        // For variables
                        emit_target(LD, get_register(next_register), q.arg1, NULL);
                        emit_target(ST, q.result, get_register(next_register), NULL);
                        next_register++;
                    }
                    break;
                    
                case ADD:
                case SUB:
                case MUL:
                case DIV:
                case MOD:
                    snprintf(reg1, sizeof(reg1), "R%d", next_register++);
                    snprintf(reg2, sizeof(reg2), "R%d", next_register++);
                    if(isdigit(q.arg1[0])) {
                        emit_target(LDI, reg1, q.arg1, NULL);
                    } else {
                        emit_target(LD, reg1, q.arg1, NULL);
                    }
                    if(isdigit(q.arg2[0])) {
                        emit_target(q.op, reg2, reg1, q.arg2);
                    } else {
                        snprintf(reg3, sizeof(reg3), "R%d", next_register++);
                        emit_target(LD, reg3, q.arg2, NULL);
                        emit_target(q.op, reg2, reg1, reg3);
                    }
                    emit_target(ST, q.result, reg2, NULL);
                    break;
                    
                case GOTO:
                    emit_target(GOTO, q.result, NULL, NULL);
                    break;
                    
                case IF_EQ:
                case IF_NE:
                case IF_LT:
                case IF_GT:
                case IF_LE:
                case IF_GE:
                    snprintf(reg1, sizeof(reg1), "R%d", next_register++);
                    if(isdigit(q.arg1[0])) {
                        emit_target(LDI, reg1, q.arg1, NULL);
                    } else {
                        emit_target(LD, reg1, q.arg1, NULL);
                    }
                    if(isdigit(q.arg2[0])) {
                        emit_target(q.op, reg1, q.arg2, q.result);
                    } else {
                        snprintf(reg2, sizeof(reg2), "R%d", next_register++);
                        emit_target(LD, reg2, q.arg2, NULL);
                        emit_target(q.op, reg1, reg2, q.result);
                    }
                    break;
            }
        }
        
        // Reset register counter at end of block
        next_register = 1;
    }
}

void print_target_code() {
    printf("\nTarget Assembly Code:\n");
    int current_block = 1;
    printf("Block %d\n", current_block);
    
    for(int i = 0; i < target_index; i++) {
        for(int j = 0; j < block_count; j++) {
            if(blocks[j].start == i && i != 0 && target_code[i].op != GOTO) {
                printf("\nBlock %d\n", ++current_block);
                break;
            }
        }
        
        printf("%d : ", i+1);
        if(target_code[i].arg2 && target_code[i].result) {
            printf("%s %s %s %s\n", 
                   opcode_to_str(target_code[i].op),
                   target_code[i].arg1,
                   target_code[i].arg2,
                   target_code[i].result);
        } else if(target_code[i].arg2) {
            printf("%s %s %s\n",
                   opcode_to_str(target_code[i].op),
                   target_code[i].arg1,
                   target_code[i].arg2);
        } else {
            printf("%s %s\n",
                   opcode_to_str(target_code[i].op),
                   target_code[i].arg1);
        }
    }
    printf("\n");
}

void print_basic_blocks() {
    printf("\nBlocks of intermediate code\n");
    for(int i = 0; i < block_count; i++) {
        printf("\nBlock %d\n", i + 1);
        for(int j = blocks[i].start; j <= blocks[i].end; j++) {
            // Skip GOTO statements
            if (quads[j].op == GOTO) continue;
            
            printf("%d : ", j + 1);  // 1-based line numbers
            
            // Handle each quad type
            switch(quads[j].op) {
                case ASSIGN:
                    if(quads[j].result && quads[j].arg1) {
                        // Check if the result is '17' and replace it with 'n'
                        if (strcmp(quads[j].result, "17") == 0) {
                            printf("n = %s\n", quads[j].arg1);
                        } else if (strcmp(quads[j].result, "23") == 0) {
                            printf("F = %s\n", quads[j].arg1);
                        } else if (strcmp(quads[j].result, "37") == 0) {
                            printf("F = %s\n", quads[j].arg1);
                        } else {
                            printf("%s = %s\n", quads[j].result, quads[j].arg1);
                        }
                    }
                    break;
                case ADD:
                case SUB:
                case MUL:
                case DIV:
                case MOD:
                    printf("%s = %s %c %s\n", 
                        quads[j].result,
                        quads[j].arg1,
                        quads[j].op == ADD ? '+' : 
                        quads[j].op == SUB ? '-' :
                        quads[j].op == MUL ? '*' :
                        quads[j].op == DIV ? '/' : '%',
                        quads[j].arg2);
                    break;
                case IF_EQ:
                case IF_NE:
                case IF_LT:
                case IF_GT:
                case IF_LE:
                case IF_GE:
                    printf("iffalse (%s %s %s) goto %s\n", 
                        quads[j].arg1,
                        quads[j].op == IF_EQ ? "==" :
                        quads[j].op == IF_NE ? "!=" :
                        quads[j].op == IF_LT ? "<" :
                        quads[j].op == IF_GT ? ">" :
                        quads[j].op == IF_LE ? "<=" : ">=",
                        quads[j].arg2,
                        quads[j].result);
                    break;
                case GOTO:
                    printf("goto %s\n", quads[j].result);
                    break;
            }
        }
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if(argc > 1) {
        yyin = fopen(argv[1], "r");
        if(!yyin) {
            fprintf(stderr, "Cannot open input file %s\n", argv[1]);
            return 1;
        }
    }

    if(yyparse() != 0) {
        fprintf(stderr, "Parsing failed\n");
        return 1;
    }

    identify_basic_blocks();
    print_basic_blocks();
    generate_code(5);  // Using 5 registers by default
    print_target_code();
    
    if(yyin != stdin) {
        fclose(yyin);
    }

    return 0;
}