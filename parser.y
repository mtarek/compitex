%{ 
#include <stdio.h> 
#include <stdlib.h> 
#include <stdarg.h> 
#include <string.h>
#include "parser.h" 

/* prototypes */ 
nodeType *opr(int oper, int nops, ...); 
nodeType *id(char* s); 
nodeType *con(double value); 
void freeNode(nodeType *p); 
int ex(nodeType *p); 
int yylex(void);
int scan_init(FILE *out, FILE *in);
int parse(FILE *out, FILE *in);


void yyerror(char *s); 
int sym[26];                             /* symbol table */ 
%} 

%union { 
	double dValue;                      /* double value */ 
    char* str;                          /* string value */ 
	nodeType *nPtr;                     /* node pointer */ 
}; 
%token <dValue> REAL

%token <str> VARIABLE 
%token WHILE IF PRINT FRAC MUL SUM SQRT BAR
%nonassoc IFX 
%nonassoc ELSE 

%left GE LE EQ NE '>' '<' 
%left '+' '-' 
%left MUL '/' 
%right '^'
%nonassoc UMINUS 

%type <nPtr> stmt expr stmt_list ident

%% 

program: 
   function                        { return(0); } 
   ; 

function: 
    function stmt                  { ex($2); freeNode($2);} 
  | /* NULL */ 
   ; 

stmt: 
    ';'                            { $$ = opr(';', 2, NULL, NULL); } 
  | expr ';'                       { $$ = $1; } 
  | PRINT expr ';'                 { $$ = opr(PRINT, 1, $2); } 
  | expr '=' expr ';'              { $$ = opr('=', 2, $1, $3); }
  | WHILE '(' expr ')' stmt { $$ = opr(WHILE, 2, $3, $5); } 
  | IF '(' expr ')' stmt %prec IFX { $$ = opr(IF, 2, $3, $5); } 
  | IF '(' expr ')' stmt ELSE stmt 
                                   { $$ = opr(IF, 3, $3, $5, $7); } 
  | '{' stmt_list '}'              { $$ = $2; } 
     ; 

stmt_list: 
    stmt                       { $$ = $1; } 
  | stmt_list stmt              { $$ = opr(';', 2, $1, $2); } 
  ; 

ident:
	REAL                        { $$ = con($1); }
  | VARIABLE                    { $$ = id($1); } 
  ;

expr:
    ident
  | BAR '{' VARIABLE '}'		{
									char tmp[32];
									sprintf(tmp, "%s_bar", $3);
									$$ = id(tmp);
								}
  | '-' expr %prec UMINUS 		{ $$ = opr(UMINUS, 1, $2); } 
  | expr '+' expr               { $$ = opr('+', 2, $1, $3); }
  | expr '-' expr               { $$ = opr('-', 2, $1, $3); } 
  | expr MUL expr               { $$ = opr('*', 2, $1, $3); }
  | expr '/' expr               { $$ = opr('/', 2, $1, $3); }
  | FRAC '{'expr'}' '{'expr'}'  { $$ = opr('/', 2, $3, $6); }
  | SUM '{'VARIABLE '=' expr '}' '^' '{'expr'}''{'expr'}'     
								{ $$ = opr(SUM, 4, id($3), $5, $9, $12);}
  | SQRT '{' expr '}'			{ $$ = opr('^', 2, $3,con(0.5)); }
  | SQRT '[' REAL ']''{' expr '}'			
								{ $$ = opr('^', 2, $6, con(1/$3)); }
  | expr '^' '{' expr '}'       { $$ = opr('^', 2, $1, $4); }
  | expr '<' expr               { $$ = opr('<', 2, $1, $3); } 
  | expr '>' expr               { $$ = opr('>', 2, $1, $3); } 
  | expr GE expr                { $$ = opr(GE, 2, $1, $3); } 
  | expr LE expr                { $$ = opr(LE, 2, $1, $3); } 
  | expr NE expr                { $$ = opr(NE, 2, $1, $3); } 
  | expr EQ expr                { $$ = opr(EQ, 2, $1, $3); } 
  | '(' expr ')'                { $$ = $2; } 
  | ident ident					{ $$ = opr('*', 2, $1, $2); }
  | ident '(' expr ')'			{ $$ = opr('*', 2, $1, $3); }
  | '(' expr ')' ident			{ $$ = opr('*', 2, $2, $4); }
  | '(' expr ')' '(' expr ')'	{ $$ = opr('*', 2, $2, $5); }
  | error						{ printf("%d: Error at (%c)\n", lineno, yychar);}	
  ; 

%% 

#define SIZEOF_NODETYPE ((char *)&p->con - (char *)p) 

nodeType *con(double value) { 
     nodeType *p; 
     size_t nodeSize; 

     /* allocate node */ 
     nodeSize = SIZEOF_NODETYPE + sizeof(conNodeType); 
     if ((p = (nodeType*)malloc(nodeSize)) == NULL) 
          yyerror("out of memory"); 

     /* copy information */ 
     p->type = typeCon; 
     p->con.value = value; 

     return p; 
} 


nodeType *id(char* s) { 
     nodeType *p; 
     size_t nodeSize; 

     /* allocate node */ 
     nodeSize = SIZEOF_NODETYPE + sizeof(idNodeType); 
      if ((p = (nodeType*)malloc(nodeSize)) == NULL) 
            yyerror("out of memory"); 

      /* copy information */ 
      p->type = typeId; 
      p->id.s = strdup(s); 

      return p; 
} 

nodeType *opr(int oper, int nops, ...) { 
      va_list ap; 
      nodeType *p; 
      size_t nodeSize; 
      int i; 

      /* allocate node */ 
      nodeSize = SIZEOF_NODETYPE + sizeof(oprNodeType) + 
            (nops - 1) * sizeof(nodeType*); 
      if ((p = (nodeType*)malloc(nodeSize)) == NULL) 
            yyerror("out of memory"); 

      /* copy information */ 
      p->type = typeOpr; 
      p->opr.oper = oper; 
      p->opr.nops = nops; 
      va_start(ap, nops); 
      for (i = 0; i < nops; i++) 
            p->opr.op[i] = va_arg(ap, nodeType*); 
      va_end(ap); 
      return p; 
} 

void freeNode(nodeType *p) { 
      int i; 

      if (!p) return; 
      if (p->type == typeOpr) { 
            for (i = 0; i < p->opr.nops; i++) 
                 freeNode(p->opr.op[i]); 
      } 
      free (p); 
} 

void yyerror(char *s) { 
      fprintf(stdout, "%s\n", s); 
} 

int parse(FILE *out, FILE *in) {
      scan_init(out, in);
      yyparse();
      return 0; 
}
	 
