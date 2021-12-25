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

void recurseStep(std::vector<std::vector<uint32_t>>& octopuses, int row, int col, int& outFlashes)
{
    if ( row < 0 || col < 0 || row >= octopuses.size() || col >= octopuses.at(0).size() )
        return; // Out of range.

    if ( octopuses.at(row).at(col) == 10 )
        return; // Octopus already flashed.

    octopuses.at(row).at(col)++; 
    if ( octopuses.at(row).at(col) == 10 )
    {
        outFlashes++;
        recurseStep(octopuses, row - 1, col - 1, outFlashes);
        recurseStep(octopuses, row - 1, col, outFlashes);
        recurseStep(octopuses, row - 1, col + 1, outFlashes);
        recurseStep(octopuses, row, col - 1, outFlashes);
        recurseStep(octopuses, row, col, outFlashes);
        recurseStep(octopuses, row, col + 1, outFlashes);
        recurseStep(octopuses, row + 1, col - 1, outFlashes);
        recurseStep(octopuses, row + 1, col, outFlashes);
        recurseStep(octopuses, row + 1, col + 1, outFlashes);
    }
}

// Performs one step of the simulation. Return the number of flashes in the step.
int step(std::vector<std::vector<uint32_t>>& octopuses)
{
    int numberOfFlashes = 0;

    // Step.
    for (int r = 0; r < octopuses.size(); r++)
    {
        for (int c = 0; c < octopuses.at(0).size(); c++)
        {
            recurseStep(octopuses, r, c, numberOfFlashes);
        }
    }

    // Clean up 10s -> 0s
    for (int r = 0; r < octopuses.size(); r++)
    {
        for (int c = 0; c < octopuses.at(0).size(); c++)
        {
            if ( octopuses.at(r).at(c) == 10 )
                octopuses.at(r).at(c) = 0;
        }
    }

    //printOctopuses(octopuses);

    return numberOfFlashes; 
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

    int totalFlashes = 0;
    for (int i = 0; i < 100; i++)
        totalFlashes += step(octopuses);

    std::cout << totalFlashes << std::endl;

    return 0;
}

