#include <iostream>
#include <vector>
#include <unordered_map>
#include <stack>
#include <set>
#include <queue>
#include "../fileRead.h"

const size_t KEY_SIZE = 3;
const size_t RANGE_BEGIN = 'a';
const std::string INPUT = "you";
const std::string OUTPUT = "out";
const std::string SERVER = "svr";
const std::string DIGITAL_ANALOG = "dac";
const std::string FOURIER_TRANSFORM = "fft";

long long Part1(const std::vector<std::string>& outputLines);
long long Part2(const std::vector<std::string>& outputLines);
int TranslateStringKeyToIntKey(const std::string& line);
void BuildAdjacencyGraphForLine(const std::string& line, std::unordered_map<int, std::vector<int>>& adjacencyGraph, std::set<int>& experiencedNodes);
long long BFS(std::unordered_map<int, std::vector<int>>& adjacencyGraph, const std::string& input, const std::string& output);
long long BFS2(std::unordered_map<int, std::vector<int>>& adjacencyGraph, int currPtr, int outputPtr, std::unordered_map<int, long long>& memo);

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
    std::unordered_map<int, std::vector<int>> adjacencyGraph{};
    std::set<int> experiencedNodes{};

    for (const std::string& s : outputLines)
    {
        BuildAdjacencyGraphForLine(s, adjacencyGraph, experiencedNodes);
    }

    return BFS(adjacencyGraph, INPUT, OUTPUT);
}

long long Part2(const std::vector<std::string>& outputLines)
{
    // ran BFS between FFT and DAC and vice versa just now, and there's no way between DAC and FFT

    std::unordered_map<int, std::vector<int>> adjacencyGraph{};
    std::set<int> experiencedNodes{};

    for (const std::string& s : outputLines)
    {
        BuildAdjacencyGraphForLine(s, adjacencyGraph, experiencedNodes);
    }

    std::unordered_map<int, long long> memo{};

    long long waysToFFT = BFS2(adjacencyGraph, TranslateStringKeyToIntKey(SERVER), TranslateStringKeyToIntKey(FOURIER_TRANSFORM), memo);

    memo.clear();

    long long waysBetweenFFTandDAC = BFS2(adjacencyGraph, TranslateStringKeyToIntKey(FOURIER_TRANSFORM), TranslateStringKeyToIntKey(DIGITAL_ANALOG), memo);

    memo.clear();

    long long waysBetweenDACandOutput = BFS2(adjacencyGraph, TranslateStringKeyToIntKey(DIGITAL_ANALOG), TranslateStringKeyToIntKey(OUTPUT), memo);

    return waysToFFT * waysBetweenFFTandDAC * waysBetweenDACandOutput;
}

void BuildAdjacencyGraphForLine(const std::string& line, std::unordered_map<int, std::vector<int>>& adjacencyGraph, std::set<int>& experiencedNodes)
{
    int inputNode = TranslateStringKeyToIntKey(line.substr(0, KEY_SIZE));

    adjacencyGraph[inputNode] = {};
    experiencedNodes.emplace(inputNode);

    size_t ptr = KEY_SIZE + 2;

    while (ptr < line.size())
    {
        int outputNode = TranslateStringKeyToIntKey(line.substr(ptr, KEY_SIZE));
        adjacencyGraph[inputNode].push_back(outputNode);
        experiencedNodes.emplace(outputNode);
        
        ptr += KEY_SIZE + 1;
    }
}

int TranslateStringKeyToIntKey(const std::string& line)
{
    int intKey = 0;

    for (char c : line)
    {
        intKey *= 100;
        intKey += c - RANGE_BEGIN;
    }

    return intKey;
}

long long BFS(std::unordered_map<int, std::vector<int>>& adjacencyGraph, const std::string& input, const std::string& output)
{
    std::stack<int> currPtrs{};
    currPtrs.push(TranslateStringKeyToIntKey(input));
    int finalOutputNode = TranslateStringKeyToIntKey(output);

    long long numWays = 0;

    while (!currPtrs.empty())
    {
        int currPtr = currPtrs.top();
        currPtrs.pop();

        if (currPtr == finalOutputNode)
        {
            numWays++;
            continue;
        }

        if (adjacencyGraph.find(currPtr) == adjacencyGraph.end())
        {
            continue;
        }

        for (int outputNode : adjacencyGraph[currPtr])
        {
            currPtrs.push(outputNode);
        }
    }

    return numWays;
}

long long BFS2(std::unordered_map<int, std::vector<int>>& adjacencyGraph, int currPtr, int outputPtr, std::unordered_map<int, long long>& memo)
{
    if (memo.find(currPtr) != memo.end())
    {
        return memo[currPtr];
    }

    if (currPtr == outputPtr)
    {
        return 1;
    }

    if (adjacencyGraph.find(currPtr) == adjacencyGraph.end())
    {
        return 0;
    }

    long long sum = 0;

    for (int node : adjacencyGraph[currPtr])
    {
        sum += BFS2(adjacencyGraph, node, outputPtr, memo);
    }

    memo[currPtr] = sum;

    return sum;
}
