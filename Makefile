assembler: preproc.o utils.o assembler.o
	gcc -g -ansi -Wall -pedantic preproc.o utils.o assembler.o -lm -o assembler

preproc.o: preproc.c preproc.h utils.h
	gcc -c -g -ansi -Wall preproc.c -o preproc.o

utils.o: utils.c utils.h
	gcc -c -g -ansi -Wall utils.c -o utils.o

assembler.o: assembler.c preproc.h utils.h
	gcc -c -g -ansi -Wall assembler.c -o assembler.o