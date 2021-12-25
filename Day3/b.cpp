#include <iostream>
#include <bitset>
#include <fstream>
#include <array>
#include <vector>

bool mostCommonBit(const std::vector<std::bitset<12>>& bitVector, uint32_t i){
    int totalCount = 0;
    
    int trueCount = 0;
    for (const std::bitset<12>& bits : bitVector){
        trueCount += bits.test(i); 
        totalCount++;
    }

    return trueCount >= (totalCount / 2);
}

void printArray(const std::vector<std::bitset<12>>& v){
    for (auto i : v ){
        std::cout << i << std::endl;
    }
    std::cout << "---------------" << std::endl;
}


int main(int argc, char *argv[]){
    std::ifstream stream;
    stream.open(argv[1]);

    if (!stream.is_open()){
        std::cerr << "Failed to open file!" << std::endl;
        return -1;
    }

    std::vector<std::bitset<12>> oxygenCandidates;
    std::vector<std::bitset<12>> co2Candidates;
    int totalCount = 0;
    std::string line;
    while (std::getline(stream, line)){
        std::bitset<12> bits(line);
        oxygenCandidates.push_back(bits);
        co2Candidates.push_back(bits);
    }

    for (int i = 0; i < 12; i++){
        int t_bit = 12 - (i + 1);
        bool mostCommonOxygenBit = mostCommonBit(oxygenCandidates, t_bit);
        auto itr = oxygenCandidates.begin();
        while (itr != oxygenCandidates.end()){
            if ((*itr).test(t_bit) != mostCommonOxygenBit){
                itr = oxygenCandidates.erase(itr); 
            } else {
                itr++;
            }

            if (oxygenCandidates.size() == 1){
                break;
            }
        }

        printArray(oxygenCandidates);
        std::cin.ignore();

        if (oxygenCandidates.size() == 1){
            break;
        }
    }

    for (int i = 0; i < 12; i++){
        int t_bit = 12 - (i + 1);
        bool mostCommonCO2Bit = mostCommonBit(co2Candidates, t_bit);
        auto itr = co2Candidates.begin();
        while (itr != co2Candidates.end()){
            if ((*itr).test(t_bit) == mostCommonCO2Bit){
                itr = co2Candidates.erase(itr); 
            } else {
                itr++;
            }

            if (co2Candidates.size() == 1){
                break;
            }
        }

        if (co2Candidates.size() == 1){
            break;
        }
    }
    std::cout << oxygenCandidates.at(0) << " " << oxygenCandidates.size() << std::endl;
    std::cout << co2Candidates.at(0) << " " << co2Candidates.size() << std::endl;

    std::cout << "Result is " << oxygenCandidates.at(0).to_ulong() * co2Candidates.at(0).to_ulong() << std::endl;

    return 0;
}
