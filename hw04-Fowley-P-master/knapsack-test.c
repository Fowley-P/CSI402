//This is just to test the various functions of the program.

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "knapsack.h"

int main(int argc, char* *argv){

    listitemptr k1 = NULL;
    int returnval;

    returnval = KnapsackSize(&k1);
    assert(returnval == 0); 

    KnapsackAdd(&k1, 10);   
    KnapsackAdd(&k1, -20);
    KnapsackAdd(&k1, 10);
    KnapsackAdd(&k1, 15);
    KnapsackAdd(&k1, -20); 
    KnapsackAdd(&k1, 10);
    KnapsackPrint(&k1);
    returnval = KnapsackItemCount(&k1, 10);
    assert(returnval == 3);
    returnval = KnapsackItemCount(&k1, -20);
    printf("%d", returnval);

    assert(returnval == 2);


    returnval = KnapsackItemCount(&k1, 8);
    assert(returnval == 0);

    returnval = KnapsackSize(&k1);
    printf("%d", returnval);
    assert(returnval == 6);

    listitemptr k2 = NULL;

    KnapsackAdd(&k2, 5);   
    KnapsackAdd(&k2, 10);
    KnapsackAdd(&k2, 15);
    KnapsackAdd(&k2, 20);
    KnapsackAdd(&k2, 10); 
    
    KnapsackRemove(&k2, 15);
    KnapsackRemove(&k2, 10);

    KnapsackAdd(&k1, 10); 
    returnval = KnapsackSize(&k2);
    assert(returnval == 3);

    returnval = KnapsackSize(&k1);
    assert(returnval == 7);
    printf("Test passed\n");
return 0;
}
