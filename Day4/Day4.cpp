#include <iostream>
#include <vector>
#include "../fileRead.h"

const char ROLL = '@';
const char EMPTY_SPACE = '.';
const int MAX_ROLLS_ALLOWED = 3;

long long Part1(const std::vector<std::string>& outputLines);
long long Part2(std::vector<std::string>& outputLines);
bool IsValidRoll(std::pair<int, int> coordinates, const std::vector<std::string>& grid, std::pair<int, int> gridDimension);
bool HasRollAtLocation(std::pair<int, int> coordinates, const std::vector<std::string>& grid, std::pair<int, int> gridDimension);
bool IsValidCoordinates(std::pair<int, int> coordinates, std::pair<int, int> gridDimension);

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
    std::pair<int, int> gridDimension = {outputLines.size(), outputLines[0].size()};

    int numValidRolls = 0;
    for (int i = 0; i < gridDimension.first; i++)
    {
        for (int j = 0; j < gridDimension.second; j++)
        {
            if (IsValidRoll({i, j}, outputLines, gridDimension))
                numValidRolls++;
        }
    }

    return numValidRolls;
}

long long Part2(std::vector<std::string>& outputLines)
{
    std::pair<int, int> gridDimension = {outputLines.size(), outputLines[0].size()};

    int numValidRolls = 0;

    while (true)
    {
        bool hadValidRoll = false;
        for (int i = 0; i < gridDimension.first; i++)
        {
            for (int j = 0; j < gridDimension.second; j++)
            {
                if (IsValidRoll({i, j}, outputLines, gridDimension))
                {
                    numValidRolls++;
                    hadValidRoll = true;
                    outputLines[i][j] = EMPTY_SPACE;
                }
                    
            }
        }

        if (!hadValidRoll)
            break;
    }

    return numValidRolls;
}

bool IsValidRoll(std::pair<int, int> coordinates, const std::vector<std::string>& grid, std::pair<int, int> gridDimension)
{
    if (!HasRollAtLocation(coordinates, grid, gridDimension))
        return false;

    int numAdjacentRolls = 0;

    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
                continue;

            if (HasRollAtLocation({coordinates.first + i, coordinates.second + j}, grid, gridDimension))
                numAdjacentRolls++;
        }
    }

    return numAdjacentRolls <= MAX_ROLLS_ALLOWED;
}

bool HasRollAtLocation(std::pair<int, int> coordinates, const std::vector<std::string>& grid, std::pair<int, int> gridDimension)
{
    if (!IsValidCoordinates(coordinates, gridDimension))
        return false;

    return grid[coordinates.first][coordinates.second] == ROLL;
}

bool IsValidCoordinates(std::pair<int, int> coordinates, std::pair<int, int> gridDimension)
{
    return !(coordinates.first < 0 || coordinates.first >= gridDimension.first || coordinates.second < 0 || coordinates.second >= gridDimension.second);
}