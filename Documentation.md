<p align="center">
  <a href="" rel="noopener">
 <img width=200px height=200px src="https://i.imgur.com/6wj0hh6.jpg" alt="Project logo"></a>
</p>

<h3 align="center">MYps</h3>

---

<p align="center"> Few lines describing your project.
    <br> 
</p>

## 📝 Table of Contents
- [About](#about)
- [Getting Started](#getting_started)
- [Implementation](#implementation)
- [Testing](#testing)
- [Usage](#usage)

## 🧐 About <a name = "about"></a>
The purpose of this project was to learn about the /proc filesystem and how we can use it to access and display information about our system. The primary Assignment was to write a simple version of the Unix ps command and implement 6 options.

## 🏁 Getting Started <a name = "getting_started"></a>
1. I read through the man page for the linux ps command as well as skimmed the /proc filesystem to understand how my project's final output should look like as well as what files within the /proc filesystem I should be parsing to return certain information. I also read the pdf linked in the assignment which had many snippets of code that would later inspire my own. It taught me how to use a buffer for parsing as well as new functions useful for the implementation of this program.
2. I Created to practice C files called cpuinfo.c and meminfo.c which parse through /proc/cpuinfo and /proc/meminfo respectfully. cpuinfo.c returns the number of processors and the cach size for each one. MemInfo returns the amount of free memory and memory buffers in the system. They are both implemented the same. Both have a function that takes a file as an argument and parses through the file. The file is opened in their main functions which are /proc/cpuinfo and /proc/meminfo.
3. In the main assignment I will be parsing through /proc/<pid>/stat and /proc/<stat>/statm directories for some of the options. The function call by itself will parse all of /proc.
   
### Prerequisites
- GCC compiler
- Linux Server to access /proc filesystem as well as man pages for dirent functions and ps commands

## Implementation <a name = "implementation"></a>
### Struct to Hold Global Variables in Options Module
#### Create a struct to hold flags for displaying certain information about the process
1. int pid is initialized -1 by default. If we use the -p option, pid takes the value of the specific pid stated after -p.
2. int state is initialized as 0 by default. If -s is called, state takes the value of 1.
3. int usr_time is initialized as 1 by default. If -U is called, user_time takes the value of 0.
4. int sys_time is initialized as 0 by default. If -S is called, sys_time takes the value of 1.
5. int vir_mem is initialized as 0 by default. If -v is called, vir_mem takes the value of 1.
6. int cmd_line is initialized as 1 by default. If -s is called, cmd_line takes the value of 0.

### Option Parsing
1. Since the first thing that is required for this program to function is the user's input, I decided that the first module should parse the command line for the options. This module would take argc and *argv[] and the struct Options as parameters. 
2. Used a while loop with the getopt() function to parse through the options in the command line. argc should tell us how many times to loop, argv should tell us the inout of the user and the Options struct should store that input for other modules to access.

### Parsing Process Information
1. Create a function for parsing. This function will take 2 arguments: the pid and options struct.
2. Using a technique learned from the pdf provided in Canvas, I will create a buffer for the path of the specified PID and print the PID into the terminal.
3. Next will be a series of conditionals related to the values stored in the Options struct:
   - e.g. if(option == 1){
       * Write the path to the file that holds the info requested by the option into a buffer
       * Open that file,
       * scan it,
       * and print it into the terminal.
   - }
   
### Parsing Process Directory
Relatively simple once the above module is completed
1. Used a DIR *dir to point to the /proc directory and a struct dirent *current_process to hold the pid of the current process.
2. Using a while loop, loop through all entries in the /proc directory and stop once we see a digit, which means it is a pid of a process.
3. We store that PID into a variable and use it to call the parse_process_info(int pid) function.
4. We continue until readdir(/proc) = NULL

## 🔧 Testing <a name = "testing"></a>
### Testing Option Parsing Function
1. Added print statements in the main function after calling the parse_options() function to see if the function would work properly.
2. When the command line is just ./MYps, the output of the print statements should be the default values.
3. When the command line is ./MYps -p 1 -s -U -S -v -c the output should be 1 1 0 1 1 0.
   * ![Main Function](Images/test0main.png)
   * ![Output](Images/test01.png)
  
### Testing Process Parsing Function
1. Tested each conditional in the parse_process_info() function before coding a new one.
2. Main:
  * ![Main](Images/testmain.png) 
3. State:
  * ![State](Images/test_state.png)
4. User time:
  * ![User Time](Images/test_utime.png)
5. System time:
  * ![System Time](Images/test_stime.png)
6. Virtual Memory:
  * ![Virtual Memory](Images/test_vmsize.png)
7. Command line:
  * ![Command Line](Images/test_cmdline.png)
8. Test Correct Parsing:
  * ![](Images/test_parse.png)
9. Proc Directory:
  * ![Proc Directory](Images/test_parse_dir.png)
### Frequent Errors While Testing
1. Buffers had to be updated to be able to store more data or they might be truncated by stat, statm, and cmdline files
2. procParser.c had an error in it regarding the fscanf() function being used in some of the conditionals but not all. Took some time and research to figure out that fscanf() can skip a variable that already has a length modifier.
3. Had a makefile issue that took the longest to figure out. Makefile script was not recognizing some of the variables and functions within my main function. Added a struct for the options flag and that fixed the problem with my variables not being recognized, however, my functions where still not being recognized. We compiled the code manually and figured there was a problem with my Makefile. Decided to have the makefile just be the command we typed in when we manually compiled and afterwards it worked.

## 🎈 Usage <a name="usage"></a>
1. Download all packages to the same directory with the make file and type command make MYps
2. Usage after compiling:
     * For information on a specific process: ./MYps -p <pid> [options]
     * For all processes: ./MYps [options]
