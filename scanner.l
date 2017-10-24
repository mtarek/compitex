/***************************************************************************
 *  
 *                    ___                _ _____   __  __
 *                   / __|___ _ __  _ __(_)_   _|__\ \/ /
 *                  | (__/ _ \ '  \| '_ \ | | |/ -_)>  < 
 *                   \___\___/_|_|_| .__/_| |_|\___/_/\_\
 *                                 |_|                  
 *
 * Copyright (C) 2012 - 2017, Mohamed Tarek El-Haddad <mtarek16@gmail.com>.
 *
 * This software is licensed as described in the LICENSE file, which
 * you should have received as part of this distribution.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/
%{ 
#include <stdlib.h> 
#include <string.h>
#include "parser.h" 
#include "y.tab.h" 
void yyerror(char *); 
int lineno=1;
%} 

digit   [0-9]
letter  [a-zA-Z]
real    {digit}+(\.{digit}+)?
res		"while"|"if"|"else"|"print"
bar		"\\bar"
var     ({letter}|"_")({letter}|{digit}|","|"_"|"\*")*
mul     "\\times"
frac    "\\frac"
sum		"\\sum_"
sqrt	"\\sqrt"
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

{mul}			return MUL;
{frac}			return FRAC;
{sum}			return SUM;
{sqrt}			return SQRT;
{bar}			return BAR;
"\\{"			return '{';
"\\}"			return '}';
 
{var}           { 
					yylval.str = strdup(yytext); 
                    return VARIABLE; 
				} 

{real}          { 
                    yylval.dValue = atof(yytext); 
                    return REAL; 
                } 

[-()<>=+/;\[\]{}^.]  { 
                    return *yytext; 
                } 

">="            return GE; 
"<="            return LE; 
"=="            return EQ; 
"!="            return NE; 
 [ \t\r]+                ;            /* ignore whitespace */ 
 [\n]			lineno++;

 .              {	
					char erstr[32];
					//sprintf(erstr, "@%d : Unknown character(%c)", lineno, *yytext);
					//yyerror(erstr); 
				}
%% 
int yywrap(void)
{ 
    return 1; 
}

int scan_init(FILE *out, FILE *in)
{
    yyout = out;
    yyin = in;
    return 0;
}
