# Makefile


all: clean lex gcc run


clean:
	rm -f *.o *.out a.out output.txt


lex: ass3_22CS10020_22CS10048.l
	flex ass3_22CS10020_22CS10048.l

# Compile the C file generated by flex along with temp.c
gcc: lex ass3_22CS10020_22CS10048.l
	gcc ass3_22CS10020_22CS10048.c -o a.out

# Run the output binary
run: gcc
	./a.out >output.txt