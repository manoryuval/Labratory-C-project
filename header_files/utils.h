/*
Changes the extension of a file name.
@param file_name The original file name.
@param new_extension The new extension to append.
@return Pointer to the new file name with the new extension
 */
char *create_extension(char *filename, char *extension);
/*
Trims leading and trailing whitespace from a line.
@param line The line to betrimmed.
 */
void trim(char *line);
/*
Counts the number of alphabetic characters in a string.
@param token The string to count characters in.
@return The number of alphabetic characters in the string.
*/
int alpha_count(char *token);
/*
Removes all whitespace characters from a string.
@param str The string to remove whitespace from.
*/
void remove_spaces(char *str);
/*
Converts a character to lowercase.
@param c The character to convert.
@return The lowercase version of the character.
*/
char lowercase(char c);