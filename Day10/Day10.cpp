#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
//#include "../fileRead.h"
#include <z3++.h>

using namespace z3;

const char ON_LIGHT = '#';

long long Part1(const std::vector<std::string>& outputLines);
long long Part2(const std::vector<std::string>& outputLines);
unsigned int ParseLights(const std::string& line);
void ParseButtons(const std::string& line, std::vector<unsigned int>& buttons);
void ParseButton(const std::string& buttonText, std::vector<unsigned int>& buttons);
long long GetSmallestPresses(unsigned int desiredFinalResult, int currButtonIndex, unsigned int currResult, const std::vector<unsigned int>& buttonMasks, int currPresses, unsigned int pressedButtons);
void ParseJoltage(const std::string& line, std::vector<int>& joltageReqs);
long long GetSmallestPressesPart2(const std::vector<int>& desiredFinalResult, const std::vector<unsigned int>& buttonMasks);
void ReadFromFile(std::string filePath, std::vector<std::string>& outputLines);

void ReadFromFile(std::string filePath, std::vector<std::string>& outputLines) {
    std::string line;
    std::ifstream rfile;
    std::vector<std::string> lines;
    rfile.open(filePath);
    if (rfile.is_open()) {
        while (std::getline(rfile, line)) {
            outputLines.push_back(std::move(line));
        }
    }
    rfile.close();
}

// part 2 terminal env variables (homebrew is not working for some reason)
// export CPATH=/Applications/Xcode.app/Contents/Developer/Library/Frameworks/Python3.framework/Versions/3.9/include
// export DYLD_LIBRARY_PATH=/Applications/Xcode.app/Contents/Developer/Library/Frameworks/Python3.framework/Versions/3.9/lib:$DYLD_LIBRARY_PATH
// to compile: 
// g++ -c [filename].cpp
// g++ -o [filename] [filename].o -lz3
// ./[filename] input

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
    long long summation = 0;
    
    for (const std::string& line : outputLines)
    {
        unsigned int finalRep = ParseLights(line);
        std::vector<unsigned int> masks{};
        ParseButtons(line, masks);
        long long smallestPresses = GetSmallestPresses(finalRep, 0, 0, masks, 0, 0);
        std::cout << smallestPresses << std::endl;
        summation += smallestPresses;
    }

    return summation;
}

long long Part2(const std::vector<std::string>& outputLines)
{
    long long summation = 0;
    for (const std::string& line : outputLines)
    {
        std::vector<unsigned int> masks{};
        ParseButtons(line, masks);
        std::vector<int> finalButtonResult{};
        ParseJoltage(line, finalButtonResult);

        long long result = GetSmallestPressesPart2(finalButtonResult, masks);
        std::cout << "Result: " << result << std::endl;
        summation += result;
    }

    return summation;
}

unsigned int ParseLights(const std::string& line)
{
    size_t closingBracketIndex = line.find(']');

    int numLights = closingBracketIndex - 1;

    unsigned int lightRep = 0;

    for (int i = 0; i < numLights; i++)
    {
        unsigned int mult = pow(2, i);
        lightRep |= (line[i + 1] == ON_LIGHT ? 1 : 0) * mult;
    }

    return lightRep;
}

void ParseButtons(const std::string& line, std::vector<unsigned int>& buttons)
{
    size_t openingBracketIndex = line.find('(');

    while (true)
    {
        size_t closingBracketIndex = line.find(')', openingBracketIndex);
        ParseButton(line.substr(openingBracketIndex + 1, closingBracketIndex - (openingBracketIndex + 1)), buttons);
        openingBracketIndex = closingBracketIndex + 2;
        if (line[openingBracketIndex] != '(')
            break;
    }
}

void ParseButton(const std::string& buttonText, std::vector<unsigned int>& buttons)
{
    size_t startingIndex = 0;
    size_t commaIndex = buttonText.find(',', startingIndex);

    unsigned int mask = 0;

    while (commaIndex != std::string::npos)
    {
        unsigned int mult = std::pow(2, std::stoi(buttonText.substr(startingIndex, commaIndex - startingIndex)));
        mask |= mult;
        startingIndex = commaIndex + 1;
        commaIndex = buttonText.find(',', startingIndex);
    }

    unsigned int mult = std::pow(2, std::stoi(buttonText.substr(startingIndex, buttonText.size() - startingIndex)));
    mask |= mult;
    buttons.emplace_back(mask);
}

