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
#include <stdio.h>
#include "compitex.h"
#include "util.h"

#define LOG printf

#define MAXBUF  4096

int main(int argc, char **argv)
{
    FILE *fin, *fout, *ftmp;
    char eqbuf[MAXBUF];
        
    fin = stdin;
    fout = stdout;

    if(argc > 1) {
        fin = fopen(argv[1], "r");
        if(!fin) {
            LOG("Could not open file %s for reading. Exiting ...\n", argv[1]);
            return -1;
        }
            
    } 
    
    if(argc > 2) {
        fout = fopen(argv[2], "w");
        if(!fout) {
            LOG("Could not open file %s for writing. Exiting ...\n", argv[2]);
            return -1;
        }
    }
    
    ftmp = tmpfile();
    
    preproc(ftmp, fin);
    
    fseek(ftmp, SEEK_SET, 0);

    codegen(fout, ftmp);
    
    fclose(fin);
    fclose(fout);
    
}
