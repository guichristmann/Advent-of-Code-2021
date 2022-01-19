#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include <math.h>
#include <algorithm>

struct Point
{
    int32_t x, y, z;

    Point() {}
    Point(int32_t x, int32_t y, int32_t z) : x(x), y(y), z(z) {}

    int& operator[](int index)
    {
        if ( index < 0 || index >= 3 )
            throw std::out_of_range("Invalid index.");

        if ( index == 0 )
            return x;
        else if ( index == 1 )
            return y;
        else if ( index == 2 )
            return z;

        throw std::out_of_range("Unexpected end of function.");
    }

    const int& operator[](int index) const
    {
        if ( index < 0 || index >= 3 )
            throw std::out_of_range("Invalid index.");

        if ( index == 0 )
            return x;
        else if ( index == 1 )
            return y;
        else if ( index == 2 )
            return z;

        throw std::out_of_range("Unexpected end of function.");
    }

    bool operator==(const Point& other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }
};

template <> struct std::hash<Point> {
  std::size_t operator()(Point const &s) const noexcept {
    std::size_t h1 = std::hash<int64_t>{}(s.x);
    std::size_t h2 = std::hash<int64_t>{}(s.y);
    std::size_t h3 = std::hash<int64_t>{}(s.z);
    return h1 ^ (h2 << 1) ^ (h3 << 2); // or use boost::hash_combine
  }
};

Point operator-(const Point& p1, const Point& p2)
{
    return { p1.x - p2.x, p1.y - p2.y, p1.z - p2.z };
}

Point operator+(const Point& p1, const Point& p2)
{
    return { p1.x + p2.x, p1.y + p2.y, p1.z + p2.z };
}

typedef std::vector<Point> Scan;

struct Operation
{
    std::vector<int> opAxis;
    std::vector<int> opMult;

    Operation() {} ;

    Operation(int x, int y, int z, int x_m, int y_m, int z_m) : opAxis( {x, y, z} ), opMult( {x_m, y_m, z_m} ) {} ;
};

const std::vector<Operation> operations = {
    {0, 1, 2, 1, 1, 1},
    {0, 2, 1, 1, -1, 1},
    {0, 1, 2, 1, -1, -1},
    {0, 2, 1, 1, 1, -1},
    {1, 0, 2, -1, 1, 1},
    {2, 0, 1, 1, 1, 1},
    {1, 0, 2, 1, 1, -1},
    {2, 0, 1, -1, 1, -1},
    {0, 1, 2, -1, -1, 1},
    {0, 2, 1, -1, -1, -1},
    {0, 1, 2, -1, 1, -1},
    {0, 2, 1, -1, 1, 1},
    {1, 0, 2, 1, -1, 1},
    {2, 0, 1, 1, -1, -1},
    {1, 0, 2, -1, -1, -1},
    {2, 0, 1, -1, -1, 1},
    {2, 1, 0, -1, 1, 1},
    {1, 2, 0, 1, 1, 1},
    {2, 1, 0, 1, -1, 1},
    {1, 2, 0, -1, -1, 1},
    {2, 1, 0, -1, -1, -1},
    {1, 2, 0, -1, 1, -1},
    {2, 1, 0, 1, 1, -1},
    {1, 2, 0, 1, -1, -1}
};

std::vector<Scan> readInput(std::ifstream& is)
{
    std::vector<Scan> readings;

    std::string line;

    while ( is.peek() != EOF )
    {
        Scan scan;
        std::getline(is, line, '\n'); // Skip scanner number line.
        while ( std::getline(is, line, '\n') )
        {
            if ( line == "" )
                break;

            std::stringstream ss(line);
            Point p;
            ss >> p.x;
            ss.ignore();
            ss >> p.y;
            ss.ignore();
            ss >> p.z;

            scan.push_back(p);
        }

        readings.push_back(scan);
    }

    return readings;
}


Point transform(const Point& p, const Operation& op)
{
    Point transformedPoint;
    for ( int i = 0; i < 3; i++ )
    {
        int axisIdx = op.opAxis[i];
        int mult = op.opMult[i];
        transformedPoint[i] = p[axisIdx] * mult;
    }

    return transformedPoint;
}

int manhattanDist(const Point& p1, const Point& p2)
{
    return (p2.x - p1.x) + (p2.y - p1.y) + (p2.z - p1.z);
}

// Check if there's overlap between `s1` and `s2`. If yes, then will transform all data from `s2` into `s1`.
bool findOverlapAndTransform(const Scan& s1, Scan& s2, Point& outTranslation)
{
    bool foundOverlap = false;
    Operation rotation;
    Point translation;
    for ( const auto& op : operations )
    {
        std::unordered_map<Point, int> disparityCount;  
        for ( const auto& r1 : s1 )
        {
            for ( const auto& r2 : s2 )
            {
                //int dist = manhattanDist(r1, transform(r2, op));
                Point diffPoint = r1 - transform(r2, op);
                auto idx = disparityCount.find(diffPoint);
                if ( idx == disparityCount.end() )
                    disparityCount[diffPoint] = 1;
                else
                    disparityCount[diffPoint] += 1;
            }
        }

        for ( const auto& e : disparityCount )
        {
            if ( e.second >= 12 )
            {
                foundOverlap = true;
                rotation = op;
                translation = e.first;
                //translation.x *= op.opMult[0];
                //translation.y *= op.opMult[1];
                //translation.z *= op.opMult[2];
            }
        }

        if ( foundOverlap )
            break;
    }

    if ( !foundOverlap )
        return false;

    // Transform all data in s2 into the coordinates of s1.
    for ( auto& e : s2 )
    {
        e = transform(e, rotation);
        e.x += translation.x;
        e.y += translation.y;
        e.z += translation.z;
    }



    outTranslation = translation;
    return true;
}

void printScan(const Scan& s)
{
    for ( const Point& p : s )
    {
        std::cout << p.x << ", " << p.y << ", " << p.z << std::endl;
    }
}

int main(int argc, char* argv[])
{
    std::ifstream is(argv[1]);
    if ( !is.is_open() )
    {
        std::cerr << "Failed to open input file!" << std::endl;
        return 1;
    }

    std::vector<Scan> scanners = readInput(is);
    std::vector<int> transformQueue;
    for ( int i = 1; i < scanners.size(); i++ )
        transformQueue.push_back(i);
    std::vector<int> transformed { 0 };

    std::vector<Point> scannerTranslations;

    while ( transformQueue.size() > 0 )
    {
        // Retrieve the front value and remove from the queue.
        int idxToBeTransformed = transformQueue.front();
        transformQueue.erase(transformQueue.begin());

        bool flag = false;
        for ( const int& i : transformed )
        {
            Point translation;
            flag = findOverlapAndTransform(scanners.at(i), scanners.at(idxToBeTransformed), translation);
            if ( flag )
            {
                scannerTranslations.push_back(translation);
                transformed.push_back(idxToBeTransformed);
                break;
            }
        }

        // Didn't find transform, push id back on the queue.
        if ( !flag )
            transformQueue.push_back(idxToBeTransformed);
    }
    
    int largestDistance = 0;
    for ( const Point& s1 : scannerTranslations )
    {
        for ( const Point& s2 : scannerTranslations )
        {
            int dist = manhattanDist(s1, s2);
            if ( dist > largestDistance )
                largestDistance = dist;
        }
    }

    std::cout << "Largest manhattan distance between any two scanners is " << largestDistance << std::endl;

    return 0;
}
