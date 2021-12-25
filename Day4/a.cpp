#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>

#define BOARD_SIZE 5

template <typename T>
void printVector(const std::vector<T>& v){
    for (auto i : v){
        std::cout << i << std::endl;
    }

    std::cout << "----------------------" << std::endl;
}

template <typename T>
void printBoard(const std::vector<std::vector<T>> board){
    for (auto row : board){
        for (auto v : row){
            std::cout << v << ", "; 
        }
        std::cout << std::endl;
    }

    std::cout << "----------------------" << std::endl;
}

void readInput(std::ifstream& fileStream, std::vector<int>& drawnNumbers, std::vector<std::vector<std::vector<int>>>& boards){
    std::string line;

    // Read lottery numbers.
    std::getline(fileStream, line);
    std::stringstream ss(line);
    for (int i; ss >> i;){
        if (ss.peek() == ',')
            ss.ignore();
        drawnNumbers.push_back(i);
    }

    // Skip empty line.
    std::getline(fileStream, line);

    while (fileStream.peek() != EOF){
        std::vector<std::vector<int>> currBoard;
        std::getline(fileStream, line);
        while (line != ""){
            std::vector<int> row;
            std::stringstream ss(line);

            for (int i; ss >> i;){
                if (ss.peek() == ' ')
                    ss.ignore();
                row.push_back(i);
            }

            currBoard.push_back(row);
            std::getline(fileStream, line);
        }

        boards.push_back(currBoard);
    }
}

bool checkForBingo(const std::vector<std::vector<bool>>& boolBoard){

    // Check for row bingo.
    for (int r = 0; r < BOARD_SIZE; r++){
        bool bingo = true;
        for (int c = 0; c < BOARD_SIZE; c++){
            if (!boolBoard.at(r).at(c)) {
                bingo = false;
                break;
            }
        }

        if (bingo)
            return true;
    }

    for (int c = 0; c < BOARD_SIZE; c++){
        bool bingo = true;
        for (int r = 0; r < BOARD_SIZE; r++){
            if (!boolBoard.at(r).at(c)) {
                bingo = false;
                break;
            }
        }

        if (bingo)
            return true;
    }

    return false;
}

int computeScore(const std::vector<std::vector<int>>& board, const std::vector<std::vector<bool>>& boolBoard,
        int& lastCalledNumber){
    int totalSum = 0;
    for (int r = 0; r < BOARD_SIZE; r++){
        for (int c = 0; c < BOARD_SIZE; c++){
            if (!boolBoard.at(r).at(c))
                totalSum += board.at(r).at(c);
        }
    }

    return totalSum * lastCalledNumber;
}

void computeBoardSuccess(const std::vector<std::vector<int>>& board, const std::vector<int>& drawnNumbers, 
        int& outWinRound, int& outScore){

    std::vector<std::vector<bool>> checkBoard { { false, false, false, false, false },
                                                { false, false, false, false, false },
                                                { false, false, false, false, false },
                                                { false, false, false, false, false },
                                                { false, false, false, false, false }};
    int currRound = 1;
    
    for (int number : drawnNumbers){
        bool newMatch = false;
        for (int r = 0; r < BOARD_SIZE; r++){
            for (int c = 0; c < BOARD_SIZE; c++){
                if (board.at(r).at(c) == number){
                    checkBoard.at(r).at(c) = true;
                    newMatch = true;
                    break;
                }
            }
            
            if (newMatch)
                break;
        }

        if (checkForBingo(checkBoard)){
            outWinRound = currRound;
            outScore = computeScore(board, checkBoard, number);
            return;
        }

        currRound++;
    }

    outScore = -1;
    outWinRound = -1;
}

int main(int argc, char* argv[]){
    std::ifstream stream;

    stream.open(argv[1]);
    if (!stream.is_open()){
        std::cerr << "Failed to open stream." << std::endl;
    }
    
    std::vector<int> drawnNumbers;
    std::vector<std::vector<std::vector<int>>> boards;

    readInput(stream, drawnNumbers, boards);
    std::cout << "Loaded " << drawnNumbers.size() << " numbers and " << boards.size() << " boards." << std::endl;

    int bestWin = 101, bestScore = -1, bestBoard = -1;
    int winRound, score, i;
    for (auto board : boards){
        computeBoardSuccess(board, drawnNumbers, winRound, score);

        if (winRound < bestWin){
            bestWin = winRound;
            bestScore = score;
            bestBoard = i;
        }

        i++;
    }

    std::cout << "Best win at round " << bestWin << " with a score of " << bestScore << " on board " << bestBoard << std::endl;

    return 0;
}
