// This solution could be greatly improved. It gives the right answer but takes about ~15 min to compute it.

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <boost/unordered_map.hpp>

#define SIZE_TIMES 5

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

    // Expanding columns (inner vectors)
    for ( uint32_t r = 0; r < size; r++ )
    {
        for ( uint32_t inc = 1; inc < SIZE_TIMES; inc++ )
        {
            for ( uint32_t c = 0; c < size; c++ )
            {
                int val = outMaze.at(r).at(c) + inc;
                if ( val >= 10 )
                    outMaze.at(r).push_back(val - 9);
                else
                    outMaze.at(r).push_back(val);
            }        
        }
    }

    // Expanding rows.
    for ( uint32_t inc = 1; inc < SIZE_TIMES; inc++ )
    {
        for ( uint32_t r = 0; r < size; r++ )
        {
            outMaze.push_back(outMaze.at(r));

            for ( int& v : outMaze.at(outMaze.size() - 1) )
            {
                v += inc;
                if ( v >= 10 )
                    v = v - 9;
            }
        } 
    }

    // Update size to new size.
    size *= 5;
}

void printMaze(const Maze& maze, const std::string& sep)
{
    for ( const auto& row : maze )
    {
        for ( const int& risk : row )
        {
            std::cout << risk << sep;
        }

        std::cout << std::endl;
    }
}

int computeLowestTotalRisk(const Maze& maze, const int mazeSize)
{
    std::deque<Coord> queue { { Coord(0, 0) } };

    Maze costMaze(mazeSize, std::vector<int>(mazeSize, INT32_MAX));
    costMaze.at(0).at(0) = 0;

    Coord endNode = std::make_pair( mazeSize - 1, mazeSize - 1 );
    bool reachedEndNode = false;

    while ( !queue.empty() )
    {
        Coord curr = queue.front();
        queue.pop_front();

        int currCost = costMaze[curr.first][curr.second];

        if ( curr.first > 0 && 
            currCost + maze[curr.first - 1][curr.second] < costMaze[curr.first - 1][curr.second] )
        {
            costMaze[curr.first - 1][curr.second] = currCost + maze[curr.first - 1][curr.second];
            queue.push_back(Coord(curr.first - 1, curr.second));
        }

        if ( curr.first < mazeSize - 1 
            && currCost + maze[curr.first + 1][curr.second] < costMaze[curr.first + 1][curr.second] )
        {
            costMaze[curr.first + 1][curr.second] = currCost + maze[curr.first + 1][curr.second];
            queue.push_back(Coord(curr.first + 1, curr.second));
        }

        if ( curr.second > 0 && 
            currCost + maze[curr.first][curr.second - 1] < costMaze[curr.first][curr.second - 1] )
        {
            costMaze[curr.first][curr.second - 1] = currCost + maze[curr.first][curr.second - 1];
            queue.push_back(Coord(curr.first, curr.second - 1));
        }

        if ( curr.second < mazeSize - 1 
            && currCost + maze[curr.first][curr.second + 1] < costMaze[curr.first][curr.second + 1] )
        {
            costMaze[curr.first][curr.second + 1] = currCost + maze[curr.first][curr.second + 1];
            queue.push_back(Coord(curr.first, curr.second + 1));
        }

        //std::cout << visited.size() << std::endl;
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

    //printMaze(maze, "");

    int lowestTotalRisk = computeLowestTotalRisk(maze, size);

    std::cout << lowestTotalRisk << std::endl;

    return 0;
}
