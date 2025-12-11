#include <iostream>
#include <vector>
#include <unordered_map>
#include "../fileRead.h"

struct Vector2
{
    int x;
    int y;

    Vector2(int x, int y) : x(x), y(y) {}

    Vector2() : x(0), y(0) {}

    bool operator==(const Vector2& otherV) const 
    {
        return otherV.x == x && otherV.y == y;
    }
};

template<>
struct std::hash<Vector2>
{
    std::size_t operator()(const Vector2& v) const noexcept
    {
        std::size_t h1 = std::hash<int>{}(v.x);
        std::size_t h2 = std::hash<int>{}(v.y);
        return h1 ^ h2; // or use boost::hash_combine
    }
};
 
inline bool operator<(const Vector2& lhs, const Vector2& rhs)
{
  return lhs.x < rhs.x && lhs.y < rhs.y;
}

struct Vector2Pair
{
    Vector2 v1;
    Vector2 v2;

    Vector2Pair(Vector2 v1, Vector2 v2) : v1(v1), v2(v2) {}
    Vector2Pair() : v1(), v2() {}
};

long long Part1(const std::vector<std::string>& outputLines);
long long Part2(const std::vector<std::string>& outputLines);
void ParseLine(const std::string& line, std::vector<Vector2>& coordinates);
long long GetRectangleArea(Vector2 corner1, Vector2 corner2);
void FillPairs(const std::vector<Vector2>& coordinates, std::vector<Vector2Pair>& sameXPairs, std::vector<Vector2Pair>& sameYPairs);
bool DoesRectangleContainOnlyGreenAndRed(Vector2 coordinate1, Vector2 coordinate2, const std::vector<Vector2Pair>& sameXPairs, const std::vector<Vector2Pair>& sameYPairs);
void FillBorder(Vector2 tile1, Vector2 tile2, std::unordered_map<Vector2, char>& border);

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
    std::vector<Vector2> coordinates{};

    for (const std::string& line : outputLines)
    {
        ParseLine(line, coordinates);
    }

    long long greatestArea = 0;

    for (size_t i = 0; i < coordinates.size(); i++)
    {
        for (size_t j = i + 1; j < coordinates.size(); j++)
        {
            greatestArea = std::max(greatestArea, GetRectangleArea(coordinates[i], coordinates[j]));
        }
    }

    return greatestArea;
}

