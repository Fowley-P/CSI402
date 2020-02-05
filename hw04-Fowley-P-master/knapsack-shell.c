#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "knapsack.h"

void KnapsackStore(listitemptr *knapsack, FILE *fp) {
	
	listitemptr temp = *knapsack;
	while(temp->next != NULL){
		fwrite(temp, sizeof(struct listitem), 1, fp);
        temp = temp->next;
    }
    fwrite(temp, sizeof(struct listitem), 1, fp);
}

void KnapsackRead(listitemptr *knapsack, FILE *fp) {
    
    listitemptr temp = (listitemptr) malloc(sizeof(struct listitem));
    listitemptr nodeT = *knapsack;
    
    while(fread(temp, sizeof(struct listitem), 1, fp)){
		listitemptr newN = (listitemptr) malloc(sizeof(struct listitem));
		newN->item = temp->item;
		newN->count = temp->count;
		newN->next = NULL;
		if(nodeT == NULL) {
			*knapsack = newN;
			nodeT = *knapsack;
			nodeT->next = NULL;
        } 
        else{
			nodeT->next = newN;
            nodeT = nodeT->next;
            nodeT->next = NULL;
        }
        newN = NULL;
    }
    nodeT->next = NULL;
}
		
int entryCheck(char *cmd) {
	int i = strlen(cmd);
	if (cmd[0] == '\0')
		return 0;
		
	for(int c = 0; c < i; c++) {
		if (isalpha(cmd[c]) || (cmd[c] == ' '))
				return 0;
	}
	
	return 1;
}
	

int main (int argc, char **argv) {
	FILE *fp;
	listitemptr k1 = NULL;
	char *in = (char *)malloc(20*sizeof(char));
	char *cmd = (char *)malloc(18*sizeof(char));
	
	while (1) {
		printf("> ");
		fgets(in, 15, stdin);
		in[strcspn(in, "\n")] = 0;
		if (in[0] != 'q' && in[1] != ' ' && in[0] != 'p')
			fprintf(stderr, "Unknown command: %s\n", in);
		
		else {
			cmd = in +2;
			if((in[0] == 'a') || (in[0] == 'r')) {
				if (entryCheck(cmd) == 1) {
					if (in[0] == 'a')
						KnapsackAdd(&k1, atoi(cmd));
					else
						KnapsackRemove(&k1, atoi(cmd));
					
					KnapsackPrint(&k1);
				}
			else
				fprintf(stderr, "Error: Bad item \"%s\"\n", cmd);
				
			}
			else if(in[0] == 'p')
				KnapsackPrint(&k1);
			
			else if(in[0] == 's') {
				if((fp = fopen(cmd, "wb")) == NULL)
					fprintf(stderr, "Error: unable to save file \"%s\"\n", cmd);
				
				else {
					KnapsackStore(&k1, fp);
					printf("stored in file \"%s\"\n", cmd);
					fclose(fp);
				}
			}
			
			else if(in[0] == 'l') {
				
				if((fp = fopen(cmd, "rb")) == NULL)
					fprintf(stderr, "Error: unable to open file \"%s\"\n", cmd);
				
				else {
					KnapsackEmpty(&k1);
					KnapsackRead(&k1, fp);
					fclose(fp);
					if (k1 != NULL) 
						printf("loaded from file \"%s\"\n", cmd);
						KnapsackPrint(&k1);
				}
			}		
			
			else if(in[0] =='q') {
				break;
			}
			
			else
				fprintf(stderr, "Unknown command: %s\n", in);
			
			}
		}
	KnapsackEmpty(&k1);
	return 0;
}

