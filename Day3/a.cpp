#include <iostream>
#include <bitset>
#include <fstream>
#include <array>

int main(int argc, char *argv[]){
    std::ifstream stream;
    stream.open(argv[1]);

    if (!stream.is_open()){
        std::cerr << "Failed to open file!" << std::endl;
        return -1;
    }

    std::array<int, 12> trueCount {};
    int totalCount = 0;
    std::string line;
    while (std::getline(stream, line)){
        std::bitset<12> bits(line);
        for (int i = 0; i < 12; i++){
            trueCount.at(i) += bits.test(i);
        }

        totalCount++;
    }

    std::bitset<12> gamma;
    for (int i = 0; i < 12; i++){
        if (trueCount.at(i) > totalCount / 2){
            gamma.set(i);
        }
    }

    uint32_t d_gamma = gamma.to_ulong();
    uint32_t d_epsilon = gamma.flip().to_ulong();

    std::cout << "Result is " << d_gamma * d_epsilon << std::endl;

    return 0;
}
