#ifndef CODEGEN_H
#define CODEGEN_H



// Opcodes for intermediate code and target code generation
enum opcode {
    ADD, SUB, MUL, DIV, MOD,
    ASSIGN, GOTO,
    IF_EQ, IF_NE, IF_LT, IF_GT, IF_LE, IF_GE,
    LD, LDI, ST, MOV, CMP, JE, JNE, JL, JG, JLE, JGE
};

// Structure for symbol table
struct symbol {
    char *name;
    int offset;
};

// Structure for intermediate code quads
struct quad {
    enum opcode op;
    char *arg1;
    char *arg2;
    char *result;
};

// Structure for basic blocks
struct basic_block {
    int start;
    int end;
};

// Structure for register descriptor
struct reg_descriptor {
    int is_free;
    char *var_name;
    int modified;
};

// External declarations
extern struct quad quads[1000];
extern int quad_index;
extern struct symbol symbols[100];
extern int symbol_count;

// Function declarations
void generate_code(int num_registers);
void print_intermediate_code();
void print_basic_blocks();
void print_target_code();
int is_leader(int index);
void identify_basic_blocks();
int allocate_register(char *var, struct reg_descriptor *regs, int num_regs);
void free_register(int reg, struct reg_descriptor *regs);
void store_variables(struct reg_descriptor *regs, int num_regs);
int get_reg_containing(char *var, struct reg_descriptor *regs, int num_regs);
void emit_target(enum opcode op, const char* dest, const char* src1, const char* src2);
const char* opcode_to_str(enum opcode op);
char* get_register(int reg_num);

#endif