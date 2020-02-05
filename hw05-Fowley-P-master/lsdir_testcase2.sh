#!/bin/bash

echo "Test Case 2: directory without execute permission"

if ! [ -e ~/resultsdir ]; then
    mkdir ~/resultsdir
    ln lsdir ~/lsdir
else
    echo "Error: resultsdir already exists"
    exit 2
fi

mkdir testcase2
cd testcase2
chmod -x .

ls -alU --time-style=long-iso 1>~/resultsdir/expectedstdout.tmp 2>~/resultsdir/expectedstderr.tmp
~/lsdir 1>~/resultsdir/actualstdout.tmp 2>~/resultsdir/actualstderr.tmp

cd ..
rm -R testcase2

echo "ls output"
echo "	On stdout:"
cat ~/resultsdir/expectedstdout.tmp
echo "	On stderr:"
cat ~/resultsdir/expectedstderr.tmp
echo "Your output:"
echo "	On stdout:"
cat ~/resultsdir/actualstdout.tmp
echo "	On stderr:"
cat ~/resultsdir/actualstderr.tmp


if [ `diff -I "^l" ~/resultsdir/expectedstdout.tmp ~/resultsdir/actualstdout.tmp | wc -l` -eq 0 ] && [ `diff ~/resultsdir/expectedstderr.tmp ~/resultsdir/actualstderr.tmp | wc -l` -eq 0 ]; then
    echo "Test passed"
    exitcode=0
else
    echo "Test failed"
    exitcode=1
fi

rm ~/lsdir
rm -R ~/resultsdir

exit $exitcode
