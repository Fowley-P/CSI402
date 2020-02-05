#include <sys/ptrace.h>
#include <sys/reg.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "ptracer.h"

FILE *fp;

int main(int argc, char *argv[]){
	//flag to alternate info printed
	//skip to determine how many argv to ignore
	//fFlag to determine if "-f" flag is used
	//other variables are necessary for ptrace execution
    int status, flag = 0, sigFlag= 0, skip = 1, fFlag = 0;
    struct user_regs_struct regs;
    siginfo_t siginfo;

	//if the program is invoked without sufficient arguments
	//kick it back
	if (argc < 2) {
		fprintf(stderr, "Insufficient arguments.\n");
		exit(-1);
	}
	
	//if they need help, print help
	//if there are other arguments, and they need help
	//they probably got it wrong, so ignore the other inputs
	if (strcmp(argv[1], "-h") == 0) {
		help();
		return 0;
	}
	
	//if they use "-f", make sure there are sufficient arguments
	//if not, kick it back
	//if yes, set fFlag on, grab fileName, and augment skip
	if (strcmp(argv[1], "-f") == 0) {
		if (argc < 4) {
			fprintf(stderr, "Insufficient arguments provided.\n");
			fprintf(stderr, "Please supply sufficient arguments, or use -h for help.\n");
			exit(EXIT_FAILURE);
		} 
		else {
			fFlag = 1;
			char* fileName = argv[2];
			fp = fopen(fileName, "w");
			skip += 2;
		}
	}
	
    

    int newArgc = argc-skip; 				//remove all arguments except commands
    char *info [newArgc+1];       
    memcpy(info, argv+skip, newArgc * sizeof(char*)); 	//copy argv to info
    info[newArgc] = NULL;       			//terminate with NULL


    pid_t child = fork();
    //set options while tracing the child
    //PTRACE_O_TRACEEXIT stops tracee when it exits
    //PTRACE_O_TRACESYSGOOD is the best way to differentiate between TRAP and syscall
    //PTRACE_O_TRACEFORK & PTRACE_O_TRACECLONE stops tracee at FORK or CLONE respectively
    //and follows the new thread
    ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_TRACESYSGOOD | PTRACE_O_TRACEEXEC | PTRACE_O_TRACEFORK | PTRACE_O_TRACECLONE);

	//if the fork fails, kick it back
    if(child == -1) {
		fprintf(stderr, "Fork failure.\n");
		exit(-1);
    }
    else if (child == 0) {
        ptrace(PTRACE_TRACEME);     //launch trace
        kill(getpid(), SIGSTOP);    //kill to start trace
        execvp(info[0], info);      //execute command

    } 
    else {
        waitpid(child, 0, 0);
        while(1) {

            //flag = 0 wait for the next system call.
            //flag = 1 execute syscall, wait for the return.
            ptrace(PTRACE_ATTACH, child, 0, 0);
            ptrace(PTRACE_SYSCALL, child, 0, 0); //Continue, but stop at the next system call entrance or exit.
            waitpid(child, &status, 0);
		
		//if any of these conditions are true, exit the program
            if (WIFEXITED(status)) 
				exit(0);   //check if child process ended normally
            if (WIFSIGNALED(status)) 
				exit(0); //check if signal caused child to exit
            if (!WIFSTOPPED(status)) 
				exit(0); //check if child is stopped.


            if(WSTOPSIG(status) != SIGTRAP){ //check if signal was TRAP
                ptrace(PTRACE_GETSIGINFO, child, 0, &siginfo); //get child signal
                if(!flag){
                    sigPrint(stdout, siginfo);
                    if(fp)
						sigPrint(fp, siginfo);
                    continue;
                }
            }


            ptrace(PTRACE_GETREGS, child, NULL, &regs); //copy tracee’s registers.

            if(!flag){
                if (regs.orig_rax == 59 && regs.rax == 0)
					continue;
                if (regs.orig_rax == 61 && regs.rax > 0)
					continue;               
            }

            //flag == 0 prints syscall data
            //flag == 1 prints syscall return
            getSyscall(stdout,regs,child,&flag); //prints syscall data to stdout
            if(fFlag == 1)
				getSyscall(fp, regs, child, &flag); //prints syscall data to supplied file

            flag = !flag;
        }
    }
    if(fp)
		fclose(fp); //if a pointer was intialized, close it because memory leaks are bad, mmmkay
		
    return 0;
}
