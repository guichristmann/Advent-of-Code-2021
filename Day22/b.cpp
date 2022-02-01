#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <utility>
#include <vector>

struct Cuboid
{
    int64_t x1, x2, y1, y2, z1, z2;

    Cuboid();
    Cuboid(int64_t x1, int64_t x2, int64_t y1, int64_t y2, int64_t z1, int64_t z2) : x1(x1), x2(x2), y1(y1), 
        y2(y2), z1(z1), z2(z2) {};

    bool overlaps(const Cuboid& other) const
    {
        bool common_x = false;
        bool common_y = false;
        bool common_z = false;

        if ( (x1 >= other.x1 && x1 <= other.x2) || (x2 >= other.x1 && x2 <= other.x2) || 
             (x1 <= other.x1 && x2 >= other.x2) )
        {
            common_x = true;      
        }

        if ( (y1 >= other.y1 && y1 <= other.y2) || (y2 >= other.y1 && y2 <= other.y2) || 
             (y1 <= other.y1 && y2 >= other.y2) )
        {
            common_y = true;      
        }

        if ( (z1 >= other.z1 && z1 <= other.z2) || (z2 >= other.z1 && z2 <= other.z2) || 
             (z1 <= other.z1 && z2 >= other.z2) )
        {
            common_z = true;      
        }

        return common_x && common_y && common_z;
    }

    Cuboid intersection(const Cuboid& other) const
    {
        return Cuboid(
                std::max(x1, other.x1),
                std::min(x2, other.x2),
                std::max(y1, other.y1),
                std::min(y2, other.y2),
                std::max(z1, other.z1),
                std::min(z2, other.z2)
        );
    }

    int64_t volume() const
    {
        int64_t span_x = x2 - x1 + 1;
        int64_t span_y = y2 - y1 + 1;
        int64_t span_z = z2 - z1 + 1;

        return span_x * span_y * span_z;
    }

    bool operator==(const Cuboid& other) const
    {
        return (x1 == other.x1) && (x2 == other.x2) &&
               (y1 == other.y1) && (y2 == other.y2) &&
               (z1 == other.z1) && (z2 == other.z2);
    }

    friend std::ostream& operator<<(std::ostream&, const Cuboid&);
};

std::ostream& operator<<(std::ostream& out, const Cuboid& c)
{
    out << "x: " << c.x1 << " -> " << c.x2 << " | y: " << c.y1 << " -> " << c.y2 << " | z: " << c.z1 << " -> " << c.z2;
    return out;
}

void readRanges(std::string& line, std::pair<int64_t, int64_t>& out_x_range, 
    std::pair<int64_t, int64_t>& out_y_range, std::pair<int64_t, int64_t>& out_z_range)
{
    auto equalIdx = line.find('=');
    auto dotsIdx = line.find("..");
    auto commaIdx = line.find(',');
    out_x_range.first = std::stoll(line.substr(equalIdx + 1, dotsIdx - equalIdx - 1));
    out_x_range.second = std::stoll(line.substr(dotsIdx + 2, commaIdx - dotsIdx - 2));

    line = line.substr(commaIdx + 1);
    equalIdx = line.find('=');
    dotsIdx = line.find("..");
    commaIdx = line.find(',');
    out_y_range.first = std::stoll(line.substr(equalIdx + 1, dotsIdx - equalIdx - 1));
    out_y_range.second = std::stoll(line.substr(dotsIdx + 2, commaIdx - dotsIdx - 2));

    line = line.substr(commaIdx + 1);
    equalIdx = line.find('=');
    dotsIdx = line.find("..");
    commaIdx = line.find(',');
    out_z_range.first = std::stoll(line.substr(equalIdx + 1, dotsIdx - equalIdx - 1));
    out_z_range.second = std::stoll(line.substr(dotsIdx + 2, commaIdx - dotsIdx - 2));
}

// Splits `original`, delimited by `inter`.
void splitCuboid(const Cuboid& original, const Cuboid& inter, std::vector<Cuboid>& outList)
{
    Cuboid c1(inter.x2 + 1, original.x2, original.y1, original.y2, original.z1, original.z2);
    Cuboid c2(original.x1, inter.x1 - 1, original.y1, original.y2, original.z1, original.z2);
    Cuboid c3(inter.x1, inter.x2, original.y1, original.y2, original.z1, inter.z1 - 1);
    Cuboid c4(inter.x1, inter.x2, original.y1, original.y2, inter.z2 + 1, original.z2);
    Cuboid c5(inter.x1, inter.x2, original.y1, inter.y1 - 1, inter.z1, inter.z2);
    Cuboid c6(inter.x1, inter.x2, inter.y2 + 1, original.y2, inter.z1, inter.z2);

    if ( c1.volume() > 0 )
        outList.push_back(c1);
    if ( c2.volume() > 0 )
        outList.push_back(c2);
    if ( c3.volume() > 0 )
        outList.push_back(c3);
    if ( c4.volume() > 0 )
        outList.push_back(c4);
    if ( c5.volume() > 0 )
        outList.push_back(c5);
    if ( c6.volume() > 0 )
        outList.push_back(c6);
}

void processInput(std::ifstream& is, std::vector<Cuboid>& cuboids)
{
    std::string tmp_line; 

    std::pair<int64_t, int64_t> x_range, y_range, z_range;
    bool turn;
    while ( std::getline(is, tmp_line, '\n') )
    {
        turn = tmp_line.substr(0, 3) == "on ";
        readRanges(tmp_line, x_range, y_range, z_range);

        // Create a cuboid from the command ranges. 
        Cuboid cmd(x_range.first, x_range.second, y_range.first, y_range.second, z_range.first, z_range.second);

        if ( cuboids.size() == 0 )
        {
            cuboids.push_back(cmd);
            continue;
        }

        std::vector<Cuboid> newCuboidsList;

        if ( turn )
            newCuboidsList.push_back(cmd);

        for ( const Cuboid& cuboid : cuboids )
        {
            if ( cuboid.overlaps(cmd) )
            {
                Cuboid intersected = cuboid.intersection(cmd);
                splitCuboid(cuboid, intersected, newCuboidsList);
            }
            else
            {
                // Keep the cuboid on the list.
                newCuboidsList.push_back(cuboid);
            }
        }
        cuboids = newCuboidsList;
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

    std::vector<Cuboid> cuboids;
    processInput(is, cuboids);

    uint64_t totalVolume = 0;

    for ( const Cuboid& c : cuboids )
    {
        totalVolume += c.volume();
    }

    std::cout << "Volume: " << totalVolume << std::endl;

    return 0;
}
