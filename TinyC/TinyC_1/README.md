# Lexical Analyzer for TinyC Language

This repository contains a Lexical Analyzer for the TinyC language.

## How to Run

Follow the steps below to run the Lexical Analyzer:

1. **Run Flex**  
   Generate the scanner using Flex:
   flex ass3_22CS10020_22CS10048.l

2. **Run C file**
    Generate the object file using GCC:
    gcc ass3_22CS10020_22CS10048.c

3. **Run object file**
    Run object file:
    ./a.out

   The c file (gcc ass3_22CS10020_22CS10048.c) will automatically pick the test file 