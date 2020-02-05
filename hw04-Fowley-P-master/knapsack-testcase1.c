#include <stdio.h>
#include <assert.h>

#include "knapsack.h"

int main(int argc, char* *argv){
    listitemptr k1 = NULL;
    int returnval;

    printf("Test case 1\n");

    printf("Asserting empty knapsack has size 0\n");
    returnval = KnapsackSize(&k1);
    assert(returnval == 0);

    KnapsackAdd(&k1, 1);
    printf("Asserting knapsack with one item (count 1) has size 1\n");
    returnval = KnapsackSize(&k1);
    assert(returnval == 1);

    KnapsackEmpty(&k1);
    assert(k1 == NULL);

    printf("Test passed\n");
return 0;
}
