#include <thread>
#include "sudoku.h"

using namespace std;

static atomic<bool> run = false;
static constexpr int MAX_CONFLICTS{ 500 };

SudokuGenerator::SudokuGenerator(BoardSz sz) : 
gen{ rd() },
boardSz { sz },
size{ static_cast<int>(sz)},
boxSx{ static_cast<int>(sqrt(size)) },
completed{ false }
{
    initBoard();
}

void SudokuGenerator::initBoard()
{
    for (int i = 0; i < size; i++)
    {
        vector<uint8_t> v;
        board.push_back(v);
        for (int j = 0; j < size; j++)
            board[i].push_back(0);

        board[i].shrink_to_fit();
    }

    board.shrink_to_fit();
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
    uint8_t rowStart = 0;
    uint8_t colStart = 0;

    if (row < 3)
        rowStart = 0;
    else if (row < 6)
        rowStart = 3;
    else
        rowStart = 6;

    if (col < 3)
        colStart = 0;
    else if (col < 6)
        colStart = 3;
    else
        colStart = 6;

    for (int i = rowStart; i < rowStart + 3; i++)
    {
        for (int j = colStart; j < colStart + 3; j++)
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

void SudokuGenerator::getFirstEmptyCell(cell* pC)
{
    bool found = false;
    for (int i = 0; i < size; i++)
    {
        if (found)
            break;
        for (int j = 0; j < size; j++)
        {
            if (board[i][j] == 0)
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
        for (auto& n : row)
        {
            if (n == 0)
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
            board[i][j] = 0;
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

float SudokuGenerator::getPercentage()
{
    run = false;

    float counter = 0;
    for (auto& row : board)
    {
        for (auto& n : row)
        {
            if (n > 0)
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

void SudokuGenerator::deleteCellsToSolveBoard(int amountOfCellsToDelete)
{
    int counter = 0;
    int row = 0;
    int col = 0;

    while (counter < amountOfCellsToDelete)
    {
        row = getRandomNumber(0, 8);
        col = getRandomNumber(0, 8);
        if (board[row][col] > 0)
        {
            board[row][col] = 0;
            counter++;
        }
    }
}

bool SudokuGenerator::isComplete()
{
    return completed;
}

void SudokuGenerator::generateSeed()
{
    int index = 0;

    while (index < size)
    {
        vector<uint8_t> v{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        int max = 8;
        for (int i = index; i < index + 3; i++)
        {
            for (int j = index; j < index + 3; j++)
            {
                int rnd = getRandomNumber(0, max);
                board[i][j] = v[rnd];
                v.erase(v.begin() + rnd);
                max--;
            }
        }
        index += 3;
    }
}

void SudokuGenerator::solve()
{
    int conflicts = 0;
    run = true;
    generateSeed();

    while (!completed)
    {
        if (!run)
            continue;

        vector<uint8_t> v{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        int max = 8;
        uint8_t n = 0;
        bool set = false;
        conflictCell.reset();
        cell c;
        getFirstEmptyCell(&c);
        if (c.hasData())
        {
            while (!v.empty())
            {
                int rnd = getRandomNumber(0, max);
                n = v[rnd];
                v.erase(v.begin() + rnd);
                max--;

                set = check(n, c.row, c.col);
                if (set)
                {
                    board[c.row][c.col] = n;
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
                if (conflictBeforeCell(c, conflictCell))
                    deleteFromCell(conflictCell.row, conflictCell.col);
                else
                {
                    board[c.row][c.col] = n;
                    board[conflictCell.row][conflictCell.col] = 0;
                }
            }
        }
        checkCompletion();
    }
}

void SudokuGenerator::printBoard(string& out)
{
    out.clear();
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
            out += "[" + to_string(board[i][j]) + "]";

        out += "\n";
    }
}

void cell::reset()
{
    row = -1;
    col = -1;
}

bool cell::hasData() const
{
    return row != -1 && col != -1;
}
