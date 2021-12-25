#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <utility>

struct TransparentPaper {
    typedef std::pair<int, int> Coordinate;

    std::vector<Coordinate> coordinates;
    uint32_t height;
    uint32_t width;
};

TransparentPaper readInput(std::ifstream& is)
{
    TransparentPaper paper;

    // Read all the dots on the paper. Stop at the blank line,
    std::string tmpLine;
    uint32_t maxX = 0, maxY = 0;
    while ( std::getline(is, tmpLine, '\n') )
    {
        if ( tmpLine == "" )
            break;

        auto indexSplit = tmpLine.find(',');

        int x = std::stoi(tmpLine.substr(0, indexSplit));
        int y = std::stoi(tmpLine.substr(indexSplit + 1));

        if (x > maxX)
            maxX = x;
        if (y > maxY)
            maxY = y;

        paper.coordinates.push_back(TransparentPaper::Coordinate(x, y));
    }

    paper.width = maxX + 1;
    paper.height = maxY + 1;

    return paper;
}

void executeFoldInstructions(std::ifstream& is, TransparentPaper& paper, bool foldOnce)
{
    std::string tmpLine; 

    while ( std::getline(is, tmpLine) )
    {
        std::cout << tmpLine << std::endl;
        auto indexSplit = tmpLine.find('=');

        bool horizontalFold = tmpLine.at(indexSplit - 1) == 'y';
        int foldCoordinate = std::stoi(tmpLine.substr(indexSplit + 1));
        std::cout << horizontalFold << ": " << foldCoordinate << std::endl;

        if ( horizontalFold )
        {
            for (auto& coord : paper.coordinates)
            {
                if ( coord.second < foldCoordinate )
                    continue;

                int distToFold = coord.second - foldCoordinate;
                coord.second = foldCoordinate - distToFold;

            }
            paper.height = paper.height - foldCoordinate - 1;

        } else 
        {
            for (auto& coord : paper.coordinates)
            {
                if ( coord.first < foldCoordinate )
                    continue;

                int distToFold = coord.first - foldCoordinate;
                coord.first = foldCoordinate - distToFold;

            }
            paper.width = paper.width - foldCoordinate - 1;
        }

        if ( foldOnce )
            break;
    }
}

int main(int argc, char* argv[])
{
    std::ifstream is(argv[1]);

    if ( !is.is_open() )
    {
        std::cerr << "Couldn't open file!" << std::endl;
        return 1;
    }

    TransparentPaper paper = readInput(is);

    std::cout << "Original: " << paper.width << "x" << paper.height << std::endl;

    // For Part 1.
    //executeFoldInstructions(is, paper, true);
    // For Part 2.
    executeFoldInstructions(is, paper, false);

    // Visualize and count for the dots.
    uint32_t dotCount = 0;
    std::vector<std::vector<char>> vizPaper(paper.height, std::vector<char>(paper.width, '.'));
    for (const auto& c : paper.coordinates)
    {
        vizPaper.at(c.second).at(c.first) = '#';
    }

    for ( int y = 0; y < paper.height; y++ )
    {
        for ( int x = 0; x < paper.width; x++ )
        {
            std::cout << vizPaper.at(y).at(x); 
            if ( vizPaper.at(y).at(x) == '#' )
                dotCount++;
        }
        std::cout << std::endl;
    }

    std::cout << "Total dots: " << dotCount << std::endl;

    return 0;
}

