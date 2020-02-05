/* knapsack.h
 * implements simple knapsack data structure as a linked list 
 * NOTE: a function may update the value of input argument *knapsack if it changes the first node of the knapsack to another node. Such a change include the case when an item is added to an empty knapsack
 */

/* pointer to linked list node data structure */
typedef struct listitem* listitemptr;
/* data structure to use as linked list nodes */
struct listitem {
  int item;           // actual int item
  unsigned int count; // number of the same item in the knapsack; should be >= 1
  listitemptr next;   // pointer to next item 
};

/*
 * adds an item to a knapsack. Nodes should be in ascending order. You must simply increase the "count" if the item already exist in the knapsack; "count" must be set to 1 for previously-nonexisting items
 * @param knapsack: pointer to a listitemptr, itself pointing to the first item in a knapsack; NULL if knapsack has not been created yet 
 * @param item: integer item to add
 * @return pointer to the listitem added/updated; NULL if unsuccessful 
 */
listitemptr KnapsackAdd(listitemptr *knapsack, int item);

/*
 * removes a value from a knapsack; must update the "count" and delete the associated listitem when count becomes 0 
 * @param knapsack: [see KnapsackAdd() params]; updated to NULL if knapsack becomes empty
 * @param item: integer item to remove
 * @return 0 if successful, -1 otherwise (when item not found or knapsack is empty)
 */
int KnapsackRemove(listitemptr *knapsack, int item);

/*
 * prints integer items (in ascending order) and their counts in a knapsack
 * @param knapsack: [see KnapsackAdd() params]
 * @stdout: for example, "" (nothing) when knapsack==NULL, or "-125 (4), 10 (1), 26 (2)" when items include four of -125, one of 10, and two of 26
 * @return void
 */
void KnapsackPrint(const listitemptr *knapsack);

/*
 * returns count of a specific item in a knapsack
 * @param knapsack: [see KnapsackAdd() params]
 * @param item: integer item to search for
 * @return item count, or 0 if it does not exist
 */
unsigned int KnapsackItemCount(const listitemptr *knapsack, int item);

/*
 * total count of items in the knapsack
 * @param knapsack: [see KnapsackAdd() params]
 * @return total item count. For example, 7 in case the items are "-125 (4), 10 (1), 26 (2)" (see SnapsackPrint() description)
 */
unsigned int KnapsackSize(const listitemptr *knapsack);

void KnapsackEmpty(listitemptr *knapsack);
