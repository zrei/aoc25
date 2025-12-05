#include <iostream>
#include <vector>
#include "../fileRead.h"

const char DASH_SEPARATOR = '-';

struct FreshnessRange
{
public:
    FreshnessRange(long long rangeStart, long long rangeEnd) : InternalRange(rangeStart, rangeEnd) {}

    long long GetRangeStart() const { return InternalRange.first; }

    long long GetRangeEnd() const { return InternalRange.second; }

    long long GetRangeLength() const { return GetRangeEnd() - GetRangeStart() + 1; }
private:
    std::pair<long long, long long> InternalRange;
};

long long Part1(const std::vector<std::string>& outputLines);
long long Part2(const std::vector<std::string>& outputLines);
void MergeAvailableRanges(std::vector<FreshnessRange>& ranges);
bool DoesRangeOverlap(const FreshnessRange& range1, const FreshnessRange& range2);
bool IsNumberWithinRange(const FreshnessRange& range, long long number);
FreshnessRange MergeRange(const FreshnessRange& range1, const FreshnessRange& range2);
FreshnessRange ParseRange(const std::string& rangeStr);
long long ParseIngredientId(const std::string& ingrdientStr);
bool IsIngredientFresh(long long ingredient, const std::vector<FreshnessRange>& ranges);

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
    bool hasCrossedBlankLine = false;
    std::vector<FreshnessRange> ranges{};
    long long freshIngredientCount = 0;
    for (const std::string& s : outputLines)
    {
        if (s.size() == 0)
        {
            hasCrossedBlankLine = true;
            continue;
        }

        if (hasCrossedBlankLine)
        {
            long long ingredientId = ParseIngredientId(s);
            if (IsIngredientFresh(ingredientId, ranges))
                freshIngredientCount++;
        }
        else
        {
            ranges.push_back(ParseRange(s));
            //std::cout << "Weh" << std::endl;
            MergeAvailableRanges(ranges);
        }
    }

    /*
    for (const FreshnessRange& range : ranges)
    {
        std::cout << "Available range: " << range.GetRangeStart() << "-" << range.GetRangeEnd() << std::endl;
    }
    */

    return freshIngredientCount;
}

long long Part2(const std::vector<std::string>& outputLines)
{
    std::vector<FreshnessRange> ranges{};

    long long allFreshIngredientRanges = 0;

    for (const std::string& s : outputLines)
    {
        if (s.size() == 0)
        {
            break;
        }

        ranges.push_back(ParseRange(s));
        MergeAvailableRanges(ranges);
    }

    for (const FreshnessRange& range : ranges)
    {
        allFreshIngredientRanges += range.GetRangeLength();
    }

    return allFreshIngredientRanges;
}

void MergeAvailableRanges(std::vector<FreshnessRange>& ranges)
{
    while (true)
    {
        bool hasMerge = false;

        size_t ptr = 0;
        while (true)
        {
            std::vector<size_t> pointersToRemove{};
            for (size_t i = ptr + 1; i < ranges.size(); i++)
            {
                if (DoesRangeOverlap(ranges[ptr], ranges[i]))
                {
                    hasMerge = true;
                    ranges[ptr] = MergeRange(ranges[ptr], ranges[i]);
                    pointersToRemove.push_back(i);
                }
            }

            int numRemoved = 0;
            for (size_t ptrToRemove : pointersToRemove)
            {
                ranges.erase(ranges.begin() + ptrToRemove + numRemoved);
                numRemoved++;
            }
            ++ptr;

            if (ptr >= ranges.size())
                break;
        }

        if (!hasMerge)
            break;
    }
}

bool DoesRangeOverlap(const FreshnessRange& range1, const FreshnessRange& range2)
{
    return IsNumberWithinRange(range1, range2.GetRangeStart()) || IsNumberWithinRange(range1, range2.GetRangeEnd()) || IsNumberWithinRange(range2, range1.GetRangeStart()) || IsNumberWithinRange(range2, range1.GetRangeEnd());
}

bool IsNumberWithinRange(const FreshnessRange& range, long long number)
{
    return number >= range.GetRangeStart() && number <= range.GetRangeEnd();
}

FreshnessRange MergeRange(const FreshnessRange& range1, const FreshnessRange& range2)
{
    return {std::min(range1.GetRangeStart(), range2.GetRangeStart()), std::max(range1.GetRangeEnd(), range2.GetRangeEnd())};
}

FreshnessRange ParseRange(const std::string& rangeStr)
{
    size_t dashIndex = rangeStr.find(DASH_SEPARATOR);
    //std::cout << dashIndex << std::endl;
    return {std::stoll(rangeStr.substr(0, dashIndex)), std::stoll(rangeStr.substr(dashIndex + 1, rangeStr.size() - dashIndex - 1))};
}

long long ParseIngredientId(const std::string& ingrdientStr)
{
    return std::stoll(ingrdientStr);
}

bool IsIngredientFresh(long long ingredient, const std::vector<FreshnessRange>& ranges)
{
    for (const FreshnessRange& range : ranges)
    {
        if (IsNumberWithinRange(range, ingredient))
            return true;
    }

    return false;
}