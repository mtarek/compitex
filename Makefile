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
CFLAGS= -g

.PHONY: all clean

all: compitex

preproc.c: preproc.l
# preproc's prefix string is changed so that it does not conflict with scanner's
	flex --prefix=pp -o $@ $<

scanner.c: scanner.l
	flex -o $@ $<

parser.tab.c: parser.y
	bison --defines=y.tab.h	$<

compitex: parser.tab.o scanner.o preproc.o list.o codegen.o util.o parameter.o compitex.o

clean:
	rm -f *~ *.o compitex preproc.c scanner.c parser.tab.c y.tab.h
