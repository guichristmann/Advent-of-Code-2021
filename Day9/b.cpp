#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <queue>
#include <utility>
#include <algorithm>

std::vector<std::vector<uint16_t>> readInputHeightmaps(std::ifstream& is)
{
    std::string line;
    std::vector<std::vector<uint16_t>> heightmaps;
    
    while ( std::getline(is, line) )
    {
        std::vector<uint16_t> row;
        for ( char c : line )
            row.push_back(c - '0');

        heightmaps.push_back(row);
    }

    return heightmaps;
}

bool checkIsLowestPoint(const std::vector<std::vector<uint16_t>>& heightmaps, const int32_t row, 
        const int32_t col, const int32_t totalRows, const int32_t totalCols)
{
    uint16_t centerValue = heightmaps.at(row).at(col);

    if ( row != 0 )
        if ( heightmaps.at(row - 1).at(col) <= centerValue )
            return false;
    if ( row != totalRows - 1 ) 
        if ( heightmaps.at(row + 1).at(col) <= centerValue )
            return false;
    if ( col != 0 )
        if ( heightmaps.at(row).at(col - 1) <= centerValue )
            return false;
    if ( col != totalCols - 1 )
        if ( heightmaps.at(row).at(col + 1) <= centerValue )
            return false;

    return true;
}

uint32_t countBasinSize(const std::vector<std::vector<uint16_t>>& heightmaps, uint32_t r, uint32_t c, 
        const uint32_t totalRows, const uint32_t totalCols, std::vector<std::pair<uint32_t, uint32_t>>& visited)
{
    if ( r == -1 || r == totalRows )
        return 0;
    else if ( c == -1 || c == totalCols )
        return 0;
    else if ( heightmaps.at(r).at(c) == 9 )
        return 0;
    auto currPair = std::make_pair(r, c);
    auto itr = std::find(visited.begin(), visited.end(), currPair); 
    if ( itr != visited.end() )
        return 0;

    visited.push_back(currPair);

    return 1 + countBasinSize(heightmaps, r + 1, c, totalRows, totalCols, visited) +
               countBasinSize(heightmaps, r - 1, c, totalRows, totalCols, visited) +
               countBasinSize(heightmaps, r, c + 1, totalRows, totalCols, visited) +
               countBasinSize(heightmaps, r, c - 1, totalRows, totalCols, visited);
               
}

void checkAndInsertInThreeLargest(std::vector<uint32_t>& v, uint32_t val)
{
    if (v.size() < 3)
    {
        v.push_back(val);
        std::sort(v.begin(), v.end());
        return;
    }

    for ( auto e : v )
        std::cout << e << ", ";
    std::cout << " | " << val << std::endl;

    uint32_t i = 0;
    for ( ; i < v.size(); i++ )
    {
        if ( val < v.at(i) ) 
            break;
    }

    if ( i == 0 )
        return;

    v.insert(v.begin() + i, val);
    v.erase(v.begin());

    std::cin.ignore();
}

int64_t computeThreeLargestBasinMulti(const std::vector<std::vector<uint16_t>>& heightmaps)
{
    uint32_t rows, cols;
    rows = heightmaps.size();
    cols = heightmaps.at(0).size();

    std::vector<uint32_t> threeLargestBasin;

    for ( uint32_t r = 0; r < rows; r++ ) 
    {
        for ( uint32_t c = 0; c < cols; c++ ) 
        {
            if ( checkIsLowestPoint(heightmaps, r, c, rows, cols) )
            {
                std::vector<std::pair<uint32_t, uint32_t>> visited;
                uint32_t basinSize = countBasinSize(heightmaps, r, c, rows, cols, visited);
                checkAndInsertInThreeLargest(threeLargestBasin, basinSize);
            }
        }
    }

    return threeLargestBasin.at(0) * threeLargestBasin.at(1) * threeLargestBasin.at(2);
}

int main(int argc, char* argv[])
{
    std::ifstream is;

    is.open(argv[1]);
    if ( !is.is_open() )
    {
        std::cerr << "Failed to open input file!" << std::endl;
        return 1;
    }

    std::vector<std::vector<uint16_t>> heightmaps = readInputHeightmaps(is);

    std::cout << computeThreeLargestBasinMulti(heightmaps) << std::endl;

    return 0;
}
