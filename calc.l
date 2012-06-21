%{ 
#include <stdlib.h> 
#include <string.h>
#include "calc.h" 
#include "y.tab.h" 
void yyerror(char *); 
%} 

digit   [0-9]
letter  [a-zA-Z]
real    {digit}+(\.{digit}+)?
res		"while"|"if"|"else"|"print"
var     ({letter}|"_")({letter}|{digit}|","|"_")*
%% 

{res}			{
					if(!strcmp(yytext, "while"))
						return WHILE;
					if(!strcmp(yytext, "if"))
						return IF;
					if(!strcmp(yytext, "else"))
						return ELSE;
					if(!strcmp(yytext, "print"))
						return PRINT;
				}
 
{var}           { 
                          yylval.str = strdup(yytext); 
                          return VARIABLE; 
				} 

{real}            { 
                          yylval.dValue = atof(yytext); 
                          return REAL; 
                } 

[-()<>=+*/;{}.] { 
                          return *yytext; 
                } 

">="                       return GE; 
"<="                       return LE; 
"=="                       return EQ; 
"!="                       return NE; 
 [ \t\r\n]+                ;            /* ignore whitespace */ 

 .                         {	
								char erstr[32];
								sprintf(erstr, "Unknown character(%c)", *yytext);
								yyerror(erstr); }
%% 
int yywrap(void) { 
       return 1; 
} 
