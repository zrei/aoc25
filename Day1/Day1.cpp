#include <iostream>
#include <vector>
#include "../fileRead.h"

enum Direction : char
{
    LEFT = 'L',
    RIGHT ='R'
};

long long Part1(const std::vector<std::string>& outputLines);
long long Part2(const std::vector<std::string>& outputLines);
std::pair<Direction, long long> ParseLine(const std::string& line, bool moduloMoveAmount=true);
int GetFinalValue(Direction direction, long long moveAmount, int startingNumber);
std::pair<long long, int> GetNumZeroPassedAndFinalNumber(Direction direction, long long moveAmount, int startingNumber);

const int NUMBER_AMOUNT = 100;
const int STARTING_NUMBER = 50;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Did not provide file name!\n";
        return 0;
    }

    std::vector<std::string> outputLines;
    ReadFromFile(argv[1], outputLines);

    long long part1Answer = Part1(outputLines);
    std::cout << "Part 1 Answer: " << part1Answer << std::endl;

    long long part2Answer = Part2(outputLines);
    std::cout << "Part 2 Answer: " << part2Answer << std::endl;
}

long long Part1(const std::vector<std::string>& outputLines)
{
    int currNumber = STARTING_NUMBER;

    long long zeroCount = 0;
    for (const std::string& line : outputLines)
    {
        std::pair<Direction, long long> values = ParseLine(line);
        currNumber = GetFinalValue(values.first, values.second, currNumber);

        if (currNumber == 0)
            zeroCount++;
    }

    return zeroCount;
}

long long Part2(const std::vector<std::string>& outputLines)
{
    int currNumber = STARTING_NUMBER;

    long long zeroCount = 0;
    for (const std::string& line : outputLines)
    {
        std::pair<Direction, long long> values = ParseLine(line, false);
        std::pair<long long, int> returnValue = GetNumZeroPassedAndFinalNumber(values.first, values.second, currNumber);
        
        currNumber = returnValue.second;
        zeroCount += returnValue.first;
        //std::cout << "Curr number: " << currNumber << ", zeroes in this rotation: " << returnValue.first << std::endl;
    }

    return zeroCount;
}

std::pair<Direction, long long> ParseLine(const std::string& line, bool moduloMoveAmount)
{
    Direction direction;

    if (line[0] == LEFT)
        direction = Direction::LEFT;
    else if (line[0] == RIGHT)
        direction = Direction::RIGHT;
    
    long long turnValue = std::stoll(line.substr(1));

    if (moduloMoveAmount)
        turnValue %= NUMBER_AMOUNT;

    return {direction, turnValue};
}

int GetFinalValue(Direction direction, long long moveAmount, int startingNumber)
{
    switch (direction)
    {
        case Direction::LEFT:
            startingNumber -= moveAmount;
            break;
        case Direction::RIGHT:
            startingNumber += moveAmount;
            break;
    }

    if (startingNumber >= 0)
        return startingNumber % NUMBER_AMOUNT;
    else
        return NUMBER_AMOUNT + startingNumber;
}

std::pair<long long, int> GetNumZeroPassedAndFinalNumber(Direction direction, long long moveAmount, int startingNumber)
{
    if (moveAmount <= 0)
        return {0, startingNumber};

    long long numCompleteCycles = moveAmount / NUMBER_AMOUNT;
    long long moveAmountModuloed = moveAmount % NUMBER_AMOUNT;

    int finalNumber;
    switch (direction)
    {
        case Direction::LEFT:
            finalNumber = startingNumber - moveAmountModuloed;
            break;
        case Direction::RIGHT:
            finalNumber = startingNumber + moveAmountModuloed;
            break;
    }

    long long numZeros = 0;

    if ((finalNumber >= NUMBER_AMOUNT || finalNumber <= 0) && startingNumber != 0)
        numZeros++;
    numZeros += numCompleteCycles;

    if (finalNumber >= 0)
        finalNumber %= NUMBER_AMOUNT;
    else
        finalNumber += NUMBER_AMOUNT;

    return {numZeros, finalNumber};
}
