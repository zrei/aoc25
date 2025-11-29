:: Pass in the day number - it will find the cpp file, compile it, run it and pass in the input file
ECHO This script compiles the cpp file and passes in the input file

set dayNumber="Day%1"
ECHO Compiling %dayNumber

set folderName="%dayNumber"
set cppFileName="%dayNumber.cpp"
set outFileName="%dayNumber.out"
CL /std:c++20 %folderName/%cppFileName "fileRead.cpp" /Fe: %folderName/%outFileName
ECHO Done compiling

ECHO Running %dayNumber
set inFileName="%dayNumber.in"
./%folderName/%outFileName ./%folderName/%inFileName

ECHO Done
