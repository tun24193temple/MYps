//JOB:
//Struct vars: pid, status, user time, system time, virtual memory, command line
//parse command line options
//scan through /proc/<pid>/stat to find status, utime, and stime
//scan through /proc/<pid>/statm to find VmSize
//scan through /proc/<pid>/cmdline and get cmdline
//scan through /proc to find everything on every process

//one function: parse /proc/<pid>/
//second function: for pid < processes (parse(/proc/pid))

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include "utils.h"

int main(int argc, char *argv[]){
    //struct for holding flag information as well as value of PID
    Options options;
    parse_options(argc, argv, &options);
    
    //if pid is specified, parse process for info
    if(options.pid != -1){     
        parse_process_info(options.pid, &options);  
    
    //if not display information for all processes
    }else{
        parse_proc_dir(&options);
    }

    return 0;
}
