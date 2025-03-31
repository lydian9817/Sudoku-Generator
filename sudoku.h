#pragma once

#include <stdint.h>
#include <vector>
#include <string>
#include <string_view>
#include <random>

using board_type = std::vector<std::vector<uint8_t>>;

struct cell
{
    uint8_t row = -1;
    uint8_t col = -1;

    void reset();
    bool hasData();
};

class SudokuGenerator
{
private:
    board_type board;
    int size; // 9x9 por ejemplo -> size = 9
    std::random_device rd;
    std::mt19937 gen;
    cell conflictCell;
    std::atomic<bool> completed;
    void initBoard();
    bool checkNumberInRow(uint8_t n, uint8_t row, uint8_t col);
    bool checkNumberInCol(uint8_t n, uint8_t row, uint8_t col);
    bool checkNumberInSubBoard(uint8_t n, uint8_t row, uint8_t col);
    bool check(uint8_t n, uint8_t row, uint8_t col);
    int getRandomNumber(int min, int max);
    void getFirstEmptyCell(cell*);
    bool checkCompletion();
    void deleteFromCell(uint8_t row, uint8_t col);
    bool conflictBeforeCell(cell& target, cell& conflict);

public:
    SudokuGenerator(const SudokuGenerator&) = delete;
    SudokuGenerator(SudokuGenerator&&) = delete;
    SudokuGenerator operator=(const SudokuGenerator&) = delete;
    SudokuGenerator operator=(SudokuGenerator&&) = delete;
    ~SudokuGenerator() = default;

    SudokuGenerator(int size);
    void printBoard(std::string& out);
    void generateSeed();
    void solve();
    bool isComplete();
    float getPercentage();
    bool checkSolution();
    void deleteCellsToSolveBoard(int amountOfCellsToDelete);
};

