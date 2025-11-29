#!/bin/bash

# Pass in the day number - it will find the cpp file, compile it, run it and pass in the input file
echo This script compiles the cpp file and passes in the input file

dayNumber="Day$1"
echo Compiling $dayNumber 

folderName=$dayNumber
cppFileName=$dayNumber.cpp
outFileName=$dayNumber.out
g++ --std=c++20 $folderName/$cppFileName "fileRead.cpp" -o $folderName/$outFileName
echo Done compiling

echo Running $dayNumber
inFileName=$dayNumber.in
./$folderName/$outFileName ./$folderName/$inFileName

echo Done
