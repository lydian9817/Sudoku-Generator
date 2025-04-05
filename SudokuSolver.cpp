#include "SudokuSolver.h"

Node::Node(uint8_t row, uint8_t col, bool value) :
	row{ row },
	col{ col },
	value{ value } {
}

void SudokuSolver::toExactCoverMatrix(std::vector<std::vector<char>>& board)
{
	int rowI = 0;
	int colI = 0;

	for (auto& row : board)
	{
		for (char& value : row)
		{

			colI++;
		}
		rowI++;
	}
}

size_t SudokuSolver::calculateMaxSz()
{
	return size_t();
}

SudokuSolver::SudokuSolver(std::vector<std::vector<char>>& board)
{
	toExactCoverMatrix(board);
}
