#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <utility>
#include <boost/functional/hash.hpp>

constexpr int64_t g_min_x = -50;
constexpr int64_t g_max_x = 50;
constexpr int64_t g_min_y = -50; 
constexpr int64_t g_max_y = 50; 
constexpr int64_t g_min_z = -50; 
constexpr int64_t g_max_z = 50; 

struct Point
{
    int64_t x, y, z;

    Point(int64_t x, int64_t y, int64_t z) : x(x), y(y), z(z) {};

    bool operator==(const Point& other) const
    {
        return (x == other.x) && (y == other.y) && (z == other.z);
    }
};

template <>
struct std::hash<Point>
{
    std::size_t operator()(const Point& p) const
    {
        using boost::hash_value;
        using boost::hash_combine;

        std::size_t seed = 0;

        hash_combine(seed, hash_value(p.x));
        hash_combine(seed, hash_value(p.y));
        hash_combine(seed, hash_value(p.z));

        return seed;
    }
};

typedef std::unordered_map<Point, bool> Field;

void readRanges(std::string& line, std::pair<int64_t, int64_t>& out_x_range, 
    std::pair<int64_t, int64_t>& out_y_range, std::pair<int64_t, int64_t>& out_z_range)
{
    auto equalIdx = line.find('=');
    auto dotsIdx = line.find("..");
    auto commaIdx = line.find(',');
    out_x_range.first = std::stoi(line.substr(equalIdx + 1, dotsIdx - equalIdx - 1));
    out_x_range.second = std::stoi(line.substr(dotsIdx + 2, commaIdx - dotsIdx - 2));

    line = line.substr(commaIdx + 1);
    equalIdx = line.find('=');
    dotsIdx = line.find("..");
    commaIdx = line.find(',');
    out_y_range.first = std::stoi(line.substr(equalIdx + 1, dotsIdx - equalIdx - 1));
    out_y_range.second = std::stoi(line.substr(dotsIdx + 2, commaIdx - dotsIdx - 2));

    line = line.substr(commaIdx + 1);
    equalIdx = line.find('=');
    dotsIdx = line.find("..");
    commaIdx = line.find(',');
    out_z_range.first = std::stoi(line.substr(equalIdx + 1, dotsIdx - equalIdx - 1));
    out_z_range.second = std::stoi(line.substr(dotsIdx + 2, commaIdx - dotsIdx - 2));
}

void processInput(std::ifstream& is, Field& field)
{
    std::string tmp_line; 

    std::pair<int64_t, int64_t> x_range, y_range, z_range;
    bool turn;
    uint64_t count = 0;
    while ( std::getline(is, tmp_line, '\n') )
    {
        //std::cout << tmp_line << std::endl;
        //std::cin.ignore();
        turn = tmp_line.substr(0, 3) == "on ";
        readRanges(tmp_line, x_range, y_range, z_range);

        int64_t start_z = std::max(z_range.first, g_min_z);
        int64_t stop_z = std::min(z_range.second, g_max_z);
        int64_t start_y = std::max(y_range.first, g_min_y);
        int64_t stop_y = std::min(y_range.second, g_max_y);
        int64_t start_x = std::max(x_range.first, g_min_x);
        int64_t stop_x = std::min(x_range.second, g_max_x);
        for ( int64_t z = start_z; z <= stop_z; z++)
        {
            for ( int64_t y = start_y; y <= stop_y; y++)
            {
                for ( int64_t x = start_x; x <= stop_x; x++)
                {
                    field[Point(x, y, z)] = turn;
                }
            }
        }
    }
}

int main(int argc, char* argv[])
{
    std::ifstream is(argv[1]);
    if ( !is.is_open() )
    {
        std::cerr << "Failed to open file!" << std::endl;
        return 1;
    }

    Field field;
    processInput(is, field);

    uint64_t cubeOnCount = 0;
    for ( const auto& v : field )
    {
        if ( v.second == true )
            cubeOnCount += 1;
    }

    std::cout << "Result is " << cubeOnCount << std::endl;


    return 0;
}
