#include <string.h>
#include <stdarg.h>
#include "util.h"

#define TAB_INDENT "    "

void inline strfcat(char * dst, const char * fmt, ...) 
{
    char tmp[2048];
    va_list args;
    
    va_start(args, fmt);
    vsprintf(tmp, fmt, args);
    va_end(args);

    strcat(dst, tmp);
}

void inline strfcati(char * dst, const char * fmt, ...) 
{
    char tmp[2048];
    char *line;
    
    va_list args;
    
    va_start(args, fmt);
    vsprintf(tmp, fmt, args);
    va_end(args);
    
    line = strtok(tmp, "\n");
    while(line) {
        
        int i = 0;
        while(i++ < INDENT) 
            strcat(dst, TAB_INDENT);
        
        strfcat(dst,"%s\n", line);
        
        line = strtok(NULL, "\n");
    }
    
}
