
## **Tanish Gupta | Ubuntu 22.04 LTS**


# Compilation and Exectution Instructions
In the directory containing the program files, run the following commands on the terminal to compile and create the executable shell and run the exectuable file obtained.

```
$ make 
$ ./a.out
```

# Shell Description
> To give functionality for commands that are implemented directly in a linux terminal, the shell is fundamentally constructed from C programmes structured in a modular form. All system commands, user commands, and built-in commands can be executed by it. Some system commands are implemented through programme files written in C, while others are run in the background or front depending on their needs using a straightforward fork. The functions of each file are listed below, followed by a list of potential shell commands.
>

# Functionality
`warp`
* It is an error for the `warp` command to have more than one command-line argument
* It is an error for the `warp` command, if the target directory doesn't exist
* `warp` supports flags such as `'.'`, `'..'`, `'~'`, `'-'`

`peek`
* It outputs the contents of a file, if it exits in different colours
* If it doesn't exist, it throws an error 

`proclore`
* `proclore` prints the process - related info of the shell program, if no argument is given
* `proclore` prints the process - related info of the given pid, if an argument in the form of a pid of a process is given
* It is an error for the `proclore` command to have more than one command-line argument
* The process-related info, consists of the pid, the status code, the virtual memory and the executable path of the process
* The status code is followed by a '+' if the proces is running in the foreground

`pastevents`
* This command stores the history, for a maximum of 20 commands, by creating a file `history.txt` in the same folder as the other program files
* This file is being created, destroyed and again created upon the entry of a new command, to update the `history.txt` file 

`system commands`
* The system commands other than the one mentioned above can be runned both in foreground and in background
* If the process runs in the background, the completion message would come immediately, and on presseing an enter, the new prompt comes.
* Errors have been handled at the requisite areas
* On ending a background process, complete command is shown

`seek`
* 'seek’ command looks for a file/directory in the specified target directory (or current if no directory is specified). It returns a list of relative paths (from target directory) of all matching files/directories (files in green and directories in blue) separated with a newline character.

`activities`
* print a list of all the processes currently running that were spawned by your shell in lexicographic order. This list should contain the following information about all processes like Command Name, pid, state : running or stopped

`ping`
* ping command is used to send signals to processes. Take the pid of a process and send a signal to it which corresponds to the signal number (which is provided as an argument). 

`fg`
* Brings the running or stopped background process with corresponding pid to foreground, handing it the control of terminal. Print “No such process found”, if no process with given pid exists.

`bg`
* Changes the state of a stopped background process to running (in the background). If a process with given pid does not exist, print “No such process found” to the terminal.

`neonate`
* The command prints the Process-ID of the most recently created process on the system (you are not allowed to use system programs), this pid will be printed every [time_arg] seconds until the key ‘x’ is pressed.

# Assumptions
  > Apart from the assumptions taken for the functioning of all the commands, some general assumptions have also been taken, stated as follows :-

* The maximum size of a single command, directory, path to a directory is taken to be 1000 using the macro defined as `#define MAX_ARGS 100` 
* Error handling for standard C functions is being done using `perror` whereas for some functions, it is done by printing off the error message in simple error detected form and exiting
* The maximum commands stored in the history are 15
* The maximum files that can be printed by Peek command are 4096 
* Anything inside inverted commas will be treated as a single entity and user requires to put inverted commas for anything with white spaces, example echo "lorem ipsum" will print lorem ipsum
* If in seek no match is found i have still printed it in history
* I have given color of executables as green
* I have also in warp function showed that which directory to my warp changes to.
* I have made the functionality to print the background processes automatically without the users input for the sake of convinence. (personal preferrence)
* I have assumed that for i/o redirection there will be input / output files on either sides of the operator 
* Assumed that activities will only print for the processes spawned by the shell
* Assumed that for fg the command with that pid will be executed in the foreground
* Starting neonate will require one to enter the neonate command correctly and then press x to start and end it






