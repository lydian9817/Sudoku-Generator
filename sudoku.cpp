#include <thread>
#include "sudoku.h"

using namespace std;

static atomic<bool> run = false;
static constexpr int MAX_CONFLICTS{ 500 };

SudokuGenerator::SudokuGenerator(BoardSz sz, Difficulty difficulty, const size_t count) :
gen{ rd() },
boardSz { sz },
difficulty{ difficulty },
count{ count },
size{ static_cast<int>(sz)},
boxSz{ static_cast<int>(sqrt(size)) },
completed{ false }
{
    setValues();
    generateBoards();
}

bool SudokuGenerator::checkNumberInRow(uint8_t n, uint8_t row, uint8_t col)
{
    for (int j = 0; j < size; j++)
    {
        if (j != col && board[row][j] == n)
        {
            conflictCell.row = row;
            conflictCell.col = j;
            return false;
        }
    }

    return true;
}

bool SudokuGenerator::checkNumberInCol(uint8_t n, uint8_t row, uint8_t col)
{
    for (int i = 0; i < size; i++)
    {
        if (i != row && board[i][col] == n)
        {
            conflictCell.row = i;
            conflictCell.col = col;
            return false;
        }
    }

    return true;
}

bool SudokuGenerator::checkNumberInSubBoard(uint8_t n, uint8_t row, uint8_t col)
{
    int rowStart = getStartingRowOrColIndex(row);
    int colStart = getStartingRowOrColIndex(col);

    for (int i = rowStart; i < rowStart + boxSz; i++)
    {
        for (int j = colStart; j < colStart + boxSz; j++)
        {
            if (i != row && j != col && board[i][j] == n)
            {
                conflictCell.row = i;
                conflictCell.col = j;
                return false;
            }
        }
    }

    return true;
}

bool SudokuGenerator::check(uint8_t n, uint8_t row, uint8_t col)
{
    if (!checkNumberInSubBoard(n, row, col))
        return false;
    if (!checkNumberInRow(n, row, col))
        return false;
    if (!checkNumberInCol(n, row, col))
        return false;
    return true;
}

int SudokuGenerator::getRandomNumber(int min, int max)
{
    uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

void SudokuGenerator::getFirstEmptyCell(cell*pC)
{
    bool found = false;
    for (int i = 0; i < size; i++)
    {
        if (found)
            break;
        for (int j = 0; j < size; j++)
        {
            if (board[i][j] == EMPTY)
            {
                pC->row = i;
                pC->col = j;
                found = true;
                break;
            }
        }
    }
}

bool SudokuGenerator::checkCompletion()
{
    for (auto& row : board)
    {
        for (char& c : row)
        {
            if (c == EMPTY)
            {
                completed = false;
                return false;
            }
        }
    }
    completed = true;
    return true;
}

void SudokuGenerator::deleteFromCell(uint8_t row, uint8_t col)
{
    for (int i = row; i < size; i++)
    {
        for (int j = col; j < size; j++)
        {
            board[i][j] = EMPTY;
        }
    }
}

bool SudokuGenerator::conflictBeforeCell(cell& target, cell& conflict)
{
    bool ret = false;

    if ((target.row > conflict.row) || (target.row == conflict.row && target.col > conflict.col))
        ret = true;

    return ret;
}

void SudokuGenerator::setValues()
{
    switch (boardSz)
    {
    case BoardSz::FOUR: values = FOUR_SZ_VALUES;
        break;
    case BoardSz::NINE: values = NINE_SZ_VALUES;
        break;
    case BoardSz::SIXTEEN: values = SIXTEEN_SZ_VALUES;
        break;
    }
}

char SudokuGenerator::getRandomValueAndDelete()
{
    char ret;
    int rnd = getRandomNumber(0, static_cast<int>(values.size()) - 1);
    ret = values[rnd];
    values.erase(values.begin() + rnd);
    return ret;
}

const int SudokuGenerator::getStartingRowOrColIndex(const int rowOrCol) const
{
    int i = rowOrCol;
    while (i % boxSz != 0)
        i--;

    return i;
}

void SudokuGenerator::boardToString(std::string& out)
{
    for (auto& row : board)
    {
        for (char& c : row)
            out += c;
    }
}

float SudokuGenerator::getPercentage()
{
    run = false;

    float counter = 0;
    for (auto& row : board)
    {
        for (char& n : row)
        {
            if (n != EMPTY)
                counter++;
        }
    }

    run = true;
    return counter * 100 / (size * size);
}

bool SudokuGenerator::checkSolution()
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (!check(board[i][j], i, j))
            {
                return false;
            }
        }
    }
    return true;
}

