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

compitex: parser.tab.o scanner.o preproc.o list.o codegen.o util.o compitex.o

clean:
	rm -f *~ *.o compitex preproc.c scanner.c parser.tab.c y.tab.h
