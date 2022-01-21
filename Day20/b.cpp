#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <array>
#include <assert.h>
#include <bitset>

typedef std::array<bool, 512> Algorithm;

#define DATA_ON 1
#define DATA_OFF 0

struct Image
{
    Image() {};
    Image(const std::vector<std::vector<int>>& data) : data(data) {};

    void Pad(const int);
    void Print() const;
    std::vector<std::vector<int>> data;
};

void Image::Print() const
{
    for ( uint32_t y = 0; y < data.size(); y++ )
    {
        for ( uint32_t x = 0; x < data.at(0).size(); x++ )
        {
            if ( data.at(y).at(x) == DATA_ON )
                std::cout << "#";
            else if ( data.at(y).at(x) == DATA_OFF )
                std::cout << ".";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
}

void Image::Pad(const int amount)
{
    // Pad top rows and bottom rows.
    std::vector<int> row(data.at(0).size(), DATA_OFF);
    for ( int i = 0; i < amount; i++ )
    {
        data.insert(data.begin(), row);
        data.insert(data.end(), row);
    }

    // Pad columns.
    for ( uint32_t row = 0; row < data.size(); row++ )
    {
        for ( int i = 0; i < amount; i++ )
        {
            data.at(row).insert(data.at(row).begin(), DATA_OFF);
            data.at(row).insert(data.at(row).end(), DATA_OFF);
        }
    }
}

void readInput(std::ifstream& is, Algorithm& outAlgo, Image& outImg)
{
    std::string line;
    
    // First line contains the complete algorithm.
    is >> line;
    uint32_t i = 0;
    for ( const char c : line )
    {
        if ( c == '#' )
        {
            outAlgo.at(i) = true;
        }
        else if ( c == '.' )
        {
            outAlgo.at(i) = false;
        }
        else
        {
            std::cerr << "Unexpected value encountered. Something is wrong!" << std::endl;
        }

        i++;
    }
    assert(i == 512);

    // Read the image. First line used to determine the square dimensions. 
    while ( is >> line )
    { 
        std::vector<int> row;
        for ( const char c : line )
        {
            if ( c == '#' )
            {
                row.push_back(1);
            }
            else if ( c == '.' )
            {
                row.push_back(0);
            }
            else
            {
                std::cerr << "Unexpected value encountered. Something is wrong!" << std::endl;
            }
        }
        outImg.data.push_back(row);
    }
}

bool isDataOn(int x, int y, const Image& img)
{
    // Wrap around.
    if ( x == -1 )
        x = img.data.at(0).size() - 1;
    else if ( x == img.data.at(0).size() )
        x = 0;
    if ( y == -1 )
        y = img.data.size() - 1;
    else if ( y == img.data.size() )
        y = 0;

    if ( img.data.at(y).at(x) == DATA_ON )
    {
        return true;
    }

    return false;
}

void operate(const int x, const int y, const Image& img, Image& outResultImg, const Algorithm& algo)
{
    std::bitset<9> binaryIdx = {0};
    if ( isDataOn(x - 1, y - 1, img) )
        binaryIdx.set(8);
    if ( isDataOn(x, y - 1, img) )
        binaryIdx.set(7);
    if ( isDataOn(x + 1, y - 1, img) )
        binaryIdx.set(6);

    if ( isDataOn(x - 1, y, img) )
        binaryIdx.set(5);
    if ( isDataOn(x, y, img) )
        binaryIdx.set(4);
    if ( isDataOn(x + 1, y, img) )
        binaryIdx.set(3);

    if ( isDataOn(x - 1, y + 1, img) )
        binaryIdx.set(2);
    if ( isDataOn(x, y + 1, img) )
        binaryIdx.set(1);
    if ( isDataOn(x + 1, y + 1, img) )
        binaryIdx.set(0);
    
    uint32_t decimalIdx = binaryIdx.to_ulong();

    outResultImg.data.at(y).at(x) = algo.at(decimalIdx);
}

void enhance(Image& img, const Algorithm& algo)
{
    Image resultImg(img.data);
    
    for ( int k_y = 0; k_y < img.data.size(); k_y++ )
    {
        for ( int k_x = 0; k_x < img.data.at(0).size(); k_x++ )
        {
            operate(k_x, k_y, img, resultImg, algo);
        }
    }

    img = resultImg;
}

uint32_t countBrightPixels(const Image& img)
{
    uint32_t count = 0;
    for ( uint32_t row = 0; row < img.data.size(); row++ )
    {
        for ( uint32_t col = 0; col < img.data.at(0).size(); col++ )
        {
            if ( img.data.at(row).at(col) == DATA_ON )
                count++;
        }
    }

    return count;
}

int main(int argc, char* argv[])
{
    std::ifstream is(argv[1]); 
    if ( !is.is_open() )
    {
        std::cerr << "Failed to open file!" << std::endl;
        return 1;
    }

    Algorithm algo;
    Image img;

    readInput(is, algo, img);

    // Adding enough padding to allow the image to grow.
    img.Pad(80);
    for ( int i = 0; i < 50; i++ )
    {
        enhance(img, algo);
    }

    img.Print();
    std::cout << "Result is " << countBrightPixels(img) << std::endl;

    return 0;
}
