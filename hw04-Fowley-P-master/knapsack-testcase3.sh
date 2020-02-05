#!/bin/bash

echo "Test case 3"
echo "Adding a couple items to the knapsack and printing it in knapsack-shell"
echo -n "" > expectedstderr.tmp
echo -ne "> knapsack: 2 (1)\n> knapsack: 1 (1), 2 (1)\n> knapsack: 1 (1), 2 (1)\n> " > expectedstdout.tmp
echo -e "a 2\na 1\np\nq" | ./knapsack-shell 1>actualstdout.tmp 2>actualstderr.tmp &
sleep 1
killall -9 knapsack-shell

echo "	Expected stdout:"
cat expectedstdout.tmp
echo ""
echo "	Expected stderr:"
cat expectedstderr.tmp

stdoutres=`diff expectedstdout.tmp actualstdout.tmp | wc -l`
stderrres=`diff expectedstderr.tmp actualstderr.tmp | wc -l`
if [ $stdoutres -eq 0 ] && [ $stderrres -eq 0 ]; then
    echo "Test passed"
else 
    if [ $stdoutres -ne 0 ]; then
        echo "	Your stdout:"
        cat actualstdout.tmp
        echo ""
    fi
    if [ $stderrres -ne 0 ]; then
        echo "	Your stderr:"
        cat actualstderr.tmp
    fi
    rm *.tmp
    exit 1
fi
rm *.tmp