long long Part2(const std::vector<std::string>& outputLines)
{
    std::vector<Vector2> tiles{};

    for (const std::string& line : outputLines)
    {
        ParseLine(line, tiles);
    }

    std::unordered_map<Vector2, char> border{};

    std::cout << "Num tiles: " << tiles.size() << std::endl;

    int maxX = -1;
    int minX = std::numeric_limits<int>::max();
    int maxY = -1;
    int minY = std::numeric_limits<int>::max();

    for (size_t i = 0; i < tiles.size() - 1; i++)
    {
        Vector2 tile1 = tiles[i];
        Vector2 tile2 = tiles[i + 1];
        border[tile1] = 1;
        border[tile2] = 1;

        FillBorder(tile1, tile2, border);
        maxX = std::max(tile2.x, std::max(tile1.x, maxX));
        minX = std::min(tile2.x, std::min(tile1.x, minX));
        maxY = std::max(tile2.y, std::max(tile1.y, maxY));
        minY = std::min(tile2.y, std::min(tile1.y, minY));
    }

    FillBorder(tiles.back(), tiles.front(), border);

    std::cout << border.size() << std::endl;

    long long maxSize = 0;

    for (size_t i = 0; i < tiles.size(); i++)
    {   
        for (size_t j = i + 1; j < tiles.size(); j++) {
            std::cout << "Checking " << i << ", " << j << std::endl;
            
            maxX = -1;
            minX = std::numeric_limits<int>::max();
            maxY = -1;
            minY = std::numeric_limits<int>::max();

            Vector2 tile1 = tiles[i];
            Vector2 tile2 = tiles[j];

            maxX = std::max(tile2.x, std::max(tile1.x, maxX));
            minX = std::min(tile2.x, std::min(tile1.x, minX));
            maxY = std::max(tile2.y, std::max(tile1.y, maxY));
            minY = std::min(tile2.y, std::min(tile1.y, minY));

            std::vector<Vector2> containedPoints{};
  
            for (std::unordered_map<Vector2, char>::iterator itr = border.begin(); itr != border.end(); itr++) 
            {
                Vector2 point = itr->first;
                if (minX < point.x && point.x < maxX && minY < point.y && point.y < maxY)
                {
                    containedPoints.emplace_back(point.x, point.y);
                    break;
                }
            }                    

            if (containedPoints.size() == 0)
            {
                maxSize = std::max(maxSize, GetRectangleArea(tile1, tile2));
            }
        }
    }
    
    return maxSize;
            

    /*
    std::vector<Vector2Pair> sameXPairs{};
    std::vector<Vector2Pair> sameYPairs{};

    FillPairs(coordinates, sameXPairs, sameYPairs);

    std::vector<Vector2Pair> condensedSameXPairs{};

    for (size_t i = 0; i < sameXPairs.size(); i += 2)
    {
        condensedSameXPairs.push_back(sameXPairs[i]);
    }

    std::vector<Vector2Pair> condensedSameYPairs{};

    for (size_t i = 0; i < sameYPairs.size(); i += 2)
    {
        condensedSameYPairs.push_back(sameYPairs[i]);
    }

    /*
    for (const Vector2Pair& vPair : condensedSameXPairs)
    {
        std::cout << "(" << vPair.v1.x << ", " << vPair.v1.y << "), (" << vPair.v2.x << ", " << vPair.v2.y << ")" << std::endl;
    }

    std::cout << std::endl;

    for (const Vector2Pair& vPair : condensedSameYPairs)
    {
        std::cout << "(" << vPair.v1.x << ", " << vPair.v1.y << "), (" << vPair.v2.x << ", " << vPair.v2.y << ")" << std::endl;
    }

    std::cout << std::endl;
    */

    /*
    long long greatestArea = 0;

    for (size_t i = 0; i < coordinates.size(); i++)
    {
        for (size_t j = i + 1; j < coordinates.size(); j++)
        {
            if (DoesRectangleContainOnlyGreenAndRed(coordinates[i], coordinates[j], condensedSameXPairs, condensedSameYPairs))
            {
                //std::cout << "(" << coordinates[i].x << ", " << coordinates[i].y << "), (" << coordinates[j].x << ", " << coordinates[j].y << ")" << std::endl;
                greatestArea = std::max(greatestArea, GetRectangleArea(coordinates[i], coordinates[j]));
            }            
        }
    }

    return greatestArea;
    */
}

void ParseLine(const std::string& line, std::vector<Vector2>& coordinates)
{
    size_t ptr = line.find(',', 0);
    coordinates.emplace_back(std::stoi(line.substr(0, ptr)), std::stoi(line.substr(ptr + 1, line.size() - (ptr + 1))));
}

long long GetRectangleArea(Vector2 corner1, Vector2 corner2)
{
    long long width = std::abs(corner1.x - corner2.x + 1);
    long long height = std::abs(corner1.y - corner2.y + 1);
    return width * height;
}

