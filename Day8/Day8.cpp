#include <iostream>
#include <vector>
#include <unordered_map>
#include <stack>
#include "../fileRead.h"

const char VECTOR_SEPARATOR = ',';
struct Vector3
{
    long long x;
    long long y;
    long long z;

    Vector3(long long x, long long y, long long z) : x(x), y(y), z(z) {}
};

struct DistancePair
{
    size_t firstNode;
    size_t secondNode;
    long long distance;

    DistancePair(size_t firstNode, size_t secondNode, long long distance) : firstNode(firstNode), secondNode(secondNode), distance(distance) {}
};

const int NUM_CONNECTIONS = 1000;

long long Part1(const std::vector<std::string>& outputLines);
long long Part2(const std::vector<std::string>& outputLines);
bool AreConnected(size_t firstNode, size_t secondNode, const std::vector<std::vector<size_t>>& adjacencyGraph);
void CreateDistancePairs(const std::vector<Vector3>& coordinates, std::vector<DistancePair>& distancePairs);
long long CalculateDistance(Vector3 firstPoint, Vector3 secondPoint);
Vector3 ParseLine(const std::string& line);
long long DFS(size_t node, const std::vector<std::vector<size_t>>& adjacencyGraph, std::unordered_map<size_t, char>& experiencedNodes);

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
    std::vector<Vector3> coordinates{};
    coordinates.reserve(outputLines.size());

    for (const std::string& s : outputLines)
    {
        coordinates.push_back(ParseLine(s));
    }

    std::vector<DistancePair> distancePairs{};
    CreateDistancePairs(coordinates, distancePairs);

    std::sort(distancePairs.begin(), distancePairs.end(), [](const DistancePair& first, const DistancePair& second) { return first.distance < second.distance; });

    std::vector<std::vector<size_t>> adjacencyGraph{};
    adjacencyGraph.reserve(coordinates.size());

    for (size_t i = 0; i < coordinates.size(); i++)
    {
        adjacencyGraph.push_back({});
    }

    for (size_t i = 0; i < NUM_CONNECTIONS; i++)
    {
        const DistancePair& distancePair = distancePairs[i];

        if (AreConnected(distancePair.firstNode, distancePair.secondNode, adjacencyGraph))
        {
            continue;
        }
    
        adjacencyGraph[distancePair.firstNode].push_back(distancePair.secondNode);
        adjacencyGraph[distancePair.secondNode].push_back(distancePair.firstNode);
    }

    std::array<long long, 3> longestCircuits{0, 0, 0};

    std::unordered_map<size_t, char> experiencedNodes{};

    for (size_t i = 0; i < adjacencyGraph.size(); i++)
    {
        if (experiencedNodes.find(i) != experiencedNodes.end())
            continue;
        
        long long circuitSize = DFS(i, adjacencyGraph, experiencedNodes);

        if (circuitSize > longestCircuits[0])
        {
            longestCircuits[2] = longestCircuits[1];
            longestCircuits[1] = longestCircuits[0];
            longestCircuits[0] = circuitSize;
        }
        else if (circuitSize > longestCircuits[1])
        {
            longestCircuits[2] = longestCircuits[1];
            longestCircuits[1] = circuitSize;
        }
        else if (circuitSize > longestCircuits[2])
        {
            longestCircuits[2] = circuitSize;
        }
    }

    return longestCircuits[0] * longestCircuits[1] * longestCircuits[2];
}

long long Part2(const std::vector<std::string>& outputLines)
{
    std::vector<Vector3> coordinates{};
    coordinates.reserve(outputLines.size());

    for (const std::string& s : outputLines)
    {
        coordinates.push_back(ParseLine(s));
    }

    std::vector<DistancePair> distancePairs{};
    CreateDistancePairs(coordinates, distancePairs);

    std::sort(distancePairs.begin(), distancePairs.end(), [](const DistancePair& first, const DistancePair& second) { return first.distance < second.distance; });

    std::vector<std::vector<size_t>> adjacencyGraph{};
    adjacencyGraph.reserve(coordinates.size());

    for (size_t i = 0; i < coordinates.size(); i++)
    {
        adjacencyGraph.push_back({});
    }

    int numConnectionsMade = 0;

    for (const DistancePair& distancePair : distancePairs)
    {
        if (AreConnected(distancePair.firstNode, distancePair.secondNode, adjacencyGraph))
        {
            continue;
        }
    
        adjacencyGraph[distancePair.firstNode].push_back(distancePair.secondNode);
        adjacencyGraph[distancePair.secondNode].push_back(distancePair.firstNode);

        numConnectionsMade++;

        if (numConnectionsMade == coordinates.size() - 1)
            return coordinates[distancePair.firstNode].x * coordinates[distancePair.secondNode].x;
    }

    return 0;
}

bool AreConnected(size_t firstNode, size_t secondNode, const std::vector<std::vector<size_t>>& adjacencyGraph)
{
    std::unordered_map<size_t, char> experiencedNodes{};
    std::stack<size_t> ptrs{};
    ptrs.push(firstNode);

    while (!ptrs.empty())
    {
        size_t currPtr = ptrs.top();
        ptrs.pop();
        if (experiencedNodes.find(currPtr) != experiencedNodes.end())
            continue;

        experiencedNodes[currPtr] = '0';
        if (currPtr == secondNode)
            return true;

        for (size_t nextPtr : adjacencyGraph[currPtr])
            ptrs.push(nextPtr);
    }

    return false;
}

void CreateDistancePairs(const std::vector<Vector3>& coordinates, std::vector<DistancePair>& distancePairs)
{
    for (size_t i = 0; i < coordinates.size(); i++)
    {
        for (size_t j = i + 1; j < coordinates.size(); j++)
        {
            distancePairs.emplace_back(i, j, CalculateDistance(coordinates[i], coordinates[j]));
        }
    }
}

long long CalculateDistance(Vector3 firstPoint, Vector3 secondPoint)
{
    return (firstPoint.x - secondPoint.x) * (firstPoint.x - secondPoint.x) + (firstPoint.y - secondPoint.y) * (firstPoint.y - secondPoint.y) + (firstPoint.z - secondPoint.z) * (firstPoint.z - secondPoint.z);
}

Vector3 ParseLine(const std::string& line)
{
    size_t firstDivider = line.find(VECTOR_SEPARATOR, 0);
    int xComponent = std::stoll(line.substr(0, firstDivider));

    size_t secondDivider = line.find(VECTOR_SEPARATOR, firstDivider + 1);
    int yComponent = std::stoll(line.substr(firstDivider + 1, secondDivider - (firstDivider + 1)));

    int zComponent = std::stoll(line.substr(secondDivider + 1, line.size() - (secondDivider + 1)));

    return {xComponent, yComponent, zComponent};
}

long long DFS(size_t node, const std::vector<std::vector<size_t>>& adjacencyGraph, std::unordered_map<size_t, char>& experiencedNodes)
{
    std::stack<size_t> ptrs{};
    ptrs.push(node);
    long long circuitSize = 0;

    while (!ptrs.empty())
    {
        size_t currPtr = ptrs.top();
        ptrs.pop();
        if (experiencedNodes.find(currPtr) != experiencedNodes.end())
            continue;

        //std::cout << currPtr << ", ";

        circuitSize++;
        experiencedNodes[currPtr] = '0';

        for (size_t nextPtr : adjacencyGraph[currPtr])
            ptrs.push(nextPtr);
    }

    //std::cout << std::endl;
    //std::cout << "Circuit size: " << circuitSize << std::endl;
    return circuitSize;
}