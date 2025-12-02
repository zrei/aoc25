#include <iostream>
#include <vector>
#include "../fileRead.h"

long long Part1(const std::vector<std::string>& outputLines);
long long Part2(const std::vector<std::string>& outputLines);
void ParseRanges(std::vector<std::pair<long long, long long>>& outputs, const std::string& inputLine);
std::pair<long long, long long> ParseRange(const std::string& range);
long long GetSumFromRange(const std::pair<long long, long long>& range);
bool IsInvalidId(const std::string& id);
long long GetSumFromRangePart2(const std::pair<long long, long long>& range);
bool IsInvalidIdPart2(const std::string& id);

const char RANGE_SEPARATOR = ',';
const char NUM_SEPARATOR = '-';

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Did not provide file name!\n";
        return 0;
    }

    std::vector<std::string> outputLines;
    ReadFromFile(argv[1], outputLines);

    long long part1Answer = Part1(outputLines);
    std::cout << "Part 1 Answer: " << Part1(outputLines) << std::endl;

    long long part2Answer = Part2(outputLines);
    std::cout << "Part 2 Answer: " << Part2(outputLines) << std::endl;
}

long long Part1(const std::vector<std::string>& outputLines)
{
    std::vector<std::pair<long long, long long>> ranges;
    ParseRanges(ranges, outputLines[0]);
    
    long long finalSum = 0;
    for (const std::pair<long long, long long>& range : ranges)
    {
        finalSum += GetSumFromRange(range);
    }

    return finalSum;
}

long long Part2(const std::vector<std::string>& outputLines)
{
    std::vector<std::pair<long long, long long>> ranges;
    ParseRanges(ranges, outputLines[0]);
    
    long long finalSum = 0;
    for (const std::pair<long long, long long>& range : ranges)
    {
        finalSum += GetSumFromRangePart2(range);
    }

    return finalSum;
}

void ParseRanges(std::vector<std::pair<long long, long long>>& outputs, const std::string& inputLine)
{
    int startPos = 0;
    int dividerPos = inputLine.find(RANGE_SEPARATOR, startPos);

    while (dividerPos != -1)
    {
        //std::cout << inputLine.substr(startPos, dividerPos - startPos) << std::endl;
        outputs.push_back(ParseRange(inputLine.substr(startPos, dividerPos - startPos)));
        startPos = dividerPos + 1;
        dividerPos = inputLine.find(RANGE_SEPARATOR, startPos);
    }
    outputs.push_back(ParseRange(inputLine.substr(startPos, inputLine.size() - startPos)));
}

std::pair<long long, long long> ParseRange(const std::string& range)
{
    int dividerPos = range.find(NUM_SEPARATOR, 0);
    //std::cout << range.substr(0, dividerPos) << " " << range.substr(dividerPos + 1, range.size() - (dividerPos + 1)) << std::endl;
    return {std::stoll(range.substr(0, dividerPos)), std::stoll(range.substr(dividerPos + 1, range.size() - (dividerPos + 1)))};
}

long long GetSumFromRange(const std::pair<long long, long long>& range)
{
    long long sum = 0;
    for (long long start = range.first; start <= range.second; start++)
    {
        std::string stringRep = std::to_string(start);
        if (IsInvalidId(stringRep))
            sum += start;
    }

    return sum;
}

bool IsInvalidId(const std::string& id)
{
    if (id.size() % 2 == 1)
        return false;

    int ptr1 = 0;
    int ptr2 = id.size() / 2;

    while (ptr2 < id.size())
    {
        if (id[ptr1] != id[ptr2])
            return false;
        ptr1++;
        ptr2++;
    }

    return true;
}

long long GetSumFromRangePart2(const std::pair<long long, long long>& range)
{
    long long sum = 0;
    for (long long start = range.first; start <= range.second; start++)
    {
        std::string stringRep = std::to_string(start);
        if (IsInvalidIdPart2(stringRep))
            sum += start;
    }

    return sum;
}

bool IsInvalidIdPart2(const std::string& id)
{
    //std::cout << id << std::endl;

    int windowSize = 1;

    while (windowSize <= id.size() / 2)
    {
        //std::cout << "Windows size: " << windowSize << std::endl;

        if (id.size() % windowSize != 0)
        {
            windowSize++;
            continue;
        }
        
        std::vector<int> ptrs;
        
        int numWindows = id.size() / windowSize;
        for (size_t i = 0; i < numWindows; ++i)
        {
            ptrs.push_back(i * windowSize);
        }

        bool isInvalid = true;
        
        while (ptrs[ptrs.size() - 1] < id.size())
        {
            char toMatch = id[ptrs[0]];
            ptrs[0]++;
            for (size_t i = 1; i < ptrs.size(); ++i)
            {
                if (id[ptrs[i]] != toMatch)
                {
                    isInvalid = false;
                    break;
                }

                ptrs[i]++;
            }

            if (!isInvalid)
                break;
        }

        if (isInvalid)
        {
            //std::cout << "Invalid id: " << id << std::endl;
            return true;
        }

        windowSize++;
    }

    return false;
}