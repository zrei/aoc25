#include <iostream>
#include <vector>
#include <unordered_map>
#include "../fileRead.h"

const char SPLITTER = '^';
const char STARTING_POINT = 'S';

long long Part1(const std::vector<std::string>& outputLines);
long long Part2(const std::vector<std::string>& outputLines);
std::pair<size_t, size_t> FindStartingPoint(const std::vector<std::string>& map);
long long GetNumSplits(std::pair<size_t, size_t> startingPos, const std::vector<std::string>& map, std::pair<int, int> dimensions);
bool IsValidCoordinate(std::pair<size_t, size_t> coordinates, std::pair<int, int> dimensions);
bool ShouldSplit(std::pair<size_t, size_t> coordinates, const std::vector<std::string>& map);
void TransferPointers(std::vector<size_t>& currPointers, std::vector<size_t>& nextPointers);
void BreakingTimelines(const std::vector<std::string>& map, std::pair<int, int> dimensions, std::vector<long long>& dp);

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
    std::pair<size_t, size_t> startingPoint = FindStartingPoint(outputLines);
    std::pair<int, int> dimensions{outputLines.size(), outputLines[0].size()};
    long long numSplits = GetNumSplits(startingPoint, outputLines, dimensions);

    return numSplits;
}

long long Part2(const std::vector<std::string>& outputLines)
{
    std::vector<long long> dp{};
    std::pair<size_t, size_t> startingPoint = FindStartingPoint(outputLines);
    std::pair<int, int> dimensions{outputLines.size(), outputLines[0].size()};
    BreakingTimelines(outputLines, dimensions, dp);

    return dp[startingPoint.second] + 1;
}

std::pair<size_t, size_t> FindStartingPoint(const std::vector<std::string>& map)
{
    for (size_t r = 0; r < map.size(); r++)
    {
        for (size_t c = 0; c < map[0].size(); c++)
        {
            if (map[r][c] == STARTING_POINT)
                return {r, c};
        }
    }
    return {0, 0};
}

long long GetNumSplits(std::pair<size_t, size_t> startingPos, const std::vector<std::string>& map, std::pair<int, int> dimensions)
{
    size_t currRow = startingPos.first;
    std::vector<size_t> laserPointers{startingPos.second};
    long long numSplits = 0;

    while (!laserPointers.empty())
    {
        std::vector<size_t> newPointers{};
        size_t nextRow = currRow + 1;
        for (size_t ptr : laserPointers)
        {
            std::pair<size_t, size_t> nextCoordinates{nextRow, ptr};
            if (!IsValidCoordinate(nextCoordinates, dimensions))
                continue;
            if (!ShouldSplit(nextCoordinates, map))
            {
                newPointers.push_back(nextCoordinates.second);
                continue;
            }
            else
            {
                // perform splitting
                numSplits++;
                if (ptr > 0)
                {
                    size_t splitCol = ptr - 1;
                    newPointers.push_back(splitCol);
                }
                if (ptr < dimensions.second - 1)
                {
                    size_t splitCol = ptr + 1;
                    newPointers.push_back(splitCol);
                }       
            }    
        }

        currRow = nextRow;
        TransferPointers(laserPointers, newPointers);
    }

    return numSplits;
}

bool IsValidCoordinate(std::pair<size_t, size_t> coordinates, std::pair<int, int> dimensions)
{
    return coordinates.first >= 0 && coordinates.first < dimensions.first && coordinates.second >= 0 && coordinates.second < dimensions.second;
}

bool ShouldSplit(std::pair<size_t, size_t> coordinates, const std::vector<std::string>& map)
{
    return map[coordinates.first][coordinates.second] == SPLITTER;
}

void TransferPointers(std::vector<size_t>& currPointers, std::vector<size_t>& nextPointers)
{
    std::unordered_map<size_t, char> seenPointers{};
    currPointers.clear();

    for (size_t nextPointer : nextPointers)
    {
        if (seenPointers.find(nextPointer) == seenPointers.end())
        {
            currPointers.push_back(nextPointer);
            seenPointers[nextPointer] = '1';
        }
    }
}

void BreakingTimelines(const std::vector<std::string>& map, std::pair<int, int> dimensions, std::vector<long long>& dp)
{
    if (dp.size() < dimensions.second)
    {
        dp.reserve(dimensions.second);
        while (dp.size() < dimensions.second)
            dp.push_back(0);
    }

    int currRow = dimensions.first - 1;

    while (currRow >= 0)
    {
        std::vector<long long> newDpRow{};

        for (size_t i = 0; i < dimensions.second; i++)
        {
            std::pair<size_t, size_t> currCoordinates{currRow, i};
            if (ShouldSplit(currCoordinates, map))
            {
                long long numSplits = 1;
                
                if (i > 0)
                {
                    numSplits += dp[i - 1];
                }

                if (i < dimensions.second - 1)
                {
                    numSplits += dp[i + 1];
                }

                newDpRow.push_back(numSplits);
            }
            else
            {
                newDpRow.push_back(dp[i]);
            }
        }

        for (size_t i = 0; i < dp.size(); i++)
        {
            dp[i] = newDpRow[i];
        }

        currRow--;
    }
}