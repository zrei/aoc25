#include <iostream>
#include <vector>
#include "../fileRead.h"

const unsigned int NUM_SHAPES = 6;
const char SHAPE = '#';

long long Part1(const std::vector<std::string>& outputLines);
long long Part2(const std::vector<std::string>& outputLines);
void ParseArea(const std::string& line, unsigned int& area, std::vector<unsigned int>& numShapes);

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
    std::vector<unsigned int> shapeSizes{};
    shapeSizes.reserve(NUM_SHAPES);

    size_t linePtr = 1;
    for (size_t i = 0; i < NUM_SHAPES; i++)
    {
        unsigned int shapeSize = 0;
        for (size_t j = 0; j < 3; j++)
        {
            for (char c : outputLines[linePtr + j])
            {
                if (c == SHAPE)
                    shapeSize++;
            }
        }
        shapeSizes.push_back(shapeSize);
        linePtr += 5;
    }

    for (unsigned int shapeSize : shapeSizes)
    {
        std::cout << "Shape has size: " << shapeSize << std::endl;
    }

    long long numRegions = 0;

    for (size_t i = linePtr - 1; i < outputLines.size(); i++)
    {
        unsigned int area = 0;
        std::vector<unsigned int> numShapes{};
        ParseArea(outputLines[i], area, numShapes);

        std::cout << "Area is: " << area << std::endl;

        //for (size_t i = 0; i < NUM_SHAPES; i++)
        //{
        //    std::cout << "Requires " << numShapes[i] << " of shape " << i << std::endl;
        //}

        unsigned int areaOfShapesRequired = 0;
        for (size_t i = 0; i < NUM_SHAPES; i++)
        {
            areaOfShapesRequired += numShapes[i] * shapeSizes[i];
        }

        if (areaOfShapesRequired < area)
            numRegions++;

        std::cout << "Required area shapes: " << areaOfShapesRequired << std::endl;
    }

    return numRegions;
}

long long Part2(const std::vector<std::string>& outputLines)
{
    return 0;
}

void ParseArea(const std::string& line, unsigned int& area, std::vector<unsigned int>& numShapes)
{
    size_t crossIdx = line.find('x');
    size_t colonIdx = line.find(':');

    area = std::stoi(line.substr(0, crossIdx)) * std::stoi(line.substr(crossIdx + 1, colonIdx - (crossIdx + 1)));

    size_t firstIndex = colonIdx + 2;
    size_t spaceIndex = line.find(' ', firstIndex);

    while (spaceIndex != std::string::npos)
    {
        unsigned int numShape = std::stoi(line.substr(firstIndex, spaceIndex - firstIndex));
        numShapes.push_back(numShape);
        firstIndex = spaceIndex + 1;
        spaceIndex = line.find(' ', firstIndex);
    }

    unsigned int numShape = std::stoi(line.substr(firstIndex, line.size() - firstIndex));
    numShapes.push_back(numShape);
}