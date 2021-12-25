#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>

void readInput(std::ifstream& is, std::vector<int>& outInputs, int& min, int& max) {
    std::string line;
    std::getline(is, line);
    std::stringstream ss(line);

    int v;
    min = INT32_MAX;
    max = 0;
    while (ss >> v) {
        outInputs.push_back(v);

        if (v < min)
            min = v;
        if (v > max)
            max = v;

        if (ss.peek() == ',')
            ss.ignore();
    }
}

uint64_t seriesSum(uint64_t n) {
    return (n * (n + 1)) / 2;
}

uint64_t computeFuelUsage(const std::vector<int>& crabs, int pos) {
    uint64_t fuel = 0;

    for (auto c : crabs) {
        fuel += seriesSum(std::abs(c - pos));
    }

    return fuel;
}

int main(int argc, char* argv[]) {
    std::ifstream is;

    is.open(argv[1]);
    if (!is.is_open()) { 
        std::cerr << "Failed to open file." << std::endl;
    }

    //std::vector<int> crabs { {16, 1, 2, 0, 4, 2, 7, 1, 2, 14} };
    std::vector<int> crabs;
    int min, max;
    readInput(is, crabs, min, max);

    uint64_t minFuel = UINT64_MAX;
    
    for (int i = min; i < max; i++) {
        uint64_t fuelUsage = computeFuelUsage(crabs, i);
        if (fuelUsage < minFuel)
            minFuel = fuelUsage;
    }

    std::cout << minFuel << std::endl;


    return 0;
}
