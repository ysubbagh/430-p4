#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

//Size of memory, 80 bytes
#define MEMSIZE 80

// return true if C-strings are equal
bool equal(char *a, char *b);

// read a line from console
// return length of line read or -1 if failed to read
// removes the \n on the line read
int fetchline(char **line);

//helper function for R command, accept and process input from a file
bool acceptFile(char* inFile);

//process the line entered
bool processLine(char* line);

//let users enter another command line
int interactiveShell();

//main function, starting point
int main();