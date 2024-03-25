#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include "utils.h"

void parse_options(int argcount, char **input, Options *options){
    //initialize display variables
    options -> pid = -1;
    options -> state = 0;
    options -> usr_time = 1;
    options -> sys_time = 0;
    options -> vir_mem = 0;
    options -> cmd_line = 1;
    
    int opt;

    //opt: used to store the result of getopt. Holds option charcter parsed from cmdline
    //"p:sUSvc": Alloed options. p: requires and argument, rest do not.
    while((opt = getopt(argcount, input, "p:sUSvc")) != -1){
        switch(opt){
            case 'p':
                options -> pid = atoi(optarg); //e.g '-p 1234' optarg point to 1234 string. convert it to int and store it in pid var
                break;
            case 's':
                options -> state = 1;//display if called
                break;
            case 'U':
                options -> usr_time = 0;//dont display if called
                break;  
            case 'S':
                options -> sys_time = 1;//display if called
                break;
            case 'v':
                options -> vir_mem = 1;//display if called
                break;
            case 'c':
                options -> cmd_line = 0;//dont display if called
                break;
            default:
                printf("Error: '%c' is an invalid option\n", optopt);
                exit(1);
        }
    }
}