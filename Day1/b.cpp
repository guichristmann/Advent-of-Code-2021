#include <iostream>
#include <string>
#include <fstream>
#include <vector>

void print_vector(const std::vector<double>& q){
    for (auto i : q ){
        std::cout << i << std::endl;
    }
    std::cout << "-------" << std::endl;
}

int sum_vector(const std::vector<double>& v){
    int result = 0;
    for (auto i : v){
        result += i;
    }

    return result;
}

int main(int argc, char *argv[]){
    std::ifstream fileStream; 
    fileStream.open(argv[1]);

    if (!fileStream.is_open()){
        std::cout << "Failed to open file!" << std::endl;
        return -1;
    }

    int upCount = 0;
    std::string line;

    std::vector<double> measurements;
    for(int i = 0; i < 3; i++){
        std::getline(fileStream, line);
        measurements.push_back(std::stod(line));
    }

    int prevSum = sum_vector(measurements);
    int currSum = 0;
    while (std::getline(fileStream, line)){
        measurements.push_back(std::stod(line));
        measurements.erase(measurements.begin());

        currSum = sum_vector(measurements);

        if (currSum > prevSum){
            upCount++;
        }

        prevSum = currSum;
    }

    fileStream.close();

    std::cout << "Up count is " << upCount << std::endl;

    return 0;
}
