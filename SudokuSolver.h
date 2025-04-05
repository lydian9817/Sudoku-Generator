#pragma once
#include <stdint.h>
#include <vector>
#include <stack>

struct Node
{
	const uint8_t row;
	const uint8_t col;
	const bool value;
	Node* top = nullptr;
	Node* bottom = nullptr;
	Node* left = nullptr;
	Node* right = nullptr;
	Node* colHeader = nullptr;
	bool header = false;

	Node(const Node&) = delete;
	Node(Node&&) = delete;
	Node operator=(const Node&) = delete;
	Node operator=(Node&&) = delete;
	~Node() = default;

	Node(uint8_t row, uint8_t col, bool value);
};

class SudokuSolver
{
	std::stack<Node>* solutions;
	std::stack<Node> matrix;

	SudokuSolver(const SudokuSolver&) = delete;
	SudokuSolver(SudokuSolver&&) = delete;
	SudokuSolver operator=(const SudokuSolver&) = delete;
	SudokuSolver operator=(SudokuSolver&&) = delete;
	~SudokuSolver() = default;

	void toExactCoverMatrix(std::vector<std::vector<char>>& board);
	size_t calculateMaxSz();

public:

	SudokuSolver(std::vector<std::vector<char>>& board);


};

