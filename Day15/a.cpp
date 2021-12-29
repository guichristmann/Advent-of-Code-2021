#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <boost/unordered_map.hpp>

typedef std::vector<std::vector<int>> Maze;
typedef std::pair<int, int> Coord;

void readInput(std::istream& is, Maze& outMaze, int& size)
{
    std::string tmpLine;

    size = 0;
    while ( is >> tmpLine )
    {
        std::vector<int> row;
        for ( const char& c : tmpLine )
            row.push_back(c - '0');
        
        outMaze.push_back(row);
        size++;
    }
}

void printMaze(const Maze& maze)
{
    for ( const auto& row : maze )
    {
        for ( const int& risk : row )
        {
            std::cout << risk << " | ";
        }

        std::cout << std::endl;
    }
}

std::vector<Coord> getValidNeighbors(const Coord& current, const std::vector<Coord>& visited, const int mazeSize)
{
    std::vector<Coord> neighbors;

    Coord candidate;
    if ( current.second + 1 < mazeSize )
    {
        candidate = std::make_pair( current.first, current.second + 1 );
        auto idx = std::find(visited.begin(), visited.end(), candidate);
        if ( idx == visited.end() )
            neighbors.push_back(candidate);
    }

    if ( current.second - 1 >= 0 )
    {
        candidate = std::make_pair( current.first, current.second - 1 );
        auto idx = std::find(visited.begin(), visited.end(), candidate);
        if ( idx == visited.end() )
            neighbors.push_back(candidate);
    }

    if ( current.first - 1 >= 0 )
    {
        candidate = std::make_pair( current.first - 1, current.second );
        auto idx = std::find(visited.begin(), visited.end(), candidate);
        if ( idx == visited.end() )
            neighbors.push_back(candidate);
    }

    if ( current.first + 1 < mazeSize )
    {
        candidate = std::make_pair( current.first + 1, current.second );
        auto idx = std::find(visited.begin(), visited.end(), candidate);
        if ( idx == visited.end() )
            neighbors.push_back(candidate);
    }

    return neighbors;
}

Coord findCoordWithSmallestCost(const Maze& costMaze, const std::vector<Coord>& queue, uint32_t& outIndex)
{
    Coord returnCoord = queue.at(0);
    outIndex = 0;
    int smallestCost = costMaze.at(returnCoord.second).at(returnCoord.first);
    for ( uint32_t i = 1; i < queue.size(); i++ )
    {
        Coord curr = queue.at(i);
        if ( costMaze.at(curr.second).at(curr.first) < smallestCost)
        {
            smallestCost = costMaze.at(curr.second).at(curr.first);
            returnCoord = curr;
            outIndex = i;
        }
    }

    return returnCoord;
}

int computeLowestTotalRisk(const Maze& maze, const int mazeSize)
{
    std::vector<Coord> queue { { Coord(0, 0) } };
    std::vector<Coord> visited;

    Maze costMaze(mazeSize, std::vector<int>(mazeSize, INT32_MAX));
    costMaze.at(0).at(0) = 0;

    // Fill queue.
    //for ( int y = 0; y < mazeSize; y++ )
    //{
    //    for ( int x = 0; x < mazeSize; x++ )
    //    {
    //        queue.push_back(Coord(x, y));
    //    }
    //}

    Coord endNode = std::make_pair( mazeSize - 1, mazeSize - 1 );
    bool reachedEndNode = false;

    while ( !queue.empty() )
    {
        uint32_t coordIndex;
        Coord coord = findCoordWithSmallestCost(costMaze, queue, coordIndex);
        queue.erase(queue.begin() + coordIndex);

        if ( coord == endNode )
        {
            reachedEndNode = true;
            break;
        }

        std::vector<Coord> neighbors = getValidNeighbors(coord, visited, mazeSize);

        for ( const Coord& n : neighbors )
        {
            int costThroughHere = costMaze.at(coord.second).at(coord.first) + maze.at(n.second).at(n.first);

            if ( costThroughHere < costMaze.at(n.second).at(n.first) )
                costMaze.at(n.second).at(n.first) = costThroughHere;

            auto idx = std::find(queue.begin(), queue.end(), n);
            if ( idx == queue.end() )
            {
                queue.push_back(n);
            }
        }

        visited.push_back(coord);
    }

    return costMaze.at(endNode.first).at(endNode.second);
}

int main(int argc, char* argv[])
{
    std::ifstream is(argv[1]);

    if ( !is.is_open() )
    {
        std::cerr << "Failed to open file!" << std::endl;
        return 1;
    }

    Maze maze;
    int size;
    readInput(is, maze, size);

    //computeLowestTotalRisk(maze, size, 0, 0, 0, lowestTotalRisk, visited);
    int lowestTotalRisk = computeLowestTotalRisk(maze, size);


    std::cout << lowestTotalRisk << std::endl;

    return 0;
}
