#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int pwr(int arg1, int arg2) {
    return (int)pow(arg1, arg2);
}

void mprn(int *MEM, int idx) {
    printf("+++ MEM[%d] set to %d\n", idx, MEM[idx]);
}

void eprn(int *R, int idx) {
    printf("+++ Standalone expression evaluates to %d\n", R[idx]);
}