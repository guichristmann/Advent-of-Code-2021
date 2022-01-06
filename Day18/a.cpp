#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>

#define I_OPEN_BRACKET -1
#define I_CLOSE_BRACKET -2
#define I_COMMA -3

void addSnailfishNumbers(std::vector<int>& a, const std::vector<int>& b)
{
    a.insert(a.begin(), I_OPEN_BRACKET);
    a.insert(a.end(), I_COMMA);
    a.insert(a.end(), b.begin(), b.end());
    a.insert(a.end(), I_CLOSE_BRACKET);
}

bool isNumber(const int c)
{
    if ( c == I_OPEN_BRACKET || c == I_CLOSE_BRACKET || c == I_COMMA )
        return false;
    else
        return true;
}

void printSequence(const std::vector<int>& s)
{
    for ( const int& i : s )
    {
        if ( i == I_OPEN_BRACKET )
            std::cout << "[";
        else if ( i == I_CLOSE_BRACKET )
            std::cout << "]";
        else if ( i == I_COMMA )
            std::cout << ",";
        else
            std::cout << i;
    }
    std::cout << std::endl;
}

void explode(std::vector<int>& s, const int64_t i)
{
    //std::cout << "\n##### [Explode OP] #####" << std::endl;
    // Grab left and right values of the pair.
    int leftVal = s.at(i + 1);
    int rightVal = s.at(i + 3);
    
    // Search left.
    auto leftIdx = std::find_if(s.rbegin() + s.size() - i, s.rend(), isNumber);
    if ( leftIdx != s.rend() )
    {
        *leftIdx += leftVal;
    } 
    
    // Search right.
    auto rightIdx = std::find_if(s.begin() + i + 4, s.end(), isNumber);
    if ( rightIdx != s.end() )
    {
        *rightIdx += rightVal;
    } 

    // Remove exploded pair, replace with 0.
    s.erase(s.begin() + i, s.begin() + i + 5);
    s.insert(s.begin() + i, 0);

    //std::cout << "### [Finish explode] ###\n" << std::endl;
}

void split(std::vector<int>& s, const int64_t i)
{
    int newLeft = s.at(i) / 2;
    int newRight = s.at(i) / 2 + (s.at(i) % 2 != 0);

    std::vector<int> toInsert = { I_OPEN_BRACKET, newLeft, I_COMMA, newRight, I_CLOSE_BRACKET };

    s.erase(s.begin() + i);
    s.insert(s.begin() + i, toInsert.begin(), toInsert.end());
}

int64_t findExplodeIndex(const std::vector<int>& s)
{
    int depth = 0;
    int64_t explodeIdx = -1;
    for ( uint64_t i = 0; i < s.size(); i++ )
    {
        if ( s.at(i) == I_OPEN_BRACKET )
        {
            depth++;
        }
        else if ( s.at(i) == I_CLOSE_BRACKET )
        {
            depth--;
        }

        if ( depth == 5 )
        {
            explodeIdx = i;
            break;
        }
    }

    return explodeIdx;
}

int64_t findSplitIdx(const std::vector<int>& s)
{
    int64_t splitIdx = -1;
    for ( uint64_t i = 0; i < s.size(); i++ )
    {
        if ( s.at(i) >= 10 )
        {
            splitIdx = i;
            break;
        }
    }

    return splitIdx;
}

void reduce(std::vector<int>& s)
{
    bool explodeFlag = false;
    bool splitFlag = false;
    bool fullyReduced = false;
    while ( !fullyReduced )
    {
        // Find something to explode.
        int64_t explodeIdx = findExplodeIndex(s);

        if ( explodeIdx != -1 )
        {
            explode(s, explodeIdx);
        }
        else
        {
            // Find something to split.
            int64_t splitIdx = findSplitIdx(s);

            if ( splitIdx != -1 )
            {
                split(s, splitIdx);
            }
            else
            {
                fullyReduced = true;
            }
                
        }
    }

}

std::vector<int> stringToVecInt(const std::string& string)
{
    std::vector<int> encoded;

    for ( const char& c : string )
    {
        switch ( c )
        {
            case '[':
                encoded.push_back(I_OPEN_BRACKET);
                break;
            case ']':
                encoded.push_back(I_CLOSE_BRACKET);
                break;
            case ',':
                encoded.push_back(I_COMMA);
                break;
            default:
                encoded.push_back((int) c - '0');
                break;
        }
    }

    return encoded;
}

int64_t firstRegularPair(const std::vector<int>& s)
{
    int64_t currStart = 0;
    while ( currStart < s.size() )
    {
        // Find the first pair with two regular numbers and replace them. 
        auto idx = std::find_if(s.begin() + currStart, s.end(), isNumber);
        if ( idx == s.end() )
        {
            std::cout << "Probably should finish before this prints out..."  << std::endl;
            return -1;
        }

        if ( isNumber(*(idx + 2)) )
            return idx - s.begin();

        currStart = idx - s.begin() + 1;
    }

    return -1;
}

int64_t computeMagnitude(std::vector<int>& s)
{
    while ( s.size() > 1 )
    {
        int64_t idx = firstRegularPair(s);

        if ( idx == -1 )
            std::cout << "Something went wrong, I guess..." << std::endl;

        int leftVal = s.at(idx);
        int rightVal = s.at(idx + 2);

        int newVal = 3 * leftVal + 2 * rightVal;

        s.insert(s.begin() + idx - 1, newVal);
        s.erase(s.begin() + idx, s.begin() + idx + 5 );
    }

    return s.at(0);
}

int64_t solve(std::ifstream& is)
{
    // Read the first line directly into the sequence.
    std::string line;
    std::getline(is, line);

    // TODO: Reduce operations?
    std::vector<int> currSequence = stringToVecInt(line);

    std::vector<int> tmpSequence;
    while ( std::getline(is, line) )
    {
        tmpSequence = stringToVecInt(line);
        addSnailfishNumbers(currSequence, tmpSequence);

        reduce(currSequence);
    }

    //printSequence(currSequence);

    // Find the magnitude of the final sequence.
    return computeMagnitude(currSequence);
}

int main(int argc, char* argv[])
{
    std::ifstream is(argv[1]);

    if ( !is.is_open() )
    {
        std::cerr << "Failed to open file." << std::endl;
        return 1;
    }

    int64_t magnitude = solve(is);

    std::cout << "Final magnitude is " << magnitude << std::endl;
}
