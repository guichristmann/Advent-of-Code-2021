#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

std::vector<std::vector<uint16_t>> readInputHeightmaps(std::ifstream& is)
{
    std::string line;
    std::vector<std::vector<uint16_t>> heightmaps;
    uint16_t val;
    
    while ( std::getline(is, line) )
    {
        //std::stringstream ss(line);

        std::cout << line << std::endl;

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

int64_t computeRiskFactor(const std::vector<std::vector<uint16_t>>& heightmaps)
{
    int64_t result = 0;

    uint32_t rows, cols;
    rows = heightmaps.size();
    cols = heightmaps.at(0).size();

    for ( uint32_t r = 0; r < rows; r++ ) 
    {
        bool lowestFlag = true;
        for ( uint32_t c = 0; c < cols; c++ ) 
        {
            if ( checkIsLowestPoint(heightmaps, r, c, rows, cols) )
            {
                result += ( heightmaps.at(r).at(c) + 1 );
            }
        }
    }

    return result;
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

    std::cout << computeRiskFactor(heightmaps) << std::endl;

    return 0;
}
