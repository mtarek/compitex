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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "parser.h"
#include "y.tab.h"
#include "util.h"
#include "list.h"
#include "parameter.h"
#include "compitex.h"

#define MAXEXP  4096
#define MAXPAR  20

static FILE *_out, *_in;
int INDENT;

typedef struct sumNode {
    char    idx[2];
    char    svar[8];
    char    lo[12], hi[12];
    char    expr[MAXEXP];
} sumNode;

char* _ex2 (char * dst, nodeType *p) {
    int nops;
    switch(p->type) {
    case typeOpr:
        switch(p->opr.oper) {
        case SUM:
            strfcat(dst,"%s", p->opr.op[0]->id.s);
            break;
        
        case '=':
            nops = p->opr.nops;
            
            /* check leftmost node */
            _ex2(dst, p->opr.op[0]);

            strfcat(dst, "%c", p->opr.oper);
            
            /* check rightmost node */
            if(nops > 1)
                _ex2(dst, p->opr.op[nops-1]);
            break;
        
        case UMINUS:
            if(p->opr.op[0]->type != typeCon) {
                strfcat(dst, "(-1*(");
                _ex2(dst,p->opr.op[0]);
                strfcat(dst, "))");
            } else {
                strfcat(dst, "(-");
                _ex2(dst,p->opr.op[0]);
                strfcat(dst, ")");
            }
            break;
                        
        case '^':
            strfcat(dst,"(pow(");
            nops = p->opr.nops;
            
            /* check leftmost node */
            _ex2(dst, p->opr.op[0]);

            strfcat(dst,",");
            
            /* check rightmost node */
            if(nops > 1)
                _ex2(dst, p->opr.op[nops-1]);
            
            strfcat(dst,"))");
            break;
            
        default: //binary operators
            strfcat(dst,"(");
            nops = p->opr.nops;
            
            /* check leftmost node */
            _ex2(dst, p->opr.op[0]);

            strfcat(dst,"%c", p->opr.oper);
            
            /* check rightmost node */
            if(nops > 1)
                _ex2(dst, p->opr.op[nops-1]);
            
            strfcat(dst,")");
        }
        break;
        
    case typeId:
        strfcat(dst,"%s", p->id.s);
        return dst;
        break;
        
    case typeCon:
        
        strfcat(dst,"%g", p->con.value);
        return dst;
        break;
        
            
    }
 
    return dst;   
}

int parse_sum_node(sumNode * s, nodeType * p, int parse_exp) 
{
    if(!p)
        return -1;
    
    if(p->type != typeOpr)
        return -2;
        
    if(p->opr.oper != SUM)
        return -3;
    
    snprintf(s->idx, 2, "%s", p->opr.op[0]->id.s);
    
    if(p->opr.op[1]->type == typeCon)
        snprintf(s->lo, 12, "%g", p->opr.op[1]->con.value);
    else if(p->opr.op[1]->type == typeId)
        snprintf(s->lo, 12, "%s", p->opr.op[1]->id.s);
    else
        return -4;
    
    
    _ex2(s->hi, p->opr.op[2]);
     
    if(parse_exp) 
        _ex2(s->expr, p->opr.op[3]);   
    
    return 0;    
}

char* _sumpass(char *dst, nodeType *p, int topcall) {
    static int sumid=0;
    sumNode *s;
    int ret;
    
    if(topcall) {
        sumid = 0;
        topcall = 0;
    }
    
    switch(p->type) {
    case typeOpr:
        if(p->opr.oper == SUM) {
            s = calloc(1, sizeof(sumNode));
            ret = parse_sum_node(s, p, 0);
            if(ret < 0) {
                printf("ERR(%d)\n", ret);
                return -1;
            }
            
            sprintf(s->svar, "sum%d", sumid++);
            strfcati(dst, "double %s=0;\n", s->svar);
            strfcati(dst, 
                   "for(%s = %s; %s <= %s ; %s++) {\n",
                   s->idx, s->lo, s->idx, s->hi, s->idx
                   ); 
                   
            INDENT++;
                   

        }
        
        _sumpass(dst, p->opr.op[p->opr.nops-1], 0);
        
        if(p->opr.oper == SUM) {

            ret = parse_sum_node(s, p, 1);
            if(ret < 0) {
                printf("ERR(%d)\n", ret);
                return -1;
            }
            

            strfcati(dst, 
                   "    %s += %s;\n",
                   s->svar, s->expr
                   );
  
            strcpy(p->opr.op[0]->id.s, s->svar);

            free(s);
        }
        _sumpass(dst, p->opr.op[0], 0);        
        if(p->opr.oper == SUM) {
            INDENT--;
            strfcati(dst, "}\n");
        }
        break;
        
    default:
        break;
    }
    
    
}

void _parampass(List *dst, nodeType *p)
{
	Parameter *par;
    switch(p->type) {
    case typeOpr:
		
		if(p->opr.oper == SUM) {
			par = Parameter_build(DTYPE_INT, p->opr.op[0]->id.s);
			List_addonce(dst, par);
		}
		
		
		_parampass(dst, p->opr.op[0]);
		
        if(p->opr.oper == SUM) {
			_parampass(dst, p->opr.op[1]);
			_parampass(dst, p->opr.op[2]);
		}
		
		_parampass(dst, p->opr.op[p->opr.nops -1]);
        break;
        
    case typeId:
		par = Parameter_build(DTYPE_DOUBLE, p->id.s);
		List_addonce(dst, par);
        break;
        
    default:
        break;
    }
}

int ex(nodeType *p) {
    char mainbuf[MAXEXP];
    int i = 0;
    memset(mainbuf, 0, MAXEXP);
	
	List ple;
	List *parlist = &ple;
	List_init(parlist, Parameter_eq);
	
    _parampass(parlist, p);
	
	/* First parameter is always the lvalue, always print this one */
	Node *it = parlist->head;
	Parameter *par = (Parameter *)it->data;
	fprintf(_out, "%s", par->identifier);
	for(i = 1; i < parlist->size; i++) {
		it = it->next;
		par = (Parameter *)it->data;
		if(par->type == DTYPE_DOUBLE)
			fprintf(_out, ", %s", par->identifier);
	}
	fprintf(_out, "\n");
	
	it = parlist->head;
	int printedone = 0;
	for(i = 1; i < parlist->size; i++) {
		it = it->next;
		par = (Parameter *)it->data;			
		if(par->type == DTYPE_INT) {
			if(printedone)
				fprintf(_out, ", ");
            else
                fprintf(_out, "int ");
                
			fprintf(_out, "%s", par->identifier);
			printedone = 1;
		}
		
	}
    if(printedone)
        fprintf(_out, ";\n");
	
	/*fprintf(_out, "Par#    Type    ID\n");
	for(i = 0;i < parlist->size; i++) {
		Parameter *par = (Parameter *)it->data;
		fprintf(_out, "%4d    %4d    %s\n", i, par->type, par->identifier);
		it = it->next;
    }*/
	
	
	
    _sumpass(mainbuf, p, 1);
    fprintf(_out, "%s\n", mainbuf);

    memset(mainbuf, 0, MAXEXP);
	
    _ex2(mainbuf, p);
    fprintf(_out, "%s;\n\n", mainbuf);
    return 0;
} 

int codegen(FILE *out, FILE *in)
{
    _out = out;
    _in = in;
    parse(_out, _in);
    return 0;
}
