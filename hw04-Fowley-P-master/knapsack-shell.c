//Patrick Fowley

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "knapsack.h"

/*
 *Stores the current list in an external file
 *@param: pointer to knapsack & file name
 */
void KnapsackStore(listitemptr *knapsack, FILE *fp) {
	
	listitemptr temp = *knapsack;
	while(temp->next != NULL){							//runs through the list
		fwrite(temp, sizeof(struct listitem), 1, fp);				//writes each item to the file
        temp = temp->next;
    }
    fwrite(temp, sizeof(struct listitem), 1, fp);					//writes final item
}

/*
 *Creates a new list from a provided file
 *@param: pointer to knapsack & file
 */
void KnapsackRead(listitemptr *knapsack, FILE *fp) {
    
    listitemptr temp = (listitemptr) malloc(sizeof(struct listitem));
    listitemptr nodeT = *knapsack;							//head of list
    
    while(fread(temp, sizeof(struct listitem), 1, fp)){					//runs through the list
		listitemptr newN = (listitemptr) malloc(sizeof(struct listitem));	//creates new item
		newN->item = temp->item;
		newN->count = temp->count;
		newN->next = NULL;
		if(nodeT == NULL) {							//if list is empty
			*knapsack = newN;						//new item is now the head
			nodeT = *knapsack;
			nodeT->next = NULL;
        } 
        else{
	    nodeT->next = newN;								//else adds the new item to the end
            nodeT = nodeT->next;
            nodeT->next = NULL;
        }
        newN = NULL;
    }
    nodeT->next = NULL;
}

/*
 *Sanitizes inputs
 *@param: user input string
 *@return: 1 if the input is ok
 *@return: 0 if the input is incorrect
 */
int entryCheck(char *cmd) {								
	int i = strlen(cmd);
	if (cmd[0] == '\0')								//catches null strings
		return 0;
		
	for(int c = 0; c < i; c++) {							//loops through input
		if (isalpha(cmd[c]) || (cmd[c] == ' '))					//checks against spaces or non-alpha characters
				return 0;
	}
	
	return 1;
}
	

int main (int argc, char **argv) {
	FILE *fp;									//pointer to file
	listitemptr k1 = NULL;								//list
	char *in = (char *)malloc(20*sizeof(char));					//input
	char *cmd = (char *)malloc(18*sizeof(char));					//command
	
	while (1) {									//runs until the user enters exit command
		printf("> ");
		fgets(in, 15, stdin);
		in[strcspn(in, "\n")] = '\0';						//terminates string where the user hits enter
		if (in[0] != 'q' && in[1] != ' ' && in[0] != 'p')			//p & q are the only commands which don't require extra info
			fprintf(stderr, "Unknown command: %s\n", in);			//so if the first character isn't p or q
											//but the 2nd is a space, not a command
		else {
			cmd = in +2;							//skips to the info
			if((in[0] == 'a') || (in[0] == 'r')) {				//checks to see if the command adds or removes an element
				if (entryCheck(cmd) == 1) {				//in an already existing list
					if (in[0] == 'a')				//a == add
						KnapsackAdd(&k1, atoi(cmd));
					else						//r == remove
						KnapsackRemove(&k1, atoi(cmd));
					
					KnapsackPrint(&k1);
				}
				else							//if the entry isn't correct
				    fprintf(stderr, "Error: Bad item \"%s\"\n", cmd);	//kick it back and shout at them
				
			}
			else if(in[0] == 'p')						//p == print
				KnapsackPrint(&k1);
			
			else if(in[0] == 's') {						//s == save to file
				if((fp = fopen(cmd, "wb")) == NULL)			//attempts to open file, error if failure
					fprintf(stderr, "Error: unable to save file \"%s\"\n", cmd);
				
				else {							//if the file is opened successfully
					KnapsackStore(&k1, fp);				//runs store function
					printf("stored in file \"%s\"\n", cmd);
					fclose(fp);					//memory leaks are bad, mmkay
				}
			}
			
			else if(in[0] == 'l') {						//l == load from file
				
				if((fp = fopen(cmd, "rb")) == NULL)
					fprintf(stderr, "Error: unable to open file \"%s\"\n", cmd);
					
				else {							
					KnapsackEmpty(&k1);				//must read from file to empty list
					KnapsackRead(&k1, fp);				//runs load function
					fclose(fp);
					if (k1 != NULL) 
					    printf("loaded from file \"%s\"\n", cmd);
					    KnapsackPrint(&k1);
				}
			}		
			
			else if(in[0] =='q') {						//q == quit
				break;
			}
			
			else
				fprintf(stderr, "Unknown command: %s\n", in);
			
			}
		}
	KnapsackEmpty(&k1);								//frees memory
	return 0;
}