void FillPairs(const std::vector<Vector2>& coordinates, std::vector<Vector2Pair>& sameXPairs, std::vector<Vector2Pair>& sameYPairs)
{
    for (int i = 0; i < coordinates.size(); i++)
    {
        int prevCoord = i - 1;
        if (prevCoord < 0)
        {
            prevCoord = coordinates.size() + prevCoord;
        }

        if (coordinates[i].x == coordinates[prevCoord].x)
        {
            if (coordinates[i].y < coordinates[prevCoord].y)
            {
                sameXPairs.emplace_back(coordinates[i], coordinates[prevCoord]);
            }
            else
            {
                sameXPairs.emplace_back(coordinates[prevCoord], coordinates[i]);
            }
        }
        else
        {
            if (coordinates[i].x < coordinates[prevCoord].x)
            {
                sameYPairs.emplace_back(coordinates[i], coordinates[prevCoord]);
            }
            else
            {
                sameYPairs.emplace_back(coordinates[prevCoord], coordinates[i]);
            }
        }

        int nextCoord = i + 1;
        if (nextCoord >= coordinates.size())
        {
            nextCoord = 0;
        }

        if (coordinates[i].x == coordinates[nextCoord].x)
        {
            if (coordinates[i].y < coordinates[nextCoord].y)
            {
                sameXPairs.emplace_back(coordinates[i], coordinates[nextCoord]);
            }
            else
            {
                sameXPairs.emplace_back(coordinates[nextCoord], coordinates[i]);
            }
        }
        else
        {
            if (coordinates[i].x < coordinates[nextCoord].x)
            {
                sameYPairs.emplace_back(coordinates[i], coordinates[nextCoord]);
            }
            else
            {
                sameYPairs.emplace_back(coordinates[nextCoord], coordinates[i]);
            }
        }
    }

    std::sort(sameXPairs.begin(), sameXPairs.end(), [](const Vector2Pair& v1, const Vector2Pair& v2) {return v1.v1.x < v2.v1.x;});
    std::sort(sameYPairs.begin(), sameYPairs.end(), [](const Vector2Pair& v1, const Vector2Pair& v2) {return v1.v1.y < v2.v1.y;});
}

