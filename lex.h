#ifndef __LEX
#define __LEX

#include <stdio.h>

enum boolean {FALSE=0, TRUE};

enum tKeyword {DOUBLE=0, FLOAT, IF, INT, RETURN, WHILE, nKeywords};
enum tTerminal {ID=nKeywords, NUM, OP_ATRIB, OP_ADIT, OP_MULT, ABRE_PARENT, FECHA_PARENT, PONTO_VIRG, ABRE_CHAVES, FECHA_CHAVES, CARACTER, OP_REL,FIM, nTerminal};

enum tErros{ERRO_LEXICO=nTerminal, ERRO_SINTATICO};
extern int lines;
extern int currlines;
extern char lexema[81];
extern char *terminalName[];
extern FILE *fin;

char nextSymbol (FILE* fin);
char isKeyword (char *s);
int lex();

#endif
