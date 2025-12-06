#include <iostream>
#include <vector>
#include "../fileRead.h"

enum Operation : char
{
    MULTIPLICATION = '*',
    ADDITION = '+'
};

const char COLUMN_SEPARATOR = ' ';

unsigned long long Part1(const std::vector<std::string>& outputLines);
unsigned long long Part2(const std::vector<std::string>& outputLines);
void ParseColumnIndexes(const std::string& line, std::vector<size_t>& indices);
void ParseOperators(const std::string& line, std::vector<Operation>& operators);
void ParseNumbersInLine(const std::string& line, const std::vector<size_t>& indices, std::vector<unsigned long long>& numbers);
unsigned long long PerformOperation(unsigned long long number1, unsigned long long number2, Operation operation);
void ParseNumbersInLineForced(const std::string& line, std::vector<unsigned long long>& numbers);
bool AllEmpty(size_t ptr, int numRows, const std::vector<std::string>& lines);
unsigned long long CalculatePart2(int numRows, const std::vector<std::string>& lines, const std::vector<Operation>& operations);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Did not provide file name!\n";
        return 0;
    }

    std::vector<std::string> outputLines;
    ReadFromFile(argv[1], outputLines);

    unsigned long long part1Answer = Part1(outputLines);
    std::cout << "Part 1 Answer: " << part1Answer << std::endl;

    unsigned long long part2Answer = Part2(outputLines);
    std::cout << "Part 2 Answer: " << part2Answer << std::endl;
}

unsigned long long Part1(const std::vector<std::string>& outputLines)
{
    //std::vector<size_t> indices{};
    std::vector<Operation> operations{};

    //ParseColumnIndexes(outputLines.front(), indices);
    ParseOperators(outputLines.back(), operations);

    std::vector<unsigned long long> result{};
    for (size_t i = 0; i < operations.size(); i++)
    {
        switch (operations[i])
        {
            case ADDITION:
                result.push_back(0);
                break;
            case MULTIPLICATION:
                result.push_back(1);
                break;
        }
    }

    for (size_t i = 0; i < outputLines.size() - 1; i++)
    {
        std::vector<unsigned long long> numbers{};
        ParseNumbersInLineForced(outputLines[i], numbers);
        //ParseNumbersInLine(outputLines[i], indices, numbers);
        for (size_t j = 0; j < numbers.size(); j++)
        {
            
            result[j] = PerformOperation(result[j], numbers[j], operations[j]);
            if (j == 1)
            {
                //std::cout << "Current number: " << numbers[j] << std::endl;
                //std::cout << "Applied result: " << result[j] << std::endl;
            }
        }
    }

    unsigned long long grandTotal = 0;
    for (unsigned long long res : result)
    {
        grandTotal += res;
        //std::cout << "Res: " << res << ", grand total: " << grandTotal << std::endl;
    }
    return grandTotal;
}

unsigned long long Part2(const std::vector<std::string>& outputLines)
{
    std::vector<Operation> operations{};

    ParseOperators(outputLines.back(), operations);

    return CalculatePart2(outputLines.size() - 1, outputLines, operations);
}

void ParseColumnIndexes(const std::string& line, std::vector<size_t>& indices)
{
    bool lookForDigit = false;
    size_t ptr = 0;

    while (ptr < line.size())
    {
        if (lookForDigit)
        {
            while (line[ptr] == COLUMN_SEPARATOR && ptr < line.size())
                ptr++;

            if (ptr < line.size())
                indices.push_back(ptr);

            lookForDigit = false;
        }
        else
        {
            while (line[ptr] != COLUMN_SEPARATOR && ptr < line.size())
                ptr++;

            lookForDigit = true;
        }
    }

    // remove the last unnecessary column if required
    if (lookForDigit)
        indices.pop_back();
}

void ParseOperators(const std::string& line, std::vector<Operation>& operators)
{
    for (const char& c : line)
    {
        if (c == Operation::ADDITION)
            operators.push_back(Operation::ADDITION);
        else if (c == Operation::MULTIPLICATION)
            operators.push_back(Operation::MULTIPLICATION);
    }
}

void ParseNumbersInLine(const std::string& line, const std::vector<size_t>& indices, std::vector<unsigned long long>& numbers)
{
    size_t prevIndice = 0;
    for (size_t indice : indices)
    {
        numbers.push_back(std::stoull(line.substr(prevIndice, indice - prevIndice)));
        //std::cout << "Indice: " << indice << ", Starting pos: " << prevIndice << ", length: " << (indice - prevIndice) << std::endl;
        prevIndice = indice + 1;
    }

    numbers.push_back(std::stoull(line.substr(prevIndice, line.size() - prevIndice)));
}

void ParseNumbersInLineForced(const std::string& line, std::vector<unsigned long long>& numbers)
{
    size_t ptr = 0;
    while (ptr < line.size())
    {
        size_t prevIndice = ptr;

        while (line[ptr] != COLUMN_SEPARATOR && ptr < line.size())
        {
            ++ptr;
        }

        if (ptr > prevIndice)
        {
            numbers.push_back(std::stoull(line.substr(prevIndice, ptr - prevIndice)));
        }

        ++ptr;
    }
}

unsigned long long PerformOperation(unsigned long long number1, unsigned long long number2, Operation operation)
{
    switch (operation)
    {
        case ADDITION:
            return number1 + number2;
        case MULTIPLICATION:
            return number1 * number2;
    }
}

unsigned long long CalculatePart2(int numRows, const std::vector<std::string>& lines, const std::vector<Operation>& operations)
{
    size_t ptr = 0;
    int operatorIndex = 0;
    unsigned long long totalSum = 0;
    
    while (ptr < lines[0].size())
    {
        while (AllEmpty(ptr, numRows, lines) && ptr < lines[0].size())
            ptr++;

        unsigned long long res;
        switch (operations[operatorIndex])
        {
            case MULTIPLICATION:
                res = 1;
                break;
            case ADDITION:
                res = 0;
                break;
        }

        while (!AllEmpty(ptr, numRows, lines) && ptr < lines[0].size())
        {
            unsigned long long currNum = 0;

            for (size_t i = 0; i < numRows; i++)
            {
                if (lines[i][ptr] != COLUMN_SEPARATOR)
                {  
                    currNum *= 10;
                    currNum += lines[i][ptr] - '0';
                }
            }

            //std::cout << "Curr num: " << currNum << std::endl;

            res = PerformOperation(res, currNum, operations[operatorIndex]);
            ptr++;
        }

        totalSum += res;
        operatorIndex++;
    }

    return totalSum;
}

bool AllEmpty(size_t ptr, int numRows, const std::vector<std::string>& lines)
{
    for (size_t i = 0; i < numRows; i++)
    {
        if (lines[i][ptr] != COLUMN_SEPARATOR)
            return false;
    }
    return true;
}
