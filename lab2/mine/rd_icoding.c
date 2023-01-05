#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylex();
extern int yylval;
extern char* yytext;

int factor();
int expr();
int term();

int tok;

void advance()
{
    tok = yylex();
    printf("tok: %s\n", yytext);
}
