#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

void readInput(std::ifstream &is, std::vector<int>& fishes) {
    std::string line;
    std::getline(is, line);
    std::stringstream ss(line);

    int v;
    while (ss >> v) {
        fishes.push_back(v);

        if (ss.peek() == ',') {
            ss.ignore();
        }
    }
}

uint64_t accum(const std::vector<uint64_t>& v) {
    uint64_t accum = 0;

    for (auto e : v) {
        accum += e;
    }

    return accum;
}

int main(int argc, char* argv[]) {
    std::ifstream stream;

    stream.open(argv[1]);
    if (!stream.is_open()) {
        std::cerr << "Failed to open file." << std::endl;
    }

    //std::vector<int> initialFishes { {0, 1, 2, 3, 4, 5, 6, 7, 8} };
    std::vector<int> initialFishes;
    readInput(stream, initialFishes);

    std::vector<uint64_t> buckets { {0, 0, 0, 0, 0, 0, 0, 0, 0} };

    for (auto f : initialFishes) {
        buckets.at(f)++;
    }

    for (int day = 1; day <= 256; day++) {
        std::cout << "Day " << day << std::endl;

        uint64_t newFish = buckets.at(0);
        buckets.at(0) = buckets.at(1);
        buckets.at(1) = buckets.at(2);
        buckets.at(2) = buckets.at(3);
        buckets.at(3) = buckets.at(4);
        buckets.at(4) = buckets.at(5);
        buckets.at(5) = buckets.at(6);
        buckets.at(6) = buckets.at(7) + newFish; 
        buckets.at(7) = buckets.at(8);

        buckets.at(8) = newFish;
        for (auto b : buckets) {
            std::cout << b << ", ";
        }

        std::cout << "\nTotal: " << accum(buckets) << std::endl;
        //std::cin.ignore();
    }


    //std::cout << fishes.size() << std::endl;
    std::cout << std::endl;

    return 0;
}
