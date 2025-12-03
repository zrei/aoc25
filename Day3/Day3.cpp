#include <iostream>
#include <vector>
#include "../fileRead.h"

const char ZERO_CHAR = '0';
const int NUM_CHARS = 12;

long long Part1(const std::vector<std::string>& outputLines);
long long Part2(const std::vector<std::string>& outputLines);
int MaximumVoltage(const std::string& line);
int GetCharValue(char c);
long long MaximumVoltage2(const std::string& line);
void TransferPointers(std::array<size_t, NUM_CHARS>& ptr_array, size_t pointerToShiftFrom, size_t positionToShiftTo);
bool HasSpaceToShift(size_t pointerToShiftFrom, size_t positionToShiftTo, const std::string& line);
void HandleOverlap(std::array<size_t, NUM_CHARS>& ptr_array, const std::string& line);

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
    int count = 0;
    long long finalSum = 0;

    for (const std::string& line : outputLines)
    {
        count++;
        finalSum += MaximumVoltage(line);
    }

    return finalSum;
}

long long Part2(const std::vector<std::string>& outputLines)
{
    long long finalSum = 0;

    for (const std::string& line : outputLines)
    {
        finalSum += MaximumVoltage2(line);
    }

    return finalSum;
}

int MaximumVoltage(const std::string& line)
{
    size_t firstPtr = 0;
    size_t secondPtr = 1;
    while (line[secondPtr] > line[firstPtr] && secondPtr < line.size() - 1)
    {
        firstPtr = secondPtr;
        secondPtr += 1;
    }
    size_t ptr = secondPtr + 1;
    while (ptr < line.size())
    {
        if (line[ptr] > line[firstPtr] && (ptr < line.size() - 1))
        {
            firstPtr = ptr;
            secondPtr = ptr + 1;
        }
        else if (line[ptr] > line[secondPtr])
        {
            secondPtr = ptr;
        }
        while (line[secondPtr] > line[firstPtr] && secondPtr < line.size() - 1)
        {
            firstPtr = secondPtr;
            secondPtr += 1;
        }
        if (ptr < secondPtr)
            ptr = secondPtr;
        ptr += 1;
    }
    return GetCharValue(line[firstPtr]) * 10 + GetCharValue(line[secondPtr]);
}

int GetCharValue(char c)
{
    return c - ZERO_CHAR;
}

long long MaximumVoltage2(const std::string& line)
{
    std::array<size_t, NUM_CHARS> ptr_array = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

    HandleOverlap(ptr_array, line);

    size_t ptr = ptr_array[NUM_CHARS - 1] + 1;

    while (ptr < line.size())
    {
        for (size_t i = 0; i < NUM_CHARS; ++i)
        {
            if (line[ptr] > line[ptr_array[i]] && HasSpaceToShift(ptr, i, line))
            {
                TransferPointers(ptr_array, ptr, i);
                break;
            }
        }
        
        HandleOverlap(ptr_array, line);

        if (ptr < ptr_array[NUM_CHARS - 1])
            ptr = ptr_array[NUM_CHARS - 1];
        ptr += 1;
    }

    long long finalSum = 0;
    long long power10 = 1;
    for (size_t i = 0; i < NUM_CHARS; ++i)
    {
        finalSum += GetCharValue(line[ptr_array[NUM_CHARS - i - 1]]) * power10;
        power10 *= 10;
    }

    return finalSum;
}

void TransferPointers(std::array<size_t, NUM_CHARS>& ptr_array, size_t pointerToShiftFrom, size_t positionToShiftTo)
{
    ptr_array[positionToShiftTo] = pointerToShiftFrom;
    for (size_t k = positionToShiftTo + 1; k < NUM_CHARS; k++)
    {
        ptr_array[k] = ptr_array[k - 1] + 1;
    }
}

bool HasSpaceToShift(size_t pointerToShiftFrom, size_t positionToShiftTo, const std::string& line)
{
    return pointerToShiftFrom <= line.size() - (NUM_CHARS - positionToShiftTo);
}

void HandleOverlap(std::array<size_t, NUM_CHARS>& ptr_array, const std::string& line)
{
    while (true)
    {
        bool shouldBreak = true;
        for (size_t i = 0; i < NUM_CHARS - 1; i++)
        {
            for (size_t j = i + 1; j < NUM_CHARS; j++)
            {
                if (line[ptr_array[j]] > line[ptr_array[i]] && HasSpaceToShift(ptr_array[j], i, line))
                {
                    TransferPointers(ptr_array, ptr_array[j], i);
                    shouldBreak = false;
                    break;
                }

                if (!shouldBreak)
                    break;
            }

            if (!shouldBreak)
                break;
        }
        if (shouldBreak)
            break;
    }
}
