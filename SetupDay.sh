#!/bin/bash

# Pass in the day number - it will create the folder and files
echo This script sets up the days folder and creates the necessary files. Only run it before starting to code

dayNumber="Day$1"
echo Setting up $dayNumber

folderName=$dayNumber
echo Making Directory $folderName
mkdir $folderName

cppFileName=$dayNumber.cpp
if test -e $folderName/$cppFileName; then
    echo $cppFileName already exists!
else
    echo Creating cpp file $cppFileName
    cp "CommonContents.txt" $folderName/$cppFileName
fi

inFileName=$dayNumber.in
if test -e $folderName/$inFileName; then
    echo $inFileName already exists!
else
    echo Creating in file $inFileName
    touch $folderName/$inFileName
fi

readMeFileName=$dayNumber.md
if test -e $folderName/$readMeFileName; then
    echo $readMeFileName already exists!
else
    echo Creating readme file $readMeFileName
    touch $folderName/$readMeFileName
fi

echo Done
