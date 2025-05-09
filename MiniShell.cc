#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64
#define MAX_PATH 1024


// You are to write a C program (not C++) that will act as a mini Linux shell

// Basic Shell:

// Your shell program should display a prompt, get the input from the user, execute the specified command, and then repeat.

// For the prompt I want you to display the current working directory followed by a >, as in (although your directory path will be different):

// /Users/dsteven/Teaching/DU/shell>

// You may use the getcwd function to get the current working directory.  It takes in a character buffer (e.g. array) and the size of the buffer as its two arguments and will fill in this buffer.

// Once you have read in the line from the user, it will need to be parsed into a string of its individual tokens.  To work correctly with execv you will want to end up with an array of stings where each element is a token in the command line.  The last element in the array should be NULL.  You should use the strtok function to tokenize the input line.

// Now that you have the input broken into tokens, you should do something different for each requested command.  There are two basic types of commands.  For this section we will focus just on shell specific commands.  To start, you should include 2 shell specific commands: exit and cd. 

// Exit should simply stop your shell program from running.

// Cd should do a standard change directory.  You will need to use the function chdir to accomplish this task.  You simply pass it the string of the directory to which you wish to change.  Note that this function will return a non-zero value if it cannot change to that directory and you should inform the user this occurred.

// Test your basic shell by changing directories a few times and then exiting.

 

// Executing Programs:

// We can get a lot more power out of our shell by allowing it to execute programs.  This will not only include programs that we write, but also built-in systems ones such as ‘ls –l’ or ‘ps’.  To do this, you will need to fork a child process, have the child process perform an execvp, and have the parent process wait for the child to finish.

// Forking is accomplished by the use of the fork command, no parameters.  It will return the process id (pid) of the child created to the parent and a pid of 0 to the child.

// To have a process start executing another programs code you use the execvp function.  This takes two arguments.  The first is the name of the program to run, such as “ls”.  The second should be an array of strings with all the arguments necessary to run it.  For example, slot 0 could be “ls” and slot 1 could be “-1”.  To make this run smoothly, I would have your parser return the tokens in this format.

// Waitpid can be used to have the parent wait for the child.  This takes 3 parameters.  The first is either the pid of the child to wait for or else a -1 indicating it should wait for any child it created.  The second should just be a pointer to an int in which it will place a status code.  And the last parameter should either be a 0 if we want it to block, or a WNOHANG (stdlib.h) if we wish it to return immediately.

// If the user tries to execute a program that does not exist, they should be told that and the command prompt should be given again.

 

// Input/Output Redirection:

// For this part you are to add input and output redirection.  It sounds complex but it is really simple.  If there is a redirection it will show up as a > or < followed by the name of the file that the output/input should go to/come from.  Thus, your first task would be to search for a ‘<’ or a ‘>’ as a token.  I highly suggest you make helper methods to check for output redirection and input redirection.  Note that you can have both input and output in the same command and that they may occur anywhere after the command name.  You only need to handle redirection in the case of executing linux programs (i.e. you don’t need to redirect shell specific commands such as 'cd').

// If you find the redirection token ('<' or '>') then the next token is simply the filename.  Note that since your shell will be taking care of the redirection, you should remove the token and associated filename from your token array so that when the command is executed (execvp) they do not exist in the array.

// To redirect use the freopen function.  This remaps one stream to another stream.  In our case we want to remap either stdin or stdout to the file specified.  The first parameter is always the name of the file.  The second and third parameters are either “w” and stdout, or “r” and stdin. 

// Test your output redirect code by redirecting something like ls –l to foo.txt.  But to test your input redirect, you will have to create a testing program that does a scanf.

 

// Background Processes:

// Lastly, you need to add the ability for programs to be run in the background.  This will be indicated by a & at the end of the command.  It can only occur at the end.  You already have the shell setup to handle making processes and waiting for them to finish.  This is simply an adjustment on that.  The shell should allow the user to execute other commands while the background command is being executed.

