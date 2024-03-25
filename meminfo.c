/*
1. get header files like the stds and string and dirent
2. maybe make a struct to hold mem info
3. make a parsing function that parses through the /proc/mem file when opened
4. main function that calls parsing function
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parse_mem_info(FILE *file){
    //set output vars
    int free_mem = 0;
    int mem_buffers = 0;

    //read lines from the file and parse CPU information //var for keeping track of current char fgetc() is on
    int currentchar;
    char line[256]; //buffer for identifying "MemFree" and "Buffers"
    int index = 0;

    while ((currentchar = fgetc(file)) != EOF) {
        if(currentchar == '\n'){
            line[index] = '\0'; //delete line
            index = 0; //reset index for next line

            if(strstr(line, "MemFree:")){
                sscanf(line, "MemFree: %d kB", &free_mem);
            }

            if(strstr(line, "Buffers:")){
                sscanf(line, "Buffers: %d kB", &mem_buffers);
            }
        
        }else{
            line[index++] = (char)currentchar;
        }
    }

    printf("Free Memory: /%d\n", free_mem);
    printf("Number of Buffers: %d\n", mem_buffers);
    
}

int main(){
    FILE *file = fopen("/proc/meminfo", "r");
    if (file == NULL) {
        perror("Error opening /proc/meminfo");
        exit(1);
    }

    parse_mem_info(file);

    fclose(file);

    return 0;
}

