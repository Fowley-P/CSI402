#include <stdio.h>
#include <assert.h>

#include "knapsack.h"

int main(int argc, char* *argv){
    listitemptr k1 = NULL;
    int returnval;

    printf("Test case 2\n");

    printf("Asserting item count for nonexistent item == 0\n");
    returnval = KnapsackItemCount(&k1, 1);
    assert(returnval == 0);

    KnapsackAdd(&k1, 1);
    KnapsackAdd(&k1, 1);
    printf("Asserting knapsack with item (count 2) yields KnapsackItemCount(knapsack, item) == 2\n");
    returnval = KnapsackItemCount(&k1, 1);
    assert(returnval == 2);

    KnapsackEmpty(&k1);
    assert(k1 == NULL);

    printf("Test passed\n");
	return 0;
}
