/*
1. Get header files: std, string for parsing, dirent to read file
2. Make a function for parsing the /proc/cpuinfo filesystem looking for keyword processor and cache_size
3. Main calls function.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>


void parse_cpu_info(FILE *file) {
    //set output vars
    int processors_count = 0;
    int cache_size;

    //read lines from the file and parse CPU information
    int currentchar; //var for keeping track of current char fgetc() is on
    char line[256]; //buffer for recording the line
    int index = 0; //index used to add current char to line buffer
    while ((currentchar = fgetc(file)) != EOF) {
        if(currentchar == '\n'){
            line[index] = '\0'; //delete line
            index = 0; //reset index for next line

            if (strstr(line, "processor")) {
                if(processors_count > 0){
                    printf("Processor %d with a cache size of %d KB\n", processors_count, cache_size);
                }
                 //see if processor is a substring of the line buffer
                processors_count++;
                cache_size = 0;
            }
            if (strstr(line, "cache size")) { //see if cache size is a substring of the line buffer
                sscanf(line, "cache size\t: %d", &cache_size); //push cache_size to defined var
            }
        }else{
            line[index++] = (char)currentchar; //add currentchar to the index and iterate index
        }
    }
    
    printf("Number of Processors: %d\n", processors_count);
    
    
}

int main() {
    //open the /proc/cpuinfo file for reading
    FILE *file = fopen("/proc/cpuinfo", "r");
    if (file == NULL) {
        perror("Error opening /proc/cpuinfo");
        exit(1);
    }

    parse_cpu_info(file);

    fclose(file);

    return 0;
}