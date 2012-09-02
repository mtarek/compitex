#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "parser.h"
#include "y.tab.h"
#include "compitex.h"

#define MAXEXP  4096

static FILE *_out, *_in;
typedef struct sumNode {
    char    idx[2];
    char    svar[8];
    char    lo[12], hi[12];
    char    expr[MAXEXP];
} sumNode;

void inline strfcat(char * dst, const char * fmt, ...) 
{
    char tmp[2048];
    va_list args;
    
    va_start(args, fmt);
    vsprintf(tmp, fmt, args);
    va_end(args);

    strcat(dst, tmp);
}

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

int parse_sum_node(sumNode * s, nodeType * p) 
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
        _sumpass(dst, p->opr.op[p->opr.nops-1], 0);
        
        if(p->opr.oper == SUM) {
            s = calloc(1, sizeof(sumNode));
            
            ret = parse_sum_node(s, p);
            if(ret < 0) {
                printf("ERR(%d)\n", ret);
                return -1;
            }
            
            sprintf(s->svar, "sum%d", sumid++);
            strfcat(dst, "double %s=0;\n", s->svar);
            strfcat(dst, 
                   "for(%s = %s; %s <= %s ; %s++)\n"
                   "    %s += %s;\n",
                   s->idx, s->lo, s->idx, s->hi, s->idx,
                   s->svar, s->expr
                   );
  
            strcpy(p->opr.op[0]->id.s, s->svar);

            free(s);
        }
        
        _sumpass(dst, p->opr.op[0], 0);
        break;
        
    default:
        break;
    }
    
    
}

char * _parampass(char *dst, nodeType *p)
{
    switch(p->type) {
    case typeOpr:
        _parampass(dst, p->opr.op[p->opr.nops-1]);
        
        if(p->opr.oper != SUM)
            _parampass(dst, p->opr.op[0]);
        break;
        
    case typeId:
        strfcat(dst, "%s, ", p->id.s);
        break;
        
    default:
        break;
    }
    
    return dst;
}

int ex(nodeType *p) {
    char params[MAXEXP];
    char mainbuf[MAXEXP];
    memset(mainbuf, 0, MAXEXP);
    memset(params, 0, MAXEXP);
    _parampass(params, p);
    fprintf(_out, "params:%s\n", params);
    _sumpass(mainbuf, p, 1);
    fprintf(_out, "%s\n", mainbuf);

    _ex2(mainbuf, p);
    fprintf(_out, "%s\n\n", mainbuf);
    return 0;
} 

int codegen(FILE *out, FILE *in)
{
    _out = out;
    _in = in;
    parse(_out, _in);
    return 0;
}
