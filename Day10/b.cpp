#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stack>
#include <algorithm>

std::vector<std::string> readInput(std::istream& is)
{ 
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(is, line))
    {
        lines.push_back(line);
    }

    return lines;
}

uint64_t computeAutocompleteScore(const std::vector<std::string>& lines)
{
    std::vector<uint64_t> autocompleteScores;

    for (const std::string& line : lines)
    {
        std::stack<char> stack;
        stack.push(line.at(0));
        if (stack.top() == ')' || stack.top() == ']' || stack.top() == '}' || stack.top() == '>')
            std::cerr << "Should handle this error case." << std::endl;

        bool isCorrupted = false;

        for (uint32_t i = 1; i < line.size(); i++)
        {
            if (line.at(i) == '(' || line.at(i) == '[' || line.at(i) == '{' || line.at(i) == '<')
            {
                stack.push(line.at(i));
            }
            else if ( (line.at(i) == ')' && stack.top() == '(')  || 
                      (line.at(i) == ']' && stack.top() == '[')  ||
                      (line.at(i) == '}' && stack.top() == '{')  ||
                      (line.at(i) == '>' && stack.top() == '<') )
            {
                stack.pop();
            }
            else
            {
                isCorrupted = true;
                break;
            }
        }

        if (isCorrupted)
            continue; // Ignoring corrupted lines.

        // Autocomplete by checking the top of the stack and adding the appropriate closing character.
        uint64_t lineScore = 0;
        while ( !stack.empty() )
        {
            lineScore *= 5;
            if ( stack.top() == '(' )
                lineScore += 1;
            else if ( stack.top() == '[' )
                lineScore += 2;
            else if ( stack.top() == '{' )
                lineScore += 3;
            else if ( stack.top() == '<' )
                lineScore += 4;

            stack.pop();
        }
        autocompleteScores.push_back(lineScore);
    }

    std::sort(autocompleteScores.begin(), autocompleteScores.end());

    std::cout << autocompleteScores.size() << std::endl;

    return autocompleteScores.at(autocompleteScores.size() / 2);
}

int main(int argc, char* argv[])
{
    std::ifstream stream(argv[1]);

    if (!stream.is_open())
    {
        std::cerr << "Failed to open input file!" << std::endl;
        return 1;
    }

    std::vector<std::string> lines = readInput(stream);

    std::cout << computeAutocompleteScore(lines) << std::endl;

    return 0;
}
