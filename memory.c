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
    char line[MEMSIZE];
    char arg[5];
    char *token;

    //read through file
    while(fgets(line, MEMSIZE, in) != NULL){
        printf("line: %s.", line);

        token = strtok(line, " ");
        int i = 0;
        while(token != NULL){
            strcpy(arg[i], token);
            token = strtok(NULL, " ");
            i++;
        }

        //send to be completed
        if(equal(arg[0], "E")){//exit command
            return false;
        }else if(equal(arg[0], "S")){ //memory pool state
            poolState();
        }else if(equal(arg[0], "C")){ //compact the memory pool
            compact();
        }else if(equal(arg[0], "F") || equal(arg[0], "A")){ //free allocations to certain process
            freeMem(arg[3]);
        }else if(equal(arg[0], "A")){ //allocate to certain process
            allocate(arg);
        }else{ //error / invalid entry
            perror("Error: Invalid command.");
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
    free(line);
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
    int size = atoi(args[2]);
    if(size > MEMSIZE){// block is too big for pool
        printf("%s is too big for memory.\n", args[1]);
        return;
    }
    //send to allocate process based on algo choice
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
    int loc = 0;
    int bSize = 0; //size of current block
    //find first available free block
    for(int i = 0; i <= MEMSIZE; i++){
        if(i == MEMSIZE){ //not enough space found
            printf("Not enough free memory to allocate: %s\n", name);
            return;
        }
        if(equal(pool[i], ".")){ //empty block
            bSize++;   
            if(bSize == size){break;}
        }else{
            bSize = 0;
            loc = i + 1;
        }
    }
    //add process to memory pool
    for(int j = loc; j < loc + size; j++){
        pool[j] = name;
    }
}

//allocate based on best fit
void bestFit(char *name, int size){
    bool option = false;
    int currLoc, currBlock, bestLoc, bestBlock = 0;
    //iterate through to find best fit free block
    for(int i = 0; i < MEMSIZE; i++){
        if(equal(pool[i], ".")){
            currBlock++;
            if(currBlock >= size && (!option || currBlock < bestBlock)){ //found a better fitting block
                bestBlock = currBlock;
                bestLoc = currLoc;
                option = true;
            }
        }else{
            currBlock = 0;
            currLoc = i + 1;
        }
    }
    //check if a block was found
    if(bestBlock == 0){
        printf("Not enough free memory to allocate: %s\n", name);
        return;
    }
    //add to memory pool
    for(int j = bestLoc; j < size + bestLoc; j++){
        pool[j] = name;
    }
}

//allocate based on worst fit
void worstFit(char *name, int size){
    int currLoc, currBlock, bigLoc, bigBlock = 0;
    //iterate through the pool to find the biggest block
    for(int i = 0; i < MEMSIZE; i++){
        if(equal(pool[i], ".")){ //free block
            if(currBlock > bigBlock){ //found a bigger block
                if(currLoc != bigLoc) {//bigger block is at a new loaction
                    bigLoc = currLoc;
                }
                bigBlock = currBlock;
            }
            currBlock++;
        }else{ //non-free block 
            currLoc = i + 1;
            currBlock = 0;
        }
    }
    //check if block is big enough
    if(bigBlock < size){
        printf("Not enough free memory to allocate: %s\n", name);
        return;
    }
    //add to mem pool
    for(int i = bigLoc; i < size + bigLoc; i++){
        pool[i] = name;
    }
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
    int lowFree, highFree, lowUsed, highUsed = -1;
    for(int i = 0; i < MEMSIZE; i ++){
        if(equal(pool[i], ".")){ //in a free block
            highFree = i;
        }else{

        }
    }
}

//inizalie the array to period to denote mem area
void init(){
    for(int i = 0; i < MEMSIZE; i++){
        pool[i] = ".";
    }
}