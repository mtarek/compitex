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
