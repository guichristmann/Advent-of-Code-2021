#include <fstream>
#include <iostream>
#include <vector>
#include <utility>
#include <unordered_map>

typedef std::unordered_map<std::string, std::string> insertion_rules;

std::unordered_map<std::string, uint64_t> readInput(std::ifstream& is, std::string& outPolymerTemplate, insertion_rules& outRules)
{
    // The return variable.
    std::unordered_map<std::string, uint64_t> elementPairCount;

    // Starting template is on the first line.
    std::getline(is, outPolymerTemplate);

    std::string tmpLine;
    std::getline(is, tmpLine); // Skips empty line.
    while ( std::getline(is, tmpLine) )
    {
        auto splitIndex = tmpLine.find('-');

        std::string pattern = tmpLine.substr(0, splitIndex - 1);
        std::string polymerToInsert = tmpLine.substr(splitIndex + 3);

        outRules.emplace(pattern, polymerToInsert);
    }

    for ( int i = 0; i < outPolymerTemplate.size() - 1; i++ )
    {
        std::string elementPair = outPolymerTemplate.substr(i, 2);
        auto itr = elementPairCount.find(elementPair);
        if ( itr == elementPairCount.end() )
        {
            elementPairCount.emplace(elementPair, 1);
        } else 
        {
            elementPairCount.at(elementPair)++;
        }
    }

    return elementPairCount;
}

std::unordered_map<char, uint64_t> getElementCountFromPolymer(const std::string& polymer)
{
    std::unordered_map<char, uint64_t> elementCount;

    for ( const char& c : polymer )
    {
        auto idx = elementCount.find(c);
        if ( idx == elementCount.end() )
            elementCount.emplace(c, 1);
        else
            elementCount.at(c)++;
    }

    return elementCount;
}

void polymerizeStep(std::unordered_map<std::string, uint64_t>& pairCount, 
        std::unordered_map<char, uint64_t>& elementCount, const insertion_rules& rules)
{
    std::unordered_map<std::string, uint64_t> newPairCount;

    for ( const auto& pair : pairCount )
    {
        // Skipping error catching here. Rules should include all possible pairs.
        std::string newElement = rules.at(pair.first);

        // Update element count.
        auto idx = elementCount.find(newElement.at(0));
        if ( idx == elementCount.end() )
            elementCount.emplace(newElement.at(0), 1);
        else
            idx->second += pair.second;

        // Update pair count.
        std::string pair1 = pair.first.at(0) + newElement;
        std::string pair2 = newElement + pair.first.at(1);

        auto idx1 = newPairCount.find(pair1);
        if ( idx1 == newPairCount.end() )
            newPairCount.emplace(pair1, pair.second);
        else
            idx1->second += pair.second;

        auto idx2 = newPairCount.find(pair2);
        if ( idx2 == newPairCount.end() )
            newPairCount.emplace(pair2, pair.second);
        else
            idx2->second += pair.second;
    }

    pairCount = newPairCount;
}

int main(int argc, char* argv[])
{
    std::ifstream is(argv[1]);
    if ( !is.is_open() )
    {
        std::cerr << "Failed to open file!" << std::endl;        
        return 1;
    }

    std::string polymer;
    insertion_rules rules;

    // Starting conditions.
    auto elementPairCount = readInput(is, polymer, rules);
    auto elementCount = getElementCountFromPolymer(polymer); 

    for ( int i = 0; i < 40; i++ )
    {
        polymerizeStep(elementPairCount, elementCount, rules);
    }

    uint64_t most = 0, least = UINT64_MAX;
    for ( const auto& e : elementCount )
    {
        std::cout << e.first << ": " << e.second << std::endl;
        if ( e.second > most )
            most = e.second;
        
        if ( e.second < least )
            least = e.second;
    }

    std::cout << "Result: " << most - least << std::endl;

    return 0;
}
