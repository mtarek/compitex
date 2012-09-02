%{ 
#include <stdlib.h> 
#include <string.h>
#include "compitex.h"
static int lineno=1;
%} 

digit   [0-9]
letter  [a-zA-Z]
var     ({letter}|"_")({letter}|{digit}|","|"_"|"\*")*
bar		("\\bar{")({var})("}")
sub		{var}"_{"{var}"}"
sup		{var}"^{"{var}"}"
g1	    ("\\alpha"|"\\theta"|"\\tau"|"\\beta"|"\\vartheta"|"\\pi")
g2		("\\upsilon"|"\\gamma"|"\\varpi"|"\\phi"|"\\delta"|"\\kappa"|"\\rho"|"\\varphi")
g3		("\\epsilon"|"\\lambda"|"\\varrho"|"\\chi"|"\\varepsilon"|"\\mu")
g4		("\\sigma"|"\\psi"|"\\zeta"|"\\nu"|"\\varsigma"|"\\omega"|"\\eta")
g5		("\\Gamma"|"\\Lambda"|"\\Sigma"|"\\Psi"|"\\Delta"|"\\Upsilon")
g6		("\\Omega"|"\\Theta"|"\\Pi"|"\\Phi")
greek	({g1}|{g2}|{g3}|{g4}|{g5}|{g6})
sigvar	"\\sum"{var}"^{"{digit}+"}"

%% 

{greek}			{
					char buf[32];
					/* Skip the first backslash converting it to underscore */
					sprintf(buf, "_%s", &yytext[1]);
					fprintf(yyout,"%s", buf);
				}
 
{bar}			{
					char buf[32], *s;
					
					/* Skip "\bar{" part */
					s = yytext+strlen("\\bar{");
					
					/* Then copy the remaining without the last "}" */
					memcpy(buf, s, strlen(s)-1);
					
					/* ... and terminate */
					buf[strlen(s)-1]='\0';
					fprintf(yyout,"%s_bar", buf);
				}

{sigvar}		{
					/* We want to change \sum X^{2} to sigma_X2 */
					char buf[32], *s, *s2;
					int pos;
					
					memcpy(buf, "sigma_", strlen("sigma_"));
					pos = strlen("sigma_");
					
					/* Skip "\\sum" */
					s = yytext+strlen("\\sum");
					s2 = strchr(s, '^');
					memcpy(&buf[pos], s, s2-s);
					s = &buf[pos+ s2 - s] ;
					s[0] = '\0';
					strcat(buf,s2+2);
					buf[strlen(buf)-1] = '\0';
					
					fprintf(yyout,"%s",buf);
	
				}
				
{sub}|{sup}		{
					char *s = yytext;
					while(*s)
					{
						
						if(*s =='^')
							fprintf(yyout,"_");
						else if(*s != '{' && *s != '}')
							fprintf(yyout,"%c",*s);
						s++;
					}
				}
				
"\'\'\'\'\'"	fprintf(yyout,"pm5");
"\'\'\'\'"		fprintf(yyout,"pm4");
"\'\'\'"		fprintf(yyout,"pm3");
"\'\'"			fprintf(yyout,"pm2");
"\'"			fprintf(yyout,"pm");

"*"				fprintf(yyout,"star");
","				fprintf(yyout,"_");
[\n]			{
					ECHO;
					lineno++;
				}
.				ECHO;

%% 
int yywrap(void)
{ 
    return 1; 
}

int preproc(FILE *out, FILE *in)
{
	ppin = in;
	ppout = out;
	return pplex();
}
