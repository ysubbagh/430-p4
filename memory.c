#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

//Size of memory, 80 bytes
#define MEMSIZE 80

// return true if C-strings are equal
bool equal(char *a, char *b) { return (strcmp(a, b) == 0); }

// read a line from console
// return length of line read or -1 if failed to read
// removes the \n on the line read
int fetchline(char **line) {
  size_t len = 0;
  size_t n = getline(line, &len, stdin);
  if (n > 0) {
    (*line)[n - 1] = '\0';
  }
  return n;
}

//helper function for R command, accept and process input from a file
bool acceptFile(char* inFile){
    FILE *in = fopen(inFile, "r");
    if(in == NULL){
        perror("Error: file could not be opened.");
        return false;
    }
    //setup for parsing
    char *line;

    while(line = fgetc(in)){
        if(line == EOF){ //reached the end of the file

        }
    }


    //finish reading from file
    fclose(in);
    return true;
}

//process the line entered
bool processLine(char* line){
    //tokenize line
    char *token;
    char *args[5]; //max size is 4 for A command, extra space for NULL
    int argsize = 0;
    //get args from user command
    token = strtok(line, " ");
    while(token != NULL){
        if(argsize >= 4){return false;} //invalid access / length 
        args[argsize++] = token;
    }
    args[argsize] = NULL;

    //check command value
    if(equal(args[0], "E")){//exit command
        return false;
    }else if(equal(args[0], "S")){ //memory pool state
        //stuff
    }else if(equal(args[0], "C")){ //compact the memory pool
        //stuff
    }else if(equal(args[0], "F")){ //free allocations to certain process
        //stuff
    }else if(equal(args[0], "A")){ //allocate to certain process
        //stuff
    }else if(equal(args[0], "R")){ //read in from file
        return acceptFile(args[1]);
    }else{ //error / invalid entry
        perror("Error: Invalid command.");
        return false;
    }
}

int interactiveShell(){
     //setup vars for loop
    bool should_run = true;
    char *line = calloc(1, 80); //max size per command

    //looop to get more commands from script
    while (should_run) {
        int n = fetchline(&line);
        if(n == -1) { //error reading line
            should_run = false;
            continue;
        }

        //check vals
        should_run = processLine(line);
    }

}



//main function, starting point
int main(){
    return interactiveShell();
}