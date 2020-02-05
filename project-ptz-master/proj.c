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

#define BUF_SIZE 1024
#define LENGTH 30


//Prints help message to screen
void help () {
	fprintf(stderr, "Type \"./ptracer [-f file.txt] command [args]\", ");
	fprintf(stderr, "where command is the command you want to trace, ");
	fprintf(stderr, "and args are necessary arguments, if any.\n");
	fprintf(stderr, "Use the \"-f\" flag to print to a file with the ");
	fprintf(stderr, ".txt extension.\n");
}

//Returns the string for a given register value (long long)
void regToString(const pid_t pid, const unsigned long long addr, char * buff, unsigned int buffer){
    unsigned int readBytes = 0;  			//acts as memory pointer
    long long* read = (long long *) addr;		//read pointer
    long long* copy = (long long *) buff;		//write pointer
    unsigned long long val;				//transfer var
    memset(buff, '\0', BUF_SIZE);
    
    val = ptrace(PTRACE_PEEKTEXT, pid, (read++), NULL);	
    *(copy++) = val;                                   
    readBytes += sizeof(long);
    while(val && readBytes < (buffer - sizeof(long))) {	//while there is data to pull, pull data & place in copy
        val = ptrace(PTRACE_PEEKTEXT, pid, (read++), NULL);
        *(copy++) = val;                                   
        readBytes += sizeof(long);
    }
}


//creates tabular formatted output
//strtok is you friend
//but there are too many things to check for
void tabFormat(char* str, char* target, int count){
    char tmp[LENGTH*2]; 
    int j = 0;           
    
    for(int i=0; i < LENGTH; ++i){ 
        if(str[i] == '\n'){
            tmp[++j] = '\\';
            tmp[++j] = 'n';
        }
        else if(str[i]=='\t'){
            tmp[++j] = '\\';
            tmp[++j] = 't';
        }
        else if(str[i]=='\0'){
            if(count >= LENGTH){
                tmp[++j] = '\\';
                tmp[++j] = '0';
            }
            else 
				tmp[j] = '\0';     
        }
        else
            tmp[j++] = str[i];
    }
    
    tmp[LENGTH - 1] = '\0';//NULL terminate the string
    strcpy(target, tmp);
}


//converts the stat to a series of longs
int statToLong(unsigned long long addr, pid_t child, size_t size, void *buffer) {
    size_t num = 0;
    size_t data;
    unsigned long tmp;

    //if not a multiple of long, exit
    if (size % sizeof(long) != 0) {
        fprintf(stderr, "Memory allocation error.\n");
        exit(EXIT_FAILURE);
	}

    long* buf = (long*) buffer;

    while (num < size) {
        errno = 0;
        tmp = ptrace(PTRACE_PEEKDATA, child, addr + num);
        if (errno)
            return -1;
        buf[num / sizeof(long)] = tmp; 
        num += sizeof(long);
    }
}

