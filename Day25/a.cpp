#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>

typedef std::vector<std::vector<char>> Seafloor;

void readStartingStateFromInput(std::istream& is, Seafloor& map)
{
    std::string tmpLine;

    while (std::getline(is, tmpLine, '\n'))
    {
        std::vector<char> row;

        for (const char& c : tmpLine)
            row.push_back(c);

        map.push_back(row);
    }
}

void printMap(const Seafloor& map)
{
    for (const std::vector<char>& row : map)
    {
        for (const char& c : row)
        {
            std::cout << c; 
        }

        std::cout << std::endl;
    }
}

uint64_t update(Seafloor& map)
{
    uint64_t moveCount = 0;

    Seafloor newMap = map;

    for (int64_t y = 0; y < map.size(); y++)
    {
        for (int64_t x = 0; x < map.at(0).size(); x++)
        {
            const char c = map.at(y).at(x);
            if (c == '.')
            {
                continue;
            }
            else if (c == '>')
            {
                int64_t x2 = x + 1;
                if (x2 == map.at(0).size())
                    x2 = 0;

                if (map.at(y).at(x2) == '.')
                {
                    newMap.at(y).at(x2) = '>';
                    newMap.at(y).at(x) = '.';
                    moveCount++;
                }
            }
        }
    }

    map = newMap;

    for (int64_t y = 0; y < map.size(); y++)
    {
        for (int64_t x = 0; x < map.at(0).size(); x++)
        {
            const char c = map.at(y).at(x);
            if (c == '.')
            {
                continue;
            }
            else if (c == 'v')
            {
                int64_t y2 = y + 1;
                if (y2 == map.size())
                    y2 = 0;

                if (map.at(y2).at(x) == '.')
                {
                    newMap.at(y2).at(x) = 'v';
                    newMap.at(y).at(x) = '.';
                    moveCount++;
                }
            }
        }
    }

    map = newMap;

    return moveCount;
}

int main(int argc, char* argv[])
{
    std::ifstream is(argv[1]);
    if (!is.is_open())
    {
        std::cerr << "Failed to open file!" << std::endl;
        return 1;
    }

    Seafloor map;

    readStartingStateFromInput(is, map);
    std::cout << "Original Map" << std::endl;
    printMap(map);
    std::cout << std::endl;

    uint64_t steps = 0;
    bool finished = false;
    while (!finished)
    {
        std::cout << "Step " << steps + 1 << std::endl;
        if (update(map) == 0)
            finished = true;
        printMap(map);
        std::cout << "------" << std::endl;
        steps++;
    }
    
    std::cout << "Finished after " << steps << " steps." << std::endl;

    return 0;
}
