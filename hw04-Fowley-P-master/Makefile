

all: knapsack.c knapsack-test.c knapsack-shell.c
	gcc knapsack.c knapsack-test.c -o knapsack-test
	gcc knapsack.c knapsack-shell.c -o knapsack-shell

knapsack-test: knapsack.c knapsack-test.c
	gcc knapsack.c knapsack-test.c -o knapsack-test

knapsack-shell: knapsack.c knapsack-shell.c
	gcc knapsack.c knapsack-shell.c -o knapsack-shell

knapsack.o: knapsack.c knapsack.h
	gcc -c knapsack.c

clean:
	rm -rf knapsack-test knapsack-shell knapsack.o