//this function takees the syscall, performs conversions where necessary, and prints
//either to screen or file, depending on pointer
//usually called twice for each syscall
//flag == 0 prints syscall info
//flag == 1 prints return value
void getSyscall(FILE *fp, struct user_regs_struct regs, pid_t child, int* flag){
    char str[BUF_SIZE], target[LENGTH];
    int j, stat;
    struct stat temp;
    unsigned long tmp;
    int pipes[2];
    static void* fds_addr;

    switch(regs.orig_rax){ //orig_rax contains syscall val
        case 0: //read
            if(!*flag){
				fprintf(fp, "[pid %d]", child);
                regToString(child, regs.rsi, str, BUF_SIZE); //to get char* buf (path)
                tabFormat(str,target,regs.rdx);	//pass into replace to format for printing
                if(regs.rdx >= sizeof(target))
					fprintf(fp,"read(%lld, \"%s\"..., %lld)",regs.rdi, target, regs.rdx); //printing with register value format
                else 
					fprintf(fp,"read(%lld, \"%s\\n\", %lld)",regs.rdi, target, regs.rdx);
            }
            else 
				fprintf(fp," = %lld\n",regs.rax);
            break;
        case 1: //write
            if(!*flag){
				fprintf(fp, "[pid %d]", child);
                regToString(child, regs.rsi, str, BUF_SIZE);
                tabFormat(str,target,regs.rdx);
                if(regs.rdx >= sizeof(target))
			fprintf(fp,"write(%lld, \"%s\"..., %lld)",regs.rdi, target, regs.rdx);
                else 
			fprintf(fp,"write(%lld, \"%s\\n\", %lld)",regs.rdi, target, regs.rdx);
            }
            else 
		fprintf(fp," = %lld\n",regs.rax);
            break;
        case 2: //open
            if(!*flag){
		fprintf(fp, "[pid %d]", child);
                regToString(child, regs.rdi, str, BUF_SIZE);
                tabFormat(str,target,regs.rdx);
                if(regs.rdx >= sizeof(target))
			fprintf(fp,"open(\"%s\"..., %lld, %lld)",target, regs.rsi, regs.rdx);
                else 
			fprintf(fp,"open(\"%s\\n\", %lld, %lld)",target, regs.rsi, regs.rdx);
            }
            else 
		fprintf(fp," = %lld\n",regs.rax);
            break;
        case 3: //close
            if(!*flag){
		fprintf(fp, "[pid %d]", child);
		fprintf(fp,"close(%lld)",regs.rdi);
	    }
            else 
		fprintf(fp," = %lld\n",regs.rax);
            break;
        case 5: //fstat
            if(!*flag){
		fprintf(fp, "[pid %d]", child);
                statToLong(regs.rsi, child, sizeof(temp), &temp);
                fprintf(fp,"fstat(%lld, {st_mode=",regs.rdi);
                fprintf(fp,"%o, st_size=%lu, ...})",temp.st_mode-stat, temp.st_size);
            }
            else 
		fprintf(fp," = %lld\n",regs.rax);
            break;
        case 8: //lseek
            if(!*flag){
		fprintf(fp, "[pid %d]", child);
                fprintf(fp,"lseek(%lld, %lld, ",regs.rdi,regs.rsi);
                if(regs.rdx == 0) 
			fprintf(fp,"SEEK_SET)");
                else if(regs.rdx == 1) 
			fprintf(fp,"SEEK_CUR)");
                else if(regs.rdx == 2) 
			fprintf(fp,"SEEK_END)");
            }
            else 
		fprintf(fp," = %lld\n",regs.rax);
            break;
        case 22: //pipe
            if(!*flag)
		fds_addr = (void *)regs.rdi;
            else{ //the second syscall must be invoked to receive pipes from first syscall
		fprintf(fp, "[pid %d]", child);
                tmp = ptrace(PTRACE_PEEKTEXT, child, fds_addr, 0); //getting child file descriptors by performing ptrace with PEEKTEXT parameter 
                pipes[0] = tmp & 0x00000000FFFFFFFF; //split the data of the descriptors with pipes
                pipes[1] = tmp >> 32;
                fprintf(fp,"pipe([%d, %d])",pipes[1],pipes[2]);
                fprintf(fp," = %lld\n",regs.rax);
            }
            break;
        case 32: //dup
            if(!*flag){
		fprintf(fp, "[pid %d]", child);
		fprintf(fp,"dup(%lld)",regs.rdi);
	    }
            else 
		fprintf(fp," = %lld\n",regs.rax);
            break;
        case 33: //dup2
            if(!*flag){
				fprintf(fp, "[pid %d]", child);
				fprintf(fp,"dup2(%lld, %lld)",regs.rdi,regs.rsi);
			}
            else 
				fprintf(fp," = %lld\n",regs.rax);
            break;
        case 39: //getpid
            if(!*flag){
				fprintf(fp, "[pid %d]", child);
				fprintf(fp,"getpid()");
			}
            else 
				fprintf(fp," = %lld\n",regs.rax);
            break;
        case 57:  //fork
            if(!*flag){
				fprintf(fp, "[pid %d]", child);
				fprintf(fp,"fork()");
			}
            else 
				fprintf(fp," = %lld\n",regs.rax);
            break;
        case 59: //execve
            if(!*flag){
				fprintf(fp, "[pid %d]", child);
                j = 0;
                regToString(child, regs.rdi, str, BUF_SIZE);
                fprintf(fp,"execve(\"%s\", [",str);
                tmp = ptrace(PTRACE_PEEKDATA, child, regs.rsi); //get reg holding args
                while(tmp > 0){ //iterate through bytes
                    if(j > 0)
						fprintf(fp,", ");
						
                    regToString(child, tmp, str, BUF_SIZE);
                    fprintf(fp,"\"%s\"",str);
                    tmp = ptrace(PTRACE_PEEKDATA, child, regs.rsi+sizeof(long)*++j);
                }
                tmp = ptrace(PTRACE_PEEKDATA, child, regs.rdx); //get & loop over const char *const envp[]
                j = 0;  //j keeps track of the size of bytes for another ptrace 
                while(tmp > 0) {
					tmp = ptrace(PTRACE_PEEKDATA, child, regs.rdx+sizeof(long)*++j);
				}
                fprintf(fp,"], 0x%llx /* %d vars */)",regs.rdx,j);
            }
            else 
				fprintf(fp," = %lld\n",regs.rax);
            break;
        case 61: //wait4
            if(!*flag){
				fprintf(fp, "[pid %d]", child);
				fprintf(fp,"wait4(%lld, 0x%llx, %lld, %lld)",regs.rdi,regs.rsi,regs.rdx,regs.r10);
			}
            else 
				fprintf(fp," = %lld\n",regs.rax);
            break;
        case 231: //exit_group
            if(!*flag){
                fprintf(fp,"exit_group(%lld) = ?\n",regs.rdi);
                fprintf(fp,"+++ exited with %lld +++\n", regs.rdi);
            }
            else 
				fprintf(fp," = %lld\n",regs.rax);
            break;
        default: //any other syscall
            if(!*flag){
				fprintf(fp, "[pid %d]", child);
                fprintf(fp,"%lld(%lld,%lld,%lld,%lld,%lld,%lld)", regs.orig_rax, regs.rdi, regs.rsi, regs.rdx, regs.r10, regs.r8, regs.r9);
            }
            else 
				fprintf(fp, " = %lld\n", regs.rax);
            break;

    }
}

//prints child signal info
void sigPrint(FILE * fp, siginfo_t si){
    char sig_str[16];
    fprintf(fp, "--- %d {si_signo=%d, si_code=%d, si_pid=%d, si_uid=%d, si_status=%d} ---\n",
            si.si_signo, si.si_signo, si.si_code, si.si_pid, si.si_uid, si.si_status);
}
