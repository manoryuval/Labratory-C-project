#ifndef SECOND_PASS_H
#define SECOND_PASS_H

/*
Second pass of the assembler - processes the .am file and generates the .ob, .ent, and .ext files.
@param file_name The name of the input file without extension.
@return 0 on success, 1 on failure.
*/
int second_pass (char *file_name);


#endif