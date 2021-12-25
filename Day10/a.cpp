#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stack>

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

uint32_t computeSyntaxErrorScore(const std::vector<std::string>& lines)
{
    uint32_t accumScore = 0;

    for (const std::string& line : lines)
    {

        std::stack<char> stack;
        stack.push(line.at(0));
        if (stack.top() == ')' || stack.top() == ']' || stack.top() == '}' || stack.top() == '>')
            std::cerr << "Should handle this error case." << std::endl;

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
                if (line.at(i) == ')')
                    accumScore += 3;
                else if (line.at(i) == ']')
                    accumScore += 57;
                else if (line.at(i) == '}')
                    accumScore += 1197;
                else if (line.at(i) == '>')
                    accumScore += 25137;
                break;
            }
        }
    }

    return accumScore;
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

    std::cout << computeSyntaxErrorScore(lines) << std::endl;

    return 0;
}
