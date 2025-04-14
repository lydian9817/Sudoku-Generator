#include <iostream>
#include "SudokuSolver.h"

using namespace std;

static void printMatrix(std::vector<std::vector<Node>>& matrix)
{
	for (auto& row : matrix)
	{
		for (Node& n : row)
		{
			if (n.initialized)
				cout << '1';
			else
				cout << '0';
		}
		cout << endl;
	}
}

Node::Node(int row, int col, int value) :
	row{ row },
	col{ col },
	value{ value },
	initialized{ true } {
}

Node::Node()
{
}

void SudokuSolver::toExactCoverMatrix(std::vector<std::vector<char>>& board)
{
	for (size_t i = 0; i < board.size(); i++)
	{
		for (size_t j = 0; j < board.size(); j++)
		{
			//if (board[i][j] == ' ')
				//matrix.push(Node(i, j, true));
		}

	}
}

void SudokuSolver::initMatrix()
{
	for (int i = 0; i < size; i++)			//fila
	{
		for (int j = 0; j < size; j++)		//columna
		{
			for (int k = 0; k < size; k++)	//valores posibles por celda
			{
				int row = (i * colOffset + j * size + k);

				Node* rowNode, * colNode, * cellNode, * boxNode;

				matrix[row][rowOffset + (i + size + k)] = Node(i, j, k);
				matrix[row][colOffset + (j + size + k)] = Node(i, j, k);
				matrix[row][cellOffset + (i + size + j)] = Node(i, j, k);
				matrix[row][boxOffset + ((i / boxPerRow + j / boxPerCol * boxPerCol) * size + k)] = Node(i, j, k);


				rowNode = &matrix[row][rowOffset + (i + size + k)];
				colNode = &matrix[row][colOffset + (j + size + k)];
				cellNode = &matrix[row][cellOffset + (i + size + j)];
				boxNode = &matrix[row][boxOffset + ((i / boxPerRow + j / boxPerCol * boxPerCol) * size + k)];

				rowNode->right = colNode;
				rowNode->left = boxNode;
				colNode->left = rowNode;
				colNode->right = cellNode;
				cellNode->left = colNode;
				cellNode->right = boxNode;
				boxNode->left = cellNode;
				boxNode->right = rowNode;
			}
		}
	}

	printMatrix(matrix);
}

size_t SudokuSolver::calculateMaxSz()
{
	return size_t();
}

SudokuSolver::SudokuSolver(std::vector<std::vector<char>>& board, const int size, const int boxPerCol, const int boxPerRow) :
	maxRows{ static_cast<int>(pow(static_cast<double>(size), 3)) },
	maxCols{ static_cast<int>(pow(static_cast<double>(size), 2)) * 4 },
	size{ size },
	boxPerCol{ boxPerCol },
	boxPerRow{ boxPerRow },
	rowOffset{ 0 },
	colOffset{ static_cast<int>(pow(static_cast<double>(size), 2)) },
	cellOffset{ colOffset + colOffset },
	boxOffset{ cellOffset + colOffset }
{
	matrix.assign(maxRows, vector<Node>(maxCols, Node()));
	initMatrix();
	toExactCoverMatrix(board);
}
