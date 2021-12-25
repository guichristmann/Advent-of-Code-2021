#include <iostream>
#include <string>
#include <fstream>

int main(int argc, char *argv[]){
    std::ifstream fileStream; 
    fileStream.open(argv[1]);

    if (!fileStream.is_open()){
        std::cout << "Failed to open file!" << std::endl;
        return -1;
    }

    int upCount = 0;
    std::string line;

    double currMeasurement, prevMeasurement;
    std::getline(fileStream, line);
    prevMeasurement = std::stod(line);

    while (std::getline(fileStream, line)){
        currMeasurement = std::stod(line);

        if (currMeasurement > prevMeasurement){
            upCount++;
        }

        prevMeasurement = currMeasurement;
    }

    fileStream.close();

    std::cout << "Up count is " << upCount << std::endl;

    return 0;
}
