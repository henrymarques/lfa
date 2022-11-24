/*
 lexv2.c : analisador lexico (automato finito determinístico sem uma tabela de transições. As transições do autômato finito determinístico estão codificadas diretamente no código do lex())
 Autor: Edna A. Hoshino
 Em: novembro/2022
*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lex.h"

/* variaveis globais */
char lexema[81]; /* armazena a palavra processada pelo automato */
int ilexema;    /* indice usado para atualizar lexema */
int lines=1, currlines=0;       /* contador de linhas processadas */
char c;          /* armazena caracter lido da entrada */

// Simbolos validos na entrada (alfabeto do AFD):
// '\t', '\n', ' ', '(', ')', '*', '!',
//  '+', ';', '<', '=', '>', '{', '}'

char *keywords[] = { /* palavras reservadas da linguagem */
  "double", "float", "if", "int", "return", "while"
};


char *terminalName[] = { /* nome dos terminais aceitos pelo automato */
  "double", "float","if", "int", "return", "while", "id", "num", "OP_ATRIB", "BIT_E", "OP_ADIT", "OP_MULT", "ABRE_PARENT", "FECHA_PARENT", "PONTO_VIRG", 
  "ABRE_CHAVES", "FECHA_CHAVES", "CARACTER", "OP_REL", "FIM"
};


/********************************************************************************
 isKeyword(): 
 - verifica se s eh uma palavra reservada
 - devolve o codigo da keyword s ou o codigo ID p/ indicar que s eh uma variavel.
*********************************************************************************/
char isKeyword (char *s)
{
  int i, f, m;
  int x;

  i=0; f=nKeywords-1;
  while(i<=f){
    m=(i+f)/2;
    if((x=strcmp(s,keywords[m]))>0){
      i=m+1;
    }
    else if(x<0){
      f=m-1;
    }
    else{
      return m;
    }
  }
  return ID;
}

/********************************************************************************
 lex(): 
 - realiza o processamento do AFD considerando o arquivo fin como fita de entrada
 - devolve codigo da palavra aceita pelo AFD (ou codigo de erro)
*********************************************************************************/
int lex()
{
  int estado_atual, prox_estado, next, t;

  ilexema =0;
  estado_atual=0;

  do{
    c=getc(fin);
    lexema[ilexema++]=c;
    lexema[ilexema]='\0';
    switch(estado_atual){
    case 0:
      switch(c){
      case '\n':
        lines++;
      case '\t':
      case ' ':
        ilexema--;
        break;
      case '(':
        return ABRE_PARENT;
      case ')':
        return FECHA_PARENT;
      case '*':
        return OP_MULT;
      case '+':
        return OP_ADIT;
	  case '&':
	    return BIT_E;
      case ';':
        return PONTO_VIRG;
      case '{':
        return ABRE_CHAVES;
      case '}':
        return FECHA_CHAVES;
      case '\'':
        estado_atual = 1;
        break;
      case '=':
        estado_atual = 5;
        break;
      case '!':
      case '>':
      case '<':
        estado_atual = 6;
        break;
      default:
        if(isdigit(c)){
          estado_atual = 3;
        }
        else if(isalpha(c)){
          estado_atual = 4;
        }
        else if(!feof(fin))
          return ERRO_LEXICO;
        break;
      }
      break;
    case 1:
      if(c!='\'' && c!='\n' && c!='\t'){
        estado_atual = 2;
      }
      else
        return ERRO_LEXICO;
      break;
    case 2:
      if(c=='\''){
        return CARACTER;
      }
      else
        return ERRO_LEXICO;      
    case 3:
      if(!isdigit(c)){
        ungetc(c,fin);
        lexema[--ilexema]='\0';
        return NUM;
      }
      break;
    case 4:
      if(!isalpha(c) && !isdigit(c)){
        ungetc(c,fin);
        lexema[--ilexema]='\0';
        return (isKeyword(lexema));
      }
      break;     
    case 5:
      if(c!='='){
        ungetc(c,fin);
        lexema[--ilexema]='\0';
        return OP_ATRIB;
      }
      else
        return OP_REL;
    case 6:
      if(c!='='){
        ungetc(c,fin);
        lexema[--ilexema]='\0';
        return OP_REL;
      }
      else
        return OP_REL;
    }
  } while(!feof(fin));
  if(estado_atual==0){ /* fim de arquivo */
    lexema[0]='\0';
    return FIM;
  }
  return ERRO_LEXICO;
}
