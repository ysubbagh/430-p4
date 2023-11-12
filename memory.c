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

//process the line entered
bool processLine(char* line){
    if(equal(line, "E")){return false;} //exit command
    
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