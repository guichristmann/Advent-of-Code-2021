#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

uint32_t readInput(std::ifstream& is) {
    uint32_t uniqueDigitsCount = 0;
    std::string line;

    while (std::getline(is, line)) {
        std::cout << line << std::endl;

        std::string::size_type splitIndex = line.find('|');

        std::stringstream ss(line.substr(splitIndex + 2));

        std::string digit;
        while (ss >> digit) {
            if (digit.size() == 2 || digit.size() == 3 || digit.size() == 4 || digit.size() == 7)
                uniqueDigitsCount++;
        }
    }

    return uniqueDigitsCount;
}

int main(int argc, char* argv[]) {
    std::ifstream is;
    is.open(argv[1]);
    
    if ( !is.is_open() ) {
        std::cerr << "Failed to open file." << std::endl;
    }

    std::cout << readInput(is) << std::endl;

    
    return 0;
}
