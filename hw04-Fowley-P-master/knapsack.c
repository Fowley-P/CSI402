#include <stdio.h>
#include <stdlib.h>
#include "knapsack.h"

listitemptr KnapsackAdd(listitemptr *knapsack, int item) {
	
	if (*knapsack == NULL) {
		listitemptr newL = (listitemptr)malloc(sizeof(struct listitem));
		newL->item = item; 
		newL->count = 1;
		newL->next = NULL;
		*knapsack = newL;
		newL = newL->next;
		return *knapsack;
	}
	
	else {
		listitemptr temp = *knapsack;
		if (temp->item == item)
			temp->count = temp->count +1;
		else if (temp->item > item) {
			listitemptr newL = (listitemptr)malloc(sizeof(struct listitem));
			newL->item = item;
			newL->count = 1;
			*knapsack = newL;
			newL->next = temp;
			newL = NULL;
			temp = NULL;
			return *knapsack;
		}
		else {
			while (temp->next != NULL && temp->next->item < item) {
				temp = temp->next;
			}
			if (temp->next != NULL && temp->next->item == item) {
				temp->next->count = temp->next->count +1;}
			else {	
				listitemptr newL = malloc(sizeof(struct listitem));
				if (temp->next == NULL) {
					newL->item = item;
					newL->count = 1;
					temp->next = newL;
					newL->next = NULL;
				}
				else if (temp->next->item > item) {
					newL->item = item;
					newL->count = 1;
					newL->next = temp->next;
					temp->next = newL;
					newL->next = NULL;
				}
			}
		}
	}
			
	return *knapsack;
}

int KnapsackRemove(listitemptr *knapsack, int item){
    listitemptr head = *knapsack;
    listitemptr temp = head;
    listitemptr preNode = NULL;
    if(head == NULL) return -1;
    while(temp != NULL){
        if(temp->item == item){
            	if(temp->count > 1){
                	temp->count--;
            	}
	    	else{
                	if(temp == head){
                    		(*knapsack) = head->next;
                    		return 0;
                	}
                	preNode->next = temp->next;
            	}
            	return 0;
        	}
        	preNode = temp;
        	temp = temp->next;
    	}
    	return -1;//not found 
}

void KnapsackPrint (const listitemptr *knapsack) {
	listitemptr temp = *knapsack;
	printf("knapsack: ");
	if(temp != NULL) {
		printf("%d (%d)", temp->item, temp->count);
		while (temp->next != NULL) {
			temp = temp->next;
			printf(", %d (%d)", temp->item, temp->count);
		}
		temp = NULL;
	}
	printf("\n");
}

unsigned int KnapsackItemCount (const listitemptr *knapsack, int item) {
	listitemptr temp = *knapsack;
	while (temp != NULL) {
			if (temp->item == item)
				return temp->count;
				
			temp = temp->next;
		}
	return 0;
}

unsigned int KnapsackSize (const listitemptr *knapsack) {
	listitemptr temp = *knapsack;
	int tot = 0;
	if (temp == NULL)
		return 0;
	else {
		while (temp->next != NULL) {
			tot = tot + (temp->item * temp->count);
			temp = temp->next;
		}
		tot = tot + (temp->item * temp->count);
		temp = NULL;
		return tot;
	}
}

void KnapsackEmpty (listitemptr *knapsack) {
	free(*knapsack);
	*knapsack = NULL;
}
	
		