void SudokuGenerator::deleteCellsToSolveBoard()
{
    int amountOfCellsToDelete = 0;
    
    switch (difficulty)
    {
    case Difficulty::EASY: amountOfCellsToDelete = 40;
        break;
    case Difficulty::NORMAL: amountOfCellsToDelete = 55;
        break;
    case Difficulty::HARD: amountOfCellsToDelete = 65;
        break;
    }

    int counter = 0;
    int row = 0;
    int col = 0;

    while (counter < amountOfCellsToDelete)
    {
        row = getRandomNumber(0, size - 1);
        col = getRandomNumber(0, size - 1);
        if (board[row][col] != EMPTY)
        {
            board[row][col] = EMPTY;
            counter++;
        }
    }
}

std::unordered_map<int, StrBoard>* SudokuGenerator::getBoards()
{
    return &generatedBoards;
}

bool SudokuGenerator::isComplete()
{
    return completed;
}

void SudokuGenerator::generateSeed()
{
    int boxCount = 0;

    while (boxCount < boxSz)
    {
        setValues();
        for (int i = boxCount; i < boxCount + boxSz; i++)
        {
            for (int j = boxCount; j < boxCount + boxSz; j++)
                board[i][j] = getRandomValueAndDelete();
        }
        boxCount += boxSz + 1;
    }
}

void SudokuGenerator::generate()
{
    int conflicts = 0;
    run = true;
    completed = false;
    board.clear();
    board.assign(size, vector<char>(size, EMPTY));

    generateSeed();

    while (!completed)
    {
        if (!run)
            continue;

        setValues();
        char c = '\0';
        bool set = false;
        conflictCell.reset();
        cell cell;
        getFirstEmptyCell(&cell);
        if (cell.row != -1 && cell.col != -1)
        {
            while (!values.empty())
            {
                c = getRandomValueAndDelete();
                set = check(c, cell.row, cell.col);
                if (set)
                {
                    board[cell.row][cell.col] = c;
                    break;
                }
            }

            if (!set)
            {
                conflicts++;
                if (conflicts > MAX_CONFLICTS)
                {
                    deleteFromCell(0, 0);
                    generateSeed();
                }
                if (conflictBeforeCell(cell, conflictCell))
                    deleteFromCell(conflictCell.row, conflictCell.col);
                else
                {
                    board[cell.row][cell.col] = c;
                    board[conflictCell.row][conflictCell.col] = EMPTY;
                }
            }
        }
        checkCompletion();
    }
}

void SudokuGenerator::generateBoards()
{
    int i = 0;
    while (i < count)
    {
        generate();
        if (checkSolution())
        {
            StrBoard strB(size);

            boardToString(strB.solved);
            deleteCellsToSolveBoard();
            boardToString(strB.puzzle);

            generatedBoards.emplace(i, strB);
            i++;
        }
    }
}

void SudokuGenerator::printBoard(string& out)
{
    out.clear();

    for (auto& row : board)
    {
        for (char& n : row)
        {
            out += "[";
            out += n;
            out += "]";
        }
        out += "\n";
    }
}

void cell::reset()
{
    row = -1;
    col = -1;
}

StrBoard::StrBoard(const int sz)
{
    int size = sz * sz;
    solved.reserve(size);
    puzzle.reserve(size);
}
