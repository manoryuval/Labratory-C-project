/*
Changes the extension of a file name.
@param file_name The original file name.
@param new_extension The new extension to append.
@return Pointer to the new file name with the new extension
 */
char *create_extension(char *filename, char *extension);
/*
Trims leading and trailing whitespace from a line.
@param line The line to be trimmed.
 */
void trim(char *line);
