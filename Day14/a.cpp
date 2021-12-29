#include <fstream>
#include <iostream>
#include <vector>
#include <utility>
#include <unordered_map>

typedef std::unordered_map<std::string, std::string> insertion_rules;

void readInput(std::ifstream& is, std::string& outPolymerTemplate, insertion_rules& outRules)
{
    //
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
}

std::string polymerizeStep(const std::string& polymer, const insertion_rules& rules)
{
    std::string newPolymer("");

    for ( std::string::size_type i = 0; i < polymer.size() - 1; i++ )
    {
        std::string pair = polymer.substr(i, 2);

        // Should always have a match, unless we messed something up.
        std::string match = rules.at(pair);

        pair.insert(1, match);
        newPolymer.append(pair.substr(0, 2));
    }

    newPolymer.push_back(polymer.at(polymer.size() - 1));

    return newPolymer;
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

    readInput(is, polymer, rules);

    for (int i = 0; i < 10; i++)
        polymer = polymerizeStep(polymer, rules);

    std::unordered_map<char, uint64_t> count;

    for (const char& c : polymer)
    {
        auto idx = count.find(c);
        if ( idx == count.end() )
        {
            count.emplace(c, 1);
        } else 
        {
            count.at(c)++;
        }
    }

    uint64_t mostCommonCount = 0;
    uint64_t leastCommonCount = UINT64_MAX;

    for ( const auto& e : count )
    {
        if ( e.second > mostCommonCount )
            mostCommonCount = e.second;

        if ( e.second < leastCommonCount )
            leastCommonCount = e.second;
    }

    std::cout << "Result: " << mostCommonCount - leastCommonCount << std::endl;

    std::cout << polymer << std::endl;

    return 0;
}