void ParseJoltage(const std::string& line, std::vector<int>& joltageReqs)
{
    size_t beginIndex = line.find('{') + 1;
    size_t joltageEndResult = line.size() - 1;

    size_t commaIndex = line.find(',', beginIndex);

    while (commaIndex != std::string::npos)
    {
        joltageReqs.emplace_back(std::stoi(line.substr(beginIndex, commaIndex - beginIndex)));
        beginIndex = commaIndex + 1;
        commaIndex = line.find(',', beginIndex);
    }

    joltageReqs.emplace_back(std::stoi(line.substr(beginIndex, joltageEndResult - beginIndex)));
}

long long GetSmallestPresses(unsigned int desiredFinalResult, int currButtonIndex, unsigned int currResult, const std::vector<unsigned int>& buttonMasks, int currPresses, unsigned int pressedButtons)
{
    if (desiredFinalResult == currResult)
    {
        std::cout << "Pressed buttons to get result " << desiredFinalResult << ": " << pressedButtons << std::endl;
        return currPresses;
    }
    else if (currButtonIndex >= buttonMasks.size())
        return buttonMasks.size();
    else
    {
        unsigned int mult = std::pow(2, currButtonIndex);
        // press
        long long res1 = GetSmallestPresses(desiredFinalResult, currButtonIndex + 1, currResult ^ buttonMasks[currButtonIndex], buttonMasks, currPresses + 1, pressedButtons | mult);
        // don't press
        long long res2 = GetSmallestPresses(desiredFinalResult, currButtonIndex + 1, currResult, buttonMasks, currPresses, pressedButtons);

        return std::min(res1, res2);
    }
}

long long GetSmallestPressesPart2(const std::vector<int>& desiredFinalResult, const std::vector<unsigned int>& buttonMasks)
{
    context c;
    const unsigned N = 10;

    expr_vector button_vals(c);

    for (size_t i = 0; i < buttonMasks.size(); i++)
    {
        std::stringstream button_name; 
        button_name << "button_" << i;
        button_vals.push_back(c.int_const(button_name.str().c_str()));
    }

    optimize o1(c);

    for (size_t i = 0; i < buttonMasks.size(); i++)
    {
        o1.add(button_vals[i] >= 0);
    }

    for (size_t i = 0; i < desiredFinalResult.size(); i++)
    {
        expr_vector eqns(c);
        int voltageRes = desiredFinalResult[i];

        unsigned int multMask = std::pow(2, i);

        for (size_t j = 0; j < buttonMasks.size(); j++) 
        {
            if (multMask & buttonMasks[j])
            {
                eqns.push_back(button_vals[j]);
            }
        }

        o1.add(sum(eqns) == voltageRes);
    }

    expr buttonPressTotal = c.int_const("button_press_total");
    o1.add(sum(button_vals) == buttonPressTotal);
    o1.minimize(buttonPressTotal);

    //std::cout << o1 << std::endl;
    //std::cout << o1.check() << std::endl;
    o1.check();
    //o1.lower(h1);
    model m = o1.get_model();
    //std::cout << "solution\n" << m << std::endl;
    return m.eval(buttonPressTotal).get_numeral_int64();
    //return 0;
}

// a part 2 description
// number of button presses for each button is in itself a variable
// add them up to form... the thing. i g. uh. yeah.

/*
long long GetSmallestPressesPart2(const std::vector<int>& desiredFinalResult, int currButtonIndex, std::vector<int> currentResult, const std::vector<unsigned int>& buttonMasks, long long currPresses, const int& numButtons)
{
    // perform checks
    bool matches = true;
    bool tooHigh = false;

    for (size_t i = 0; i < desiredFinalResult.size(); i++)
    {
        if (currentResult[i] > desiredFinalResult[i])
        {
            tooHigh = true;
            break;
        }
        else if (currentResult[i] != desiredFinalResult[i])
        {
            matches = false;
            break;
        }
    }

    if (tooHigh)
    {
        std::cout << "What, you?" << std::endl;
        return std::numeric_limits<long long>::max();
    }
    else if (matches)
    {
        return currPresses;
    }

    // don't press, move on to next button
    long long res1 = GetSmallestPressesPart2(desiredFinalResult, currButtonIndex + 1, currentResult, buttonMasks, currPresses, numButtons);
    std::cout << "Stop!" << std::endl;
    // press current button
    std::cout << "Meh" << std::endl;
    for (int i = 0; i < numButtons; i++)
    {
        unsigned int mask = std::pow(2, i);
        if (mask & buttonMasks[currButtonIndex])
        {
            currentResult[i] += 1;
        }
    }
    std::cout << "?" << std::endl;
    long long res2 = GetSmallestPressesPart2(desiredFinalResult, currButtonIndex, currentResult, buttonMasks, currPresses + 1, numButtons);

    return std::min(res1, res2);
}
*/