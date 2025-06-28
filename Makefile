CC = gcc
CFLAGS = -g -ansi -Wall -pedantic

SRC_DIR = c_files
INC_DIR = header_files

OBJ = preproc.o utils.o assembler.o first_pass.o labels.o
EXEC = assembler

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -lm -o $(EXEC)

preproc.o: $(SRC_DIR)/preproc.c $(INC_DIR)/preproc.h $(INC_DIR)/utils.h
	$(CC) -c $(CFLAGS) $(SRC_DIR)/preproc.c -o preproc.o

utils.o: $(SRC_DIR)/utils.c $(INC_DIR)/utils.h
	$(CC) -c $(CFLAGS) $(SRC_DIR)/utils.c -o utils.o

assembler.o: $(SRC_DIR)/assembler.c $(INC_DIR)/preproc.h $(INC_DIR)/utils.h
	$(CC) -c $(CFLAGS) $(SRC_DIR)/assembler.c -o assembler.o

first_pass.o: $(SRC_DIR)/first_pass.c $(INC_DIR)/first_pass.h $(INC_DIR)/utils.h
	$(CC) -c $(CFLAGS) $(SRC_DIR)/first_pass.c -o first_pass.o

labels.o: $(SRC_DIR)/labels.c $(INC_DIR)/labels.h $(INC_DIR)/utils.h
	$(CC) -c $(CFLAGS) $(SRC_DIR)/labels.c -o labels.o


#על כל קובץ חדש יש להוסיף  לOBJ

#וליצור קובץ .o
clean:
	rm -f *.o $(EXEC)
