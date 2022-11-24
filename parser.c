/*
 parser.c : analisador sintatico (exemplo de automato com pilha)
 Autor: Edna A. Hoshino

Gramática da linguagem aceita pelo parser:

 S         -> Function S_ 
 S_        -> Function S_ 
            | epsilon
 Function  -> Type id() { B }
 Type      -> float 
            | int
 B         -> C B 
            | return E ;
 C         -> id = E ; 
            | Type id;
 E         -> T E_
 E_        -> + T E_ 
            | epsilon
 T         -> K T_
 T_        -> * K T_ 
            | epsilon
 K         -> F K_
 K_	       -> & F K_
 F         -> ( E ) 
            | id X
            | num
 X         -> ()
            | epsilon  

 A saida do analisador apresenta o total de linhas processadas e uma mensagem de erro ou sucesso. 
 Atualmente, nao ha controle sobre a coluna e a linha em que o erro foi encontrado.
*/

#include <stdio.h>
#include <stdlib.h>
#include "lex.h"
#include "parser.h"

/* variaveis globais */
int lookahead;
FILE *fin;

/* Exige que o próximo terminal seja t e avança o ponteiro da fita de entrada (i.e., pega o próximo terminal) */
void match(int t)
{
  if(lookahead==t){
    lookahead=lex();
  }
  else{
     printf("\nErro(linha=%d): token %s (cod=%d) esperado.## Encontrado \"%s\" ##\n", lines, terminalName[t], t, lexema);
    exit(1);
  }
}

// S  -> Function S_ 
void S(){
  Function();
  S_();
}

/* S_ -> Function S_ 
      | epsilon
*/
void S_(){
  if(lookahead==INT || lookahead==FLOAT){
    Function();
    S_();
  }
}

// Function -> Type id() { B }
void Function(){
   Type();
   match(ID);
   match(ABRE_PARENT);
   match(FECHA_PARENT);
   match(ABRE_CHAVES);
   B();
   match(FECHA_CHAVES);
}

/* Type -> int | float
*/
void Type(){  
  if(lookahead==INT){
    match(INT);
  }
  else{
    match(FLOAT);
  }
}

/* B -> C B 
      | return E ;
*/
void B(){
  if(lookahead==RETURN){
     match(RETURN);
     E();
     match(PONTO_VIRG);
  }
  else{
    C();
    B();
  }
}
/*
 C -> id = E ; 
      | Type id ; 
*/
void C(){
   if(lookahead==ID){ //id = E ;
      match(ID);
      match(OP_ATRIB);
      E();
      match(PONTO_VIRG);
   }
   else {
      Type();
      match(ID);
      match(PONTO_VIRG);
   }
}
// E-> T E_
void E(){
  T();
  E_();
}

// T -> K T_
void T(){
  K();
  T_();
}

//  K -> F K_
void K(){
	F();
	K_();
}

// T -> F T_
/*void T(){
  F();
  T_();
}*/

// E_ -> + T E_ | epsilon
void E_(){
  if(lookahead==OP_ADIT){
    match(OP_ADIT);
    T();
    E_();
  }
}


/* K_-> & F K_
      | epsilon
*/
void K_(){
  if(lookahead==BIT_E){
    match(BIT_E);
    F();
    K_();
  }
}

/* T_-> * K T_
      | epsilon
*/
void T_(){
  if(lookahead==OP_MULT){
    match(OP_MULT);
    K();
    T_();
  }
}

/*
 F -> (E) 
      | id X
      | num
*/
void F(){
  if(lookahead==ABRE_PARENT){
    match(ABRE_PARENT);
    E();
    match(FECHA_PARENT);
  }
  else{
    if(lookahead==ID){
      match(ID);
      X();
    }
    else
      match(NUM);
  }
}
/*
 X -> () 
      | epsilon
*/
void X(){
  if(lookahead==ABRE_PARENT){
    match(ABRE_PARENT);
    match(FECHA_PARENT);
  }
}

/*******************************************************************************************
 parser(): 
 - efetua o processamento do automato com pilha AP
 - devolve uma mensagem para indicar se a "palavra" (programa) estah sintaticamente correta.
********************************************************************************************/
char *parser()
{
   lookahead=lex(); // inicializa lookahead com o primeiro terminal da fita de entrada (arquivo)
   S(); // chama a variável inicial da gramática.
   if(lookahead==FIM)
      return("Programa sintaticamente correto!");
   else
      return("Fim de arquivo esperado");
}

int main(int argc, char**argv)
{
  if(argc<2){
    printf("\nUse: compile <filename>\n");
    return 1;
  }
  else{
    printf("\nAnalisando lexica e sintaticamente o programa: %s", argv[1]);
    fin=fopen(argv[1], "r");
    if(!fin){
      printf("\nProblema na abertura do programa %s\n", argv[1]);
      return 1;
    }
    // chama o parser para processar o arquivo de entrada
    printf("\nTotal de linhas processadas: %d\nResultado: %s\n", lines, parser());
    fclose(fin);
    return 0;
  }
}

