:: Pass in the day number - it will create the folder and files
ECHO This script sets up the days folder and creates the necessary files. Only run it before starting to code

set dayNumber="Day%1"
ECHO Setting up %dayNumber

set folderName=%dayNumber
ECHO Making Directory %folderName
mkdir %folderName

set cppFileName=%dayNumber.cpp
if exist %folderName/%cppFileName (
    ECHO %cppFileName already exists!
) else (
    ECHO Creating cpp file %cppFileName
    cp "CommonContents.txt" %folderName/%cppFileName
)

set inFileName=%dayNumber.in
if exist %folderName/%inFileName (
    ECHO %inFileName already exists!
) else (
    ECHO Creating in file %inFileName
    touch %folderName/%inFileName
)

ECHO Done
