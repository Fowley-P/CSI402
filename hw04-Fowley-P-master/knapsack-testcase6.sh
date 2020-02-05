#!/bin/bash

echo "Test case 6"
echo "Save and load a knapsack"
echo -n "" > expectedstderr.tmp
echo -ne "> knapsack: -5 (1)\n> stored in file \"k1\"\n> knapsack: -5 (2)\n> loaded from file \"k1\"\nknapsack: -5 (1)\n> " > expectedstdout.tmp
echo -e "a -5\ns k1\na -5\nl k1\nq" | ./knapsack-shell 1>actualstdout.tmp 2>actualstderr.tmp &
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
