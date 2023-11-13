#include "memory.h"

bool equal(char *a, char *b) { return (strcmp(a, b) == 0); }

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

    while(fgets(line, sizeof(line), in) != NULL){
        if(processLine(line) == false){ //send line to be proccessed, if exit command then leave
            fclose(in);
            return false;
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
        token = strtok(NULL, " ");
    }
    args[argsize] = NULL;

    //check command value
    if(equal(args[0], "E")){//exit command
        printf("E done\n");
        return false;
    }else if(equal(args[0], "S")){ //memory pool state
        //stuff
        printf("S done\n");
    }else if(equal(args[0], "C")){ //compact the memory pool
        //stuff
        printf("C done\n");
    }else if(equal(args[0], "F")){ //free allocations to certain process
        //stuff
        printf("F done\n");
    }else if(equal(args[0], "A")){ //allocate to certain process
        //stuff
        printf("A done\n");
    }else if(equal(args[0], "R")){ //read in from file
        printf("R done\n");
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
            perror("fetch line broken.\n");
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