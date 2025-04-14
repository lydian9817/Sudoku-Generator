#pragma once
#include <stdint.h>
#include <vector>
#include <stack>

struct Node
{
	int row = -1;
	int col = -1;
	int value = -1;
	Node* top = nullptr;
	Node* bottom = nullptr;
	Node* left = nullptr;
	Node* right = nullptr;
	Node* colHeader = nullptr;
	bool header = false;
	bool initialized = false;

	//Node(const Node&) = delete;
	//Node(Node&&) = delete;
	//Node operator=(const Node&) = delete;
	//Node operator=(Node&&) = delete;
	//~Node() = default;

	Node(int row, int col, int value);
	Node();
};

class SudokuSolver
{
	std::stack<Node>* solutions;
	std::vector<std::vector<Node>> matrix;
	const int maxRows;
	const int maxCols;
	const int size;
	const int boxPerCol;
	const int boxPerRow;
	const int rowOffset;
	const int colOffset;
	const int cellOffset;
	const int boxOffset;
	

	void toExactCoverMatrix(std::vector<std::vector<char>>& board);
	void initMatrix();
	size_t calculateMaxSz();

public:

	SudokuSolver(const SudokuSolver&) = delete;
	SudokuSolver(SudokuSolver&&) = delete;
	SudokuSolver operator=(const SudokuSolver&) = delete;
	SudokuSolver operator=(SudokuSolver&&) = delete;
	~SudokuSolver() = default;

	SudokuSolver(std::vector<std::vector<char>>& board, const int size, const int boxPerCol, const int boxPerRow);


};