bool DoesRectangleContainOnlyGreenAndRed(Vector2 coordinate1, Vector2 coordinate2, const std::vector<Vector2Pair>& sameXPairs, const std::vector<Vector2Pair>& sameYPairs)
{
    // find the top first
    int minX = std::min(coordinate1.x, coordinate2.x);
    int maxX = std::max(coordinate1.x, coordinate2.x);
    int minY = std::min(coordinate1.y, coordinate2.y);
    int maxY = std::max(coordinate1.y, coordinate2.y);

    if (minX == maxX || minY == maxY)
        return true;

    //std::cout << "Checking: (" << minX << ", " << minY << "), (" << maxX << ", " << maxY << ")" << std::endl;
    
    // check upper range
    for (size_t x = minX + 1; x < maxX; x++)
    {
        bool isInPolygon = false;

        for (const Vector2Pair& vPair : sameYPairs)
        {
            //std::cout << "Checking against: (" << vPair.v1.x << ", " << vPair.v1.y << "), (" << vPair.v2.x << ", " << vPair.v2.y << ")" << std::endl;
            if (minY < vPair.v1.y)
            {
                //std::cout << "Early break" << std::endl;
                break;
            }
            else
            {
                if (x >= vPair.v1.x && x <= vPair.v2.x)
                {
                    if (minY == vPair.v1.y)
                    {
                        isInPolygon = true;
                        break;
                    }
                    isInPolygon = !isInPolygon;
                }
            }
        }

        if (!isInPolygon)
            return false;
    }

    //std::cout << "Completed upper check" << std::endl;

    // check lower range
    for (size_t x = minX + 1; x < maxX; x++)
    {
        bool isInPolygon = false;

        for (const Vector2Pair& vPair : sameYPairs)
        {
            //std::cout << "Checking against: (" << vPair.v1.x << ", " << vPair.v1.y << "), (" << vPair.v2.x << ", " << vPair.v2.y << ")" << std::endl;
            if (maxY < vPair.v1.y)
            {
                //std::cout << "Early break" << std::endl;
                break;
            }
            else
            {
                if (x >= vPair.v1.x && x <= vPair.v2.x)
                {
                    if (maxY == vPair.v1.y)
                    {
                        isInPolygon = true;
                        break;
                    }
                    isInPolygon = !isInPolygon;
                }
            }
        }

        if (!isInPolygon)
            return false;
    }

    //std::cout << "Completed lower check" << std::endl;

    // check left range
    for (size_t y = minY + 1; y < maxY; y++)
    {
        bool isInPolygon = false;

        for (const Vector2Pair& vPair : sameXPairs)
        {
            //std::cout << "Checking against: (" << vPair.v1.x << ", " << vPair.v1.y << "), (" << vPair.v2.x << ", " << vPair.v2.y << ")" << std::endl;
            if (minX < vPair.v1.x)
            {
                //std::cout << "Early break" << std::endl;
                break;
            }
            else
            {
                if (y >= vPair.v1.y && y <= vPair.v2.y)
                {
                    if (minX == vPair.v1.x)
                    {
                        isInPolygon = true;
                        break;
                    }
                    isInPolygon = !isInPolygon;
                }
            }
        }

        if (!isInPolygon)
            return false;
    }

    //std::cout << "Completed left check" << std::endl;

    // check right range
    for (size_t y = minY + 1; y < maxY; y++)
    {
        bool isInPolygon = false;

        for (const Vector2Pair& vPair : sameXPairs)
        {
            //std::cout << "Checking against: (" << vPair.v1.x << ", " << vPair.v1.y << "), (" << vPair.v2.x << ", " << vPair.v2.y << ")" << std::endl;
            if (maxX < vPair.v1.x)
            {
                //std::cout << "Early break" << std::endl;
                break;
            }
            else
            {
                if (y >= vPair.v1.y && y <= vPair.v2.y)
                {
                    if (maxX == vPair.v1.x)
                    {
                        isInPolygon = true;
                        break;
                    }
                    isInPolygon = !isInPolygon;
                }
            }
        }

        if (!isInPolygon)
            return false;
    }

    //std::cout << "Completed right check" << std::endl;

    return true;
    /*

    std::cout << "Checking pairs: " << "(" << minX << ", " << minY << "), (" << maxX << ", " << maxY << ")" << std::endl;
    bool toReturn = false;

    size_t minXCheck = 0;
    size_t maxXCheck = 0;

    for (size_t i = 0; i < sameXPairs.size(); i++)
    {
        const Vector2Pair& vPair = sameXPairs[i];
        int xToCheckFor = maxX;
        if (sameXPairs[i].v1.x >= xToCheckFor)
        {
            if (minY > sameXPairs[i].v2.y || maxY < sameXPairs[i].v1.y)
            {
                continue;
            }
            else if (minY == sameXPairs[i].v2.y || maxY == sameXPairs[i].v1.y)
            {
                toReturn = false;
                break;
            }
            else
            {
                std::cout << "X lower pair: " << "(" << sameXPairs[i].v1.x << ", " << sameXPairs[i].v1.y << "), (" << sameXPairs[i].v2.x << ", " << sameXPairs[i].v2.y << ")" << std::endl;
                toReturn = true;
                minXCheck = i;
                break;
            }
        }
    }

    if (!toReturn)
        return false;

    for (int i = sameXPairs.size() - 1; i >= 0; i--)
    {
        const Vector2Pair& vPair = sameXPairs[i];
        int xToCheckFor = minX;
        if (sameXPairs[i].v1.x <= xToCheckFor)
        {
            if (minY > sameXPairs[i].v2.y || maxY < sameXPairs[i].v1.y)
            {
                continue;
            }
            else if (minY == sameXPairs[i].v2.y || maxY == sameXPairs[i].v1.y)
            {
                toReturn = false;
                break;
            }
            else
            {
                std::cout << "X higher pair: " << "(" << sameXPairs[i].v1.x << ", " << sameXPairs[i].v1.y << "), (" << sameXPairs[i].v2.x << ", " << sameXPairs[i].v2.y << ")" << std::endl;
                toReturn = true;
                maxXCheck = i;
                break;
            }
        }
    }

    if (!toReturn)
        return false;

    for (size_t i = minXCheck + 1; i < maxXCheck - 1; i++)
    {
        if ((minY >= sameXPairs[i].v1.y && minY <= sameXPairs[i].v2.y) || (maxY >= sameXPairs[i].v1.y && maxY <= sameXPairs[i].v2.y))
        {
            std::cout << "Return false due to " << "(" << sameXPairs[i].v1.x << ", " << sameXPairs[i].v1.y << "), (" << sameXPairs[i].v2.x << ", " << sameXPairs[i].v2.y << ")" << std::endl;
            return false;
        }
    }

    size_t minYCheck = 0;
    size_t maxYCheck = 0;

    for (size_t i = 0; i < sameYPairs.size(); i++)
    {
        const Vector2Pair& vPair = sameYPairs[i];
        int yToCheckFor = maxY;
        if (sameYPairs[i].v1.y >= yToCheckFor)
        {
            if (minX > sameYPairs[i].v2.x || maxX < sameYPairs[i].v1.x)
            {
                continue;
            }
            else if (minX == sameYPairs[i].v2.x || maxX == sameYPairs[i].v1.x)
            {
                toReturn = false;
                break;
            }
            else
            {
                std::cout << "Y lower pair: " << "(" << sameYPairs[i].v1.x << ", " << sameYPairs[i].v1.y << "), (" << sameYPairs[i].v2.x << ", " << sameYPairs[i].v2.y << ")" << std::endl;
                toReturn = true;
                minYCheck = i;
                break;
            }
        }
    }

    if (!toReturn)
        return false;

    for (int i = sameYPairs.size() - 1; i >= 0; i--)
    {
        const Vector2Pair& vPair = sameYPairs[i];
        int yToCheckFor = minY;
        if (sameYPairs[i].v1.y <= yToCheckFor)
        {
            if (minX > sameYPairs[i].v2.x || maxX < sameYPairs[i].v1.x)
            {
                continue;
            }
            else if (minX == sameYPairs[i].v2.x || maxX == sameYPairs[i].v1.x)
            {
                toReturn = false;
                break;
            }
            else
            {
                std::cout << "Y higher pair: " << "(" << sameYPairs[i].v1.x << ", " << sameYPairs[i].v1.y << "), (" << sameYPairs[i].v2.x << ", " << sameYPairs[i].v2.y << ")" << std::endl;
                toReturn = true;
                maxYCheck = i;
                break;
            }
        }
    }

    if (!toReturn)
        return false;

    for (size_t i = minYCheck + 1; i < maxYCheck - 1; i++)
    {
        if ((minX >= sameYPairs[i].v1.x && minX <= sameYPairs[i].v2.x) || (maxX >= sameYPairs[i].v1.x && maxX <= sameYPairs[i].v2.x))
        {
            std::cout << "Return false due to " << "(" << sameYPairs[i].v1.x << ", " << sameYPairs[i].v1.y << "), (" << sameYPairs[i].v2.x << ", " << sameYPairs[i].v2.y << ")" << std::endl;
            return false;
        }
            
    }

    std::cout << "Correct: " << "(" << minX << ", " << minY << "), (" << maxX << ", " << maxY << ")" << std::endl;
    return true;
    */
}

void FillBorder(Vector2 tile1, Vector2 tile2, std::unordered_map<Vector2, char>& border)
{
    if (tile1.x == tile2.x)
    {
        // same x, collect all y
        int start = std::min(tile1.y, tile2.y);
        int end = std::max(tile1.y, tile2.y) + 1;
        for (int y = start; y < end; y++)
        {
            border[{tile1.x, y}] = 1;
        }
    }
    else
    {
        // same y, collect all x
        int start = std::min(tile1.x, tile2.x);
        int end = std::max(tile1.x, tile2.x) + 1;
        for (int x = start; x < end; x++)
        {
            border[{x, tile1.y}];
        }
    }
}
