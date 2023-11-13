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

//allocate memory for a given process, go to picked algorithim
void allocate(char* args);

//allocate based on first fit
void firstFit(char name, int size);

//allocate based on fibestrst fit
void bestFit(char name, int size);

//allocate based on worst fit
void worstFit(char name, int size);

//free all allocations owned by a process
void freeMem(char *args);

//show the state of the memory pool
void poolState();

//compact the memory pool 
void compact();