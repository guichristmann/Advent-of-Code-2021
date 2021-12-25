#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>

struct Line 
{
    int x0, y0;
    int x1, y1;

    Line (int a, int b, int c, int d) : x0 {a}, y0 {b}, x1 {c}, y1 {d} {}

    friend std::ostream& operator<< (std::ostream& out, const Line& l);
};

// Used to hash a std::pair<int, int>
struct pair_hash 
{
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1,T2> &p) const 
    {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);

        // Mainly for demonstration purposes, i.e. works but is overly simple
        // In the real world, use sth. like boost.hash_combine
        return h1 ^ h2;  
    }
};

std::ostream& operator<< (std::ostream& out, const Line& l)
{
    out << "(" << l.x0 << ", " << l.y0 << ") -> (" << l.x1 << ", " << l.y1 << ")";

    return out;
}

void loadLines(std::ifstream& stream, std::vector<Line> &outLines)
{
    std::string line;
    std::stringstream ss;
    int a, b, c, d; 

    while (std::getline(stream, line))
    {
        ss.clear();
        std::string::size_type splitIndex = line.find("->");

        // Read first pair.
        ss.str(line.substr(0, splitIndex));
        ss >> a;
        ss.ignore(); // Skipping the comma character.
        ss >> b;

        // Read second pair.
        ss.str(line.substr(splitIndex + 2));
        ss >> c;
        ss.ignore();
        ss >> d;

        outLines.push_back(Line(a, b, c, d));
    }
}

int min_of(int a, int b)
{
    return a < b ? a : b;
}

int max_of(int a, int b)
{
    return a > b ? a : b;
}

void findAndIncrementCoordMap(std::unordered_map<std::pair<int, int>, int, pair_hash>& outCoordMap, 
        const std::pair<int, int>& coord)
{
    auto itr = outCoordMap.find(coord);
    if (itr == outCoordMap.end())
    {
        outCoordMap.emplace(std::make_pair(coord, 1));
    } else 
    {
        outCoordMap.at(coord) += 1;
    }
}

// Updates the coordinate map with every coordinate that a line passes through.
void updateCoordMap(const Line& line, std::unordered_map<std::pair<int, int>, int, pair_hash>& outCoordMap)
{
    int dist_x = line.x0 - line.x1;
    int dist_y = line.y0 - line.y1;

    if (!(dist_x == 0 || dist_y == 0))
    {
        //std::cout << "Ignoring " << line << " because it's not a row or column line." << std::endl;
        return;
    }

    if (dist_y == 0)
    {
        int y = line.y0; // or y1.
        for (int x = min_of(line.x0, line.x1); x <= max_of(line.x0, line.x1); x++)
        {
            findAndIncrementCoordMap(outCoordMap, std::make_pair(x, y));
        }
    } else 
    {
        int x = line.x0; // or x1.
        for (int y = min_of(line.y0, line.y1); y <= max_of(line.y0, line.y1); y++)
        {
            findAndIncrementCoordMap(outCoordMap, std::make_pair(x, y));
        }
    }
}

int main(int argc, char* argv[])
{
    std::ifstream stream;

    stream.open(argv[1]);
    if (!stream.is_open())
    {
        std::cerr << "Failed to open file!" << std::endl;
    }

    std::vector<Line> lines;
    loadLines(stream, lines);
    std::unordered_map<std::pair<int, int>, int, pair_hash> coordMap;
    for (auto line : lines)
    {
        updateCoordMap(line, coordMap);
    }

    int numberOfIntersections;
    for (auto e : coordMap)
    {
        if (e.second > 1)
        {
            //std::cout << e.first.first << ", " << e.first.second << " | " << e.second << std::endl;
            numberOfIntersections++;
        }
    }

    std::cout << "The number of intersections is " << numberOfIntersections << std::endl;

    return 0;
}
