//Patrick Fowley

#include <stdio.h> 
#include <dirent.h> 
#include <unistd.h>
#include <pwd.h>
#include <stdlib.h>
#include <grp.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sysmacros.h>

int main() { 
    struct dirent *direntp;                                             //Pointer for directory entry 
    struct stat fileStat;                                               //stores file info
    char cwd[200];
    char editTime[30];
    long max = 0;
    unsigned long links = 0;
    double temp = 0;
    double ltemp = 0;
    int count = 0;

    struct tm *tmp = malloc(sizeof(struct tm));                         //pointer to time info
    struct passwd *pass = malloc(sizeof(struct passwd));                //pointer to password
    struct group *grp = malloc(sizeof(struct group));                   //pointer to group info

    DIR *dr = opendir(".");                                             //pointer to the current directory

    if(dr == NULL){                                                     //if the directory can't be opened
        perror("ls: cannot open directory '.'");                        //kick it back with an error
        return -1;
    }
    
    //in order for the output to match, the files must be looped through twice to print the total # of links in the right place
    //it's less processor efficient, but much more memory efficient
    while ((direntp = readdir(dr)) != NULL) {
        lstat(direntp->d_name, &fileStat);                              
        if (max < fileStat.st_size){
            max = fileStat.st_size;
        }

        if(links < fileStat.st_nlink){
        	links = fileStat.st_nlink;
        }
        	
        count += fileStat.st_blocks;
    }

    printf("total %d\n", count>>1);                                     //the target architecture printed 2x the links of the target output
    
    //determines how many spaces are needed for proper output alignment
    while(pow(10, temp) < max) {
        temp++;
    }
    while(pow(10, ltemp) < links) {
        ltemp++;
    }

    closedir(dr);
    //close the directory to reopen it from the start
    dr = opendir(".");

    //loop back through to print out the relevant info
    while ((direntp = readdir(dr)) != NULL){
      	count = 0;
        lstat(direntp->d_name,&fileStat);

        printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
        printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
        printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
        printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
        printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
        printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
        printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
        printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
        printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
        printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
        pass = getpwuid(fileStat.st_uid);
        grp = getgrgid(fileStat.st_gid);

        if(fileStat.st_nlink == 0) {
            for(int i = ltemp; i > 0; i--) {
                printf(" ");
            }
        }
        else {
            while(pow(10, (ltemp-count)) > fileStat.st_nlink) {
                printf(" ");
                count++;
            }
        }
       
      	count = 0;

        printf("%lu", fileStat.st_nlink);
        strftime(editTime, sizeof(editTime), "%F %H:%M", localtime(&(fileStat.st_mtime)));
        printf(" %s %s", pass->pw_name, grp->gr_name);

        if(fileStat.st_size == 0) {
            for(int i = temp; i > 0; i--) {
                printf(" ");
            }
        }
        else {
           while(pow(10, (temp-count)) > fileStat.st_size) {
                printf(" ");
                count++;
            }
        }

      	printf("%ld %s", fileStat.st_size, editTime);
        printf(" %s\n", direntp->d_name);
    } 
    closedir(dr);     
    return 0; 
}
