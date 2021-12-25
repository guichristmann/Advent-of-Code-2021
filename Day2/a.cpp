#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]){
    std::ifstream stream(argv[1]);

    if (!stream.is_open()){
        std::cerr << "Couldn't open file." << std::endl;
        return -1;
    }

    int horizontal = 0;
    int depth = 0;

    std::string line;
    while (std::getline(stream, line)){
        int amount = (int) *(line.end() - 1) - '0';
        if (*line.begin() == 'f'){
            horizontal += amount;
        } else if (*line.begin() == 'u') {
            depth -= amount; 
        } else if (*line.begin() == 'd'){
            depth += amount;
        }
    }

    std::cout << "Final result is " << horizontal * depth << std::endl;

    return 0;
}