// You may want to add a signal handler into your code to work with the SIGCHLD signal.  This signal occurs when a process dies.  You can setup a handler to catch it with the signal function.  This function takes two parameters.  The first is the signal type.  For us, SIGCHLD is probably what you want to use (signal.h).  The second is the name of the function to handle that signal.

// You will need to find a way to reap zombies.  That is, your shell is a long-running program and cannot simply leave zombie processes in the system.  A good way to test to see how your code is doing is to create a simple test program that simply loops or sleeps for a set amount of time.  Then you can place this program in the background.  See if you can run other programs while it is running in the background.  And then make sure it doesn’t end up a zombie when it is done executing.  Recall that you can always execute the command ‘ps’ through your shell to check on the processes. 

// Here is a list of things you do not need to worry about. You do not need to inform the user when the background program ends.  You do not need to handle suspending (ctrl-z) processes or placing them into the background after the command has already been run (i.e. bg in the built-in shells).  You do not need to be able to take a shell that was in the background and put it into the foreground (i.e. fg).  You do not need to handle programs that do interactive input/output while in the background (i.e. emacs, vi, vim).  You do not need to list all the processes you have placed in the background (i.e. jobs)  If you have any other questions as to what you should be able to handle just ask.

void signal_handler(int sig) {
    while (waitpid(-1, NULL, WNOHANG) > 0) {}
}

void prompt(char *cwd) {
    printf("%s> ", cwd);
}

int parse_input(char *input, char **args, char **input_file, char **output_file, int *background) {
    int arg_count = 0;
    *background = 0;
    *input_file = NULL;
    *output_file = NULL;

    char *token = strtok(input, " \n");
    while (token != NULL) {
        if (strcmp(token, "<") == 0) {
            token = strtok(NULL, " \n");
            *input_file = token;
        } else if (strcmp(token, ">") == 0) {
            token = strtok(NULL, " \n");
            *output_file = token;
        } else if (strcmp(token, "&") == 0) {
            *background = 1;
        } else {
            args[arg_count++] = token;
        }
        token = strtok(NULL, " \n");
    }

    args[arg_count] = NULL;
    return arg_count;
}

void execute_command(char **args, char *input_file, char *output_file, int background) {
    pid_t pid = fork();

    if (pid == 0) { // Child process
        if (input_file) {
            freopen(input_file, "r", stdin);
        }
        if (output_file) {
            freopen(output_file, "w", stdout);
        }

        if (execvp(args[0], args) == -1) {
            perror("Execution failed");
        }
        exit(EXIT_FAILURE);
    } else if (pid > 0) { //parent process
        if (!background) {
            waitpid(pid, NULL, 0); //wait for the child process to finish
//pid: This is the process ID you are waiting on. The behavior of waitpid changes based on the value of pid:

// If pid is greater than 0, waitpid waits for the child with a process ID equal to pid.
// If pid is 0, waitpid waits for any child process whose process group ID is equal to that of the calling process.
// If pid is -1, waitpid waits for any child process; this is similar to the wait() function.
// If pid is less than -1, waitpid waits for any child process whose
        } else {
            printf("Process running in background [PID: %d]\n", pid);
        }
    } else {
        perror("Fork failed");
    }
}

int main() {
    char input[MAX_INPUT];
    char *args[MAX_ARGS];
    char cwd[MAX_PATH];
    char *input_file, *output_file;
    int background;

    signal(SIGCHLD, signal_handler); //signal handler for background processes
    //this threw me off but i got it now

    while (1) {
        getcwd(cwd, sizeof(cwd)); //grab current directory
        prompt(cwd);

        if (fgets(input, MAX_INPUT, stdin) == NULL) {
            perror("Error reading input");
            continue;
        }

        if (strlen(input) == 1) continue; //output may be empty, ignore it 

        //parse input into command and arguments
        parse_input(input, args, &input_file, &output_file, &background);

        //exit command
        if (strcmp(args[0], "exit") == 0) {
            break;
        }

        //cd command
        if (strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL || chdir(args[1]) != 0) {
                perror("cd failed");
            }
            continue;
        }

        //testing
        execute_command(args, input_file, output_file, background);
    }

    return 0;
}