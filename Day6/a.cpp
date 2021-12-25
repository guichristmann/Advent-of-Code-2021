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

void iterate(std::vector<int>& fishes) {
    int initialSize = fishes.size();
    for (int i = 0; i < initialSize; i++) {
        if (fishes.at(i) == 0) {
            fishes.at(i) = 6;
            fishes.push_back(8);
        } else {
            fishes.at(i)--;
        }
    }
}

int main(int argc, char* argv[]) {
    std::ifstream stream;

    stream.open(argv[1]);
    if (!stream.is_open()) {
        std::cerr << "Failed to open file." << std::endl;
    }

    std::vector<int> fishes { {1} };
    //fishes.push_back(3);
    //readInput(stream, fishes);

    for (int i = 0; i < 256; i++) {
        std::cout << "Simulating day " << i+1 << "." << std::endl;
        iterate(fishes);
        std::cout << "Total: " << fishes.size() << std::endl;
        for (auto f : fishes) {
            std::cout << f << ", ";
        }
        std::cout << std::endl;
        std::cin.ignore();
    }

    std::cout << fishes.size() << std::endl;

    return 0;
}
