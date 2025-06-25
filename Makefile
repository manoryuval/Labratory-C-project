CC = gcc
CFLAGS = -g -ansi -Wall -pedantic

SRC_DIR = c_files
INC_DIR = header_files

OBJ = preproc.o utils.o assembler.o
EXEC = assembler

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -lm -o $(EXEC)

preproc.o: $(SRC_DIR)/preproc.c $(INC_DIR)/preproc.h $(INC_DIR)/utils.h
	$(CC) -c $(CFLAGS) $(SRC_DIR)/preproc.c -o preproc.o

utils.o: $(SRC_DIR)/utils.c $(INC_DIR)/utils.h
	$(CC) -c $(CFLAGS) $(SRC_DIR)/utils.c -o utils.o

assembler.o: $(SRC_DIR)/assembler.c $(INC_DIR)/preproc.h $(INC_DIR)/utils.h
	$(CC) -c $(CFLAGS) $(SRC_DIR)/assembler.c -o assembler.o

clean:
	rm -f *.o $(EXEC)
