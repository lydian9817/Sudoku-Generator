#pragma once

#include <stdint.h>
#include <vector>
#include <string>
#include <string_view>
#include <random>

const std::vector<char> FOUR_SZ_VALUES{ '1', '2', '3', '4' };
const std::vector<char> NINE_SZ_VALUES{ '1', '2', '3', '4', '5', '6', '7', '8', '9' };
const std::vector<char> SIXTEEN_SZ_VALUES{ '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G' };
constexpr char EMPTY{ ' ' };

enum class BoardSz
{
    FOUR = 4,
    NINE = 9,
    SIXTEEN = 16
};

struct cell
{
    uint8_t row = -1;
    uint8_t col = -1;

    void reset();
};

class SudokuGenerator
{
private:
    std::vector<char> values;
    std::vector<std::vector<char>> board;
    const int size;     // 9x9 por ejemplo -> size = 9
    const int boxSz;    // 9x9 por ejemplo -> boxSz = sqrt(size) = 3 
    std::random_device rd;
    std::mt19937 gen;
    cell conflictCell;
    std::atomic<bool> completed;
    bool checkNumberInRow(uint8_t n, uint8_t row, uint8_t col);
    bool checkNumberInCol(uint8_t n, uint8_t row, uint8_t col);
    bool checkNumberInSubBoard(uint8_t n, uint8_t row, uint8_t col);
    bool check(uint8_t n, uint8_t row, uint8_t col);
    int getRandomNumber(int min, int max);
    void getFirstEmptyCell(cell*);
    bool checkCompletion();
    void deleteFromCell(uint8_t row, uint8_t col);
    bool conflictBeforeCell(cell& target, cell& conflict);
    void setValues();
    char getRandomValueAndDelete();
    const int getStartingRowOrColIndex(const int rowOrCol) const;

public:
    SudokuGenerator(const SudokuGenerator&) = delete;
    SudokuGenerator(SudokuGenerator&&) = delete;
    SudokuGenerator operator=(const SudokuGenerator&) = delete;
    SudokuGenerator operator=(SudokuGenerator&&) = delete;
    ~SudokuGenerator() = default;

    SudokuGenerator(BoardSz sz);
    void printBoard(std::string& out);
    void generateSeed();
    void generate();
    bool isComplete();
    float getPercentage();
    bool checkSolution();
    void deleteCellsToSolveBoard(int amountOfCellsToDelete);
    const BoardSz boardSz;
};

