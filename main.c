#include <stdio.h>
#include "compitex.h"

#define LOG(...)

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
