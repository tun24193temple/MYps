#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <ctype.h>
#include "utils.h"


//p is the pid we use to identify process and *options is a struct containing the display flags 
void parse_process_info(int p, Options *options){
    char path[1024];//create a buffer to store the path
    snprintf(path, sizeof(path), "/proc/%d/", p); //path: /proc/<pid>/

    //check if process exists
    DIR *proc_dir = opendir(path);
    if(proc_dir == NULL){
        perror("Process does not exist");
        return;
    }
    closedir(proc_dir);

    printf("PID: %d\n", p);

    /* %c
    One of the following characters, indicating process state:
    R  Running
    S  Sleeping in an interruptible wait
    D  Waiting in uninterruptible disk sleep
    Z  Zombie
    T  Stopped (on a signal) or (before Linux 2.6.33) trace stopped
    t  Tracing stop (Linux 2.6.33 onward)
    W  Paging (only before Linux 2.6.0)
    X  Dead (from Linux 2.6.0 onward)
    x  Dead (Linux 2.6.33 to 3.13 only)
    K  Wakekill (Linux 2.6.33 to 3.13 only)
    W  Waking (Linux 2.6.33 to 3.13 only)
    P  Parked (Linux 3.9 to 3.13 only)*/
    if(options -> state){
        char stat[1030];//store path to stat dir
        snprintf(stat, sizeof(stat),"%sstat", path); //add stat to /proc/<pid>/

        FILE *file = fopen(stat, "r"); 
        if(file){
            char parsed_state; //create a char variable to store the value for the Process State
            fscanf(file, "%*d %*s %c", &parsed_state); //skip two process properties until you get to the 3rd value which should be a %c
            fclose(file);
            printf("\tState: %c\n", parsed_state); //output
        }else{
            perror("Error opening stat file");
            exit(1);  
        }
    }

    /* %lu
    Amount of time that this process has been  scheduled  in  user  mode,
    measured  in  clock ticks (divide by sysconf(_SC_CLK_TCK)).  This in‐
    cludes guest time, guest_time (time spent running a virtual CPU,  see
    below),  so  that  applications  that are not aware of the guest time
    field do not lose that time from their calculations. */
    if(options -> usr_time){
        char stat[1030];
        snprintf(stat, sizeof(stat), "%sstat", path);

        FILE *file = fopen(stat, "r");

        if(file){
            unsigned long parsed_utime;
            //skip 13 process properties until you get to utime
            fscanf(file, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu", &parsed_utime);
            fclose(file);
            printf("\tUser Time: %lu\n", parsed_utime);
        }else{
            perror("Error opening stat file");
            exit(1);
        }


    }

    /* %lu
        Amount of time that this process has been scheduled in  kernel  mode,
        measured in clock ticks (divide by sysconf(_SC_CLK_TCK)).*/
    if(options -> sys_time){
        char stat[1030];
        snprintf(stat, sizeof(stat), "%sstat", path);

        FILE *file = fopen(stat, "r");
        if(file){
            unsigned long parsed_stime;
            //skip 14
            fscanf(file, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %*u %lu", &parsed_stime);
            fclose(file);
            printf("\tSystem Time: %lu\n", parsed_stime);
        }else{
            perror("Error opening stat file");
        }
    }

    if(options -> vir_mem){
        char statm[1030];
        snprintf(statm, sizeof(statm), "%sstatm", path);

        FILE *file = fopen(statm, "r");
        if(file){
            int VmSize;
            //first one is VmSize
            fscanf(file, "%d", &VmSize);
            fclose(file);
            printf("\tVmSize: %d\n", VmSize);
        }else{
            perror("Error opening statm file");
        }
    }

    if(options -> cmd_line){
        char cmdline[2000];
        snprintf(cmdline, sizeof(cmdline), "%scmdline", path);//store path /proc/<pid>/cmdline to buffer

        FILE *file = fopen(cmdline, "r");
        if(file){
            char cmd[1030]; //store command line content
            fread(cmd, 1, sizeof(cmd), file); //read content from cmdline to cmd 
            fclose(file);
            
            if(cmd[0] != '\0'){ //if file not empty, print cmd
                printf("\t[%s]\n", cmd);  
            }
        }else{
            printf("Error opening cmdline file");
        }

    }

    /*
        size (1) total program size
    */

    }

void parse_proc_dir(Options *options){
    DIR *dir; //pointer to dir
    struct dirent *current_process; //ptr to struct dirent representing current file within /proc
    dir = opendir("/proc");

    if(dir == NULL){
        perror("Error opening /proc directory");
        exit(1);
    }

    //loop through all entries in /proc
    while((current_process = readdir(dir)) != NULL){
        char *current_pid = current_process -> d_name; //access name of current dir and set a char pointer to that address

        //if entry name begins with a digit, it's a process with a pid
        if(isdigit(current_pid[0])){
            int pid = atoi(current_pid); //entry name is the pid of process
            parse_process_info(pid, options); //use pid to parse through process for that pid
        }
    }

    closedir(dir);

}   