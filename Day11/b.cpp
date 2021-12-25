#include <iostream>
#include <fstream>
#include <string>
#include <vector>

std::vector<std::vector<uint32_t>> readInput(std::istream& is)
{
    std::vector<std::vector<uint32_t>> octopuses;
    std::string tmpLine;

    while ( std::getline(is, tmpLine) )
    {
        std::vector<uint32_t> row;
        for ( char c : tmpLine )
        {
            row.push_back(c - '0');     
        }


        octopuses.push_back(row);
    }

    return octopuses;
}

void printOctopuses(const std::vector<std::vector<uint32_t>> octopuses)
{
    for (auto row : octopuses)
    {
        for (auto o : row)
        {
            std::cout << o;
        }
        std::cout << std::endl;
    }

    std::cout << "---------" << std::endl;
}

void recurseStep(std::vector<std::vector<uint32_t>>& octopuses, int row, int col)
{
    if ( row < 0 || col < 0 || row >= octopuses.size() || col >= octopuses.at(0).size() )
        return; // Out of range.

    if ( octopuses.at(row).at(col) == 10 )
        return; // Octopus already flashed.

    octopuses.at(row).at(col)++; 
    if ( octopuses.at(row).at(col) == 10 )
    {
        recurseStep(octopuses, row - 1, col - 1);
        recurseStep(octopuses, row - 1, col);
        recurseStep(octopuses, row - 1, col + 1);
        recurseStep(octopuses, row, col - 1);
        recurseStep(octopuses, row, col);
        recurseStep(octopuses, row, col + 1);
        recurseStep(octopuses, row + 1, col - 1);
        recurseStep(octopuses, row + 1, col);
        recurseStep(octopuses, row + 1, col + 1);
    }
}

int findSyncStep(std::vector<std::vector<uint32_t>>& octopuses)
{
    bool synced = false;
    int step = 0;

    int totalSize = octopuses.size() * octopuses.at(0).size();
    
    while ( !synced )
    {
        for (int r = 0; r < octopuses.size(); r++)
        {
            for (int c = 0; c < octopuses.at(0).size(); c++)
            {
                recurseStep(octopuses, r, c);
            }
        }

        // Clean up 10s -> 0s
        int cleanUpCount = 0;
        for (int r = 0; r < octopuses.size(); r++)
        {
            for (int c = 0; c < octopuses.at(0).size(); c++)
            {
                if ( octopuses.at(r).at(c) == 10 )
                {
                    octopuses.at(r).at(c) = 0;
                    cleanUpCount++;
                }
            }
        }

        if ( cleanUpCount == totalSize )
            synced = true;

        step++;
    }


    return step; 
}


int main(int argc, char* argv[])
{
    std::ifstream is(argv[1]);

    if ( !is.is_open() )
    {
        std::cerr << "Failed to open file!" << std::endl;
        return 1;
    }

    auto octopuses = readInput(is);

    std::cout << findSyncStep(octopuses) << std::endl;


    return 0;
}

