#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <unordered_map>

struct DigitsData {
    std::vector<std::string> inputs;
    std::vector<std::string> outputs;
};

// Canonical mapping.
const std::unordered_map<std::string, std::string> canonicalMap = { 
    { "abcefg", "0" },
    { "cf", "1" },
    { "acdeg", "2" },
    { "acdfg", "3" },
    { "bcdf", "4" },
    { "abdfg", "5" },
    { "abdefg", "6" },
    { "acf", "7" },
    { "abcdefg", "8" },
    { "abcdfg", "9" }
};

std::vector<DigitsData> readInput(std::ifstream& is) {
    std::vector<DigitsData> digitsData;

    std::string line;
    std::string digit;
    while (std::getline(is, line)) {
        DigitsData dd;

        std::string::size_type splitIndex = line.find('|');
        std::stringstream ss(line.substr(0, splitIndex - 1));
        while (ss >> digit) {
            std::sort(digit.begin(), digit.end());
            dd.inputs.push_back(digit);
        }

        ss.clear();
        ss.str(line.substr(splitIndex + 2));
        while (ss >> digit) {
            std::sort(digit.begin(), digit.end());
            dd.outputs.push_back(digit);
        }

        digitsData.push_back(dd);
    }

    return digitsData;
}

std::string setDiff(const std::string& a, const std::string& b) {
    std::string result;

    if ( a.size() >= b.size() ) {
        std::set_difference(a.begin(), a.end(),
                            b.begin(), b.end(),
                            std::back_inserter(result));
    } else {
        std::set_difference(b.begin(), b.end(),
                            a.begin(), a.end(),
                            std::back_inserter(result));
    }

    return result;
}

std::unordered_map<char, char> findWireMap(std::vector<std::string>& displays) {
    std::unordered_map<char, char> wireMap;

    // Sort displays by the number of segments (string length).
    std::sort(displays.begin(), displays.end(), [] (const std::string& a, const std::string& b) {
        return a.size() < b.size();
    });

    //for (auto d : displays) {
    //    std::cout << d << std::endl;
    //}
    //std::cout << "------------------" << std::endl;

    // 1. Finding map to canonical 'a'.
    std::string aEq = setDiff(displays.at(1), displays.at(0));
    wireMap.emplace(std::make_pair(aEq.at(0), 'a'));

    // 2. Find canonical 'f' by finding the number 6.
    std::string fEq;
    std::string eightMinusOne = setDiff(displays.at(9), displays.at(0));
    for (auto itr = displays.begin() + 6; itr != displays.begin() + 9; itr++) {
        fEq = setDiff(*itr, eightMinusOne);
        if (fEq.size() == 1)
            break;
    }
    wireMap.emplace(std::make_pair(fEq.at(0), 'f'));
    // And its complement from number 1 will be canonical 'c'.
    std::string cEq = setDiff(fEq, displays.at(0));
    wireMap.emplace(std::make_pair(cEq.at(0), 'c'));

    // 3. Find canonical 'g' by finding the 9:
    std::string opString = displays.at(2);
    opString.append(aEq);
    std::sort(opString.begin(), opString.end());
    std::string gEq;
    for (auto itr = displays.begin() + 6; itr != displays.begin() + 9; itr++) {
        gEq = setDiff(*itr, opString); 
        if (gEq.size() == 1)
            break;
    }
    wireMap.emplace(std::make_pair(gEq.at(0), 'g'));

    // 4. Find canonical 'd' by finding the 3.
    opString = displays.at(0) + aEq + gEq;
    std::sort(opString.begin(), opString.end());
    std::string dEq;
    auto indexOfNumber3 = displays.begin();
    for (auto itr = displays.begin() + 3; itr != displays.begin() + 6; itr++) {
        indexOfNumber3 = itr;
        dEq = setDiff(*itr, opString);
        if (dEq.size() == 1)
            break;
    }
    wireMap.emplace(std::make_pair(dEq.at(0), 'd'));

    // Find bEq from number 5
    //std::cout << "3: " << *indexOfNumber3 << std::endl;
    std::string bEq;
    for (auto itr = displays.begin() + 3; itr != displays.begin() + 6; itr++) {
        if (itr == indexOfNumber3)
            continue; // Skip it;

        std::string res = setDiff(*indexOfNumber3, *itr);
        if (res == cEq) {
            //std::cout << "This is 5: " << *itr << std::endl;
            opString = aEq + dEq + fEq + gEq;
            std::sort(opString.begin(), opString.end());
            bEq = setDiff(*itr, opString);
            wireMap.emplace(std::make_pair(bEq.at(0), 'b'));
        } 
    }

    opString = aEq + bEq + cEq + dEq + fEq + gEq;
    std::sort(opString.begin(), opString.end());
    std::string eEq = setDiff(displays.at(9), opString);
    //std::cout << eEq << std::endl;
    wireMap.emplace(std::make_pair(eEq.at(0), 'e'));


    //std::cout << "### Result Mappings ###" << std::endl;
    //for (auto e : wireMap) {
    //    std::cout << e.first << " -> " << e.second << std::endl;
    //}
    //std::cin.ignore();

    return wireMap;
}

int convertOutputs(const std::unordered_map<char, char>& map, const std::vector<std::string>& outs) {
    std::string resultDigit;

    for (auto digit : outs) {
        std::string remapped;
        for (char c : digit) {
            auto itr = map.find(c);
            if (itr == map.end())
                std::cerr << "No mapping found, something went wrong!" << std::endl;
            remapped += itr->second;
        }

        std::sort(remapped.begin(), remapped.end());
        auto itr = canonicalMap.find(remapped);
        if (itr == canonicalMap.end()) 
            std::cerr << "No canonical map found, something went wrong!" << std::endl;

        resultDigit.append(itr->second);

    }
   
    //std::cout << std::stoi(resultDigit) << std::endl;
    return std::stoi(resultDigit);
}

int main(int argc, char* argv[]) {
    std::ifstream is;
    is.open(argv[1]);
    
    if ( !is.is_open() ) {
        std::cerr << "Failed to open file." << std::endl;
    }

    std::vector<DigitsData> data = readInput(is);

    int64_t accum = 0;
    for (auto d : data) {
        auto mappings = findWireMap(d.inputs);
        int convertedOuts = convertOutputs(mappings, d.outputs);

        accum += convertedOuts;
    }

    std::cout << "Result is: " << accum << std::endl;

    return 0;
}
