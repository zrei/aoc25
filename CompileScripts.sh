#!/bin/bash

# Pass in the day number - it will find the cpp file, compile it, run it and pass in the input file. You can also pass in an optional second input to use a different input file
echo This script compiles the cpp file and passes in the input file. Provide alternative input file as optional second input

dayNumber="Day$1"
echo Compiling $dayNumber 

folderName=$dayNumber
cppFileName=$dayNumber.cpp
outFileName=$dayNumber.out
g++ --std=c++20 $folderName/$cppFileName "fileRead.cpp" -o $folderName/$outFileName
echo Done compiling

echo Running $dayNumber
if [ -n "$2" ]
then
inFileName="$2"
else
inFileName=$dayNumber.in
fi
echo Using input file $inFileName
./$folderName/$outFileName ./$folderName/$inFileName

echo Done
