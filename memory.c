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
        return false;
    }else if(equal(args[0], "S")){ //memory pool state
        poolState();
    }else if(equal(args[0], "C")){ //compact the memory pool
        compact();
    }else if(equal(args[0], "F")){ //free allocations to certain process
        freeMem(args[1]);
    }else if(equal(args[0], "A")){ //allocate to certain process
        allocate(args);
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
    init();
    return interactiveShell();
}

//allocate mem for a process
void allocate(char* args[]){
    if(args[1] == NULL || strlen(args[1]) > 1){ //inavlid process name
        perror("Error: Process name invalid.\n");
        return;
    }
    //send to allocate process based on algo choice
    int size = atoi(args[2]);
    if(equal(args[3], "F")){ //send to first fit
        firstFit(args[1], size);
    }else if(equal(args[3], "B")){ //send to best fit
        bestFit(args[1], size);
    }else if(equal(args[3], "W")){ //send to worst fit
        worstFit(args[1], size);
    }else{
        perror("Error: Invalid algorithim choice.\n");
    }
}

//allocate based on first fit
void firstFit(char *name, int size){
    int bSize = 0; //size of current block
    int i = 0;
    //find first available free block
    while(true){
        if(i >= MEMSIZE){
            printf("Not enough free memory to allocate: %s\n", name);
            return;
        }else if(equal(pool[i], ".")){ //free block
            bSize++;
            if(bSize == size){
                i = i - bSize;
                break;
            }
            i++;
        }else{ //non empty block
            bSize = 0;
            i++;
        }   
    }
    //add process to memory pool
    for(int j = i; j < size; j++){
        pool[j] = name;
    }
}

//allocate based on best fit
void bestFit(char *name, int size){


}

//allocate based on worst fit
void worstFit(char *name, int size){


}

//free all allocations owned by a process
void freeMem(char *name){
    for(int i = 0; i < MEMSIZE; i++){
        if(equal(pool[i], name)){
            pool[i] = ".";
        }
    }
}

//show the state of the memory pool
void poolState(){
    for(int i = 0; i < MEMSIZE; i++){
        printf("%s", pool[i]);
    }
    printf("\n");
}

//compact the memory pool 
void compact(){

}

//inizalie the array to period to denote mem area
void init(){
    for(int i = 0; i < MEMSIZE; i++){
        pool[i] = ".";
    }
}