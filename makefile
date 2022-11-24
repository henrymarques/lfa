parser: lex.o parser.o lex.h parser.h
	gcc parser.o lex.o -g -o parser -ansi -Wall -pedantic
lex.o: lex.h lex.c
	gcc -c lex.c -g
parser.o: parser.h parser.c
	gcc -c parser.c -g
clean:
	rm *.o
