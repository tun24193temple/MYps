#ifndef UTILS_H
#define UTILS_H

typedef struct{
    int pid;
    int state;
    int usr_time;
    int sys_time;
    int vir_mem;
    int cmd_line;
}Options;

void parse_options(int argcount, char **input, Options *options);

void parse_process_info(int p, Options *options);

void parse_proc_dir(Options *options);

#endif