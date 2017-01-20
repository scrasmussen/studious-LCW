/**
 * Driver for scanner alone.  Reads source from 
 * standard input or named file, emits tokens (one per line) 
 * to standard output
 * 
 * Usage:  ./scanner someQuackCode.qk
 * 
 * Output format: 
 * 1  CLASS "class"
 * 1  IDENT "Sample"
 * 1  ( "("
 * ... 
 * 4  IDENT "x"
 * 4  ; ";"
 * 5  } "}"
 * 6  } "}"
 * 8  IDENT "z"
 * 8  = "="
 * 8  STRING_LIT ""strings can have \" escaped quotes and \. other escapes""
 * 
 * etc
 */

#include <stdio.h>
#include <stdlib.h>

#include "quack.tab.h"
#include "toknames.h"   /* Extracted from quack.tab.h */ 
#include "lex.yy.h"

extern void yyrestart(FILE *f);

static char* lookup_token(int token) {
  static char token_buf[2] = "@"; 
  for (int i=0; i < num_named_tokens; ++i) {
    // printf("Comparing %d to %d\n", token, 
    // 	   token_name_table[i].num);
    if (token == token_name_table[i].num) {
      return token_name_table[i].name;
    }
  }
  /* Not in the table ... maybe it's a 
   * single character punctuation token? 
   * We consider only printable characters. 
   */
  // printf("Not a named token; might be punctuation\n"); 
  if (token >= ' ' && token <= '}') {
    token_buf[0] = (char) token; 
    // printf("Returning as %s\n", token_buf); 
    return token_buf;
  }
  fprintf(stderr, "Illegal token %d\n", token);
  return "****BAD TOKEN***"; 
}

int main(int argc, char **argv) {
  FILE *f;
  // yydebug = 0; // Set to one to see each rule matched in scanner
  if (argc > 1) {
    if( !(f = fopen(argv[1], "r"))) {
	perror(argv[1]);
	exit(1);
      }
      yyrestart(f);
  }

  int token = yylex(); 
  char* name;
  while (token != EOF) {
    name = lookup_token(token); 
    printf("%d  %s \"%s\"\n", 
	   yylineno, name, yytext); 
    token = yylex(); 
  }
}
