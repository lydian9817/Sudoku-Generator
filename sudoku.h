#pragma once

#include <stdint.h>
#include <unordered_map>
#include <vector>
#include <string>
#include <string_view>
#include <random>

const std::vector<char> FOUR_SZ_VALUES{ '1', '2', '3', '4' };
const std::vector<char> NINE_SZ_VALUES{ '1', '2', '3', '4', '5', '6', '7', '8', '9' };
const std::vector<char> SIXTEEN_SZ_VALUES{ '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G' };
constexpr char EMPTY{ ' ' };

struct StrBoard
{
	std::string solved = "";
	std::string puzzle = "";

	explicit StrBoard(const int sz);
};

enum class BoardSz
{
	FOUR = 4,
	NINE = 9,
	SIXTEEN = 16
};

enum class Difficulty
{
	EASY,
	NORMAL,
	HARD
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

	std::unordered_map<int, StrBoard> generatedBoards;
	Difficulty difficulty;
	const size_t count;

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
	void boardToString(std::string& out);

public:
	SudokuGenerator(const SudokuGenerator&) = delete;
	SudokuGenerator(SudokuGenerator&&) = delete;
	SudokuGenerator operator=(const SudokuGenerator&) = delete;
	SudokuGenerator operator=(SudokuGenerator&&) = delete;
	~SudokuGenerator() = default;

	SudokuGenerator(BoardSz sz, Difficulty difficulty, const size_t count);
	void printBoard(std::string& out);
	void generateSeed();
	void generate();
	void generateBoards();
	bool isComplete();
	float getPercentage();
	bool checkSolution();
	void deleteCellsToSolveBoard();
	const BoardSz boardSz;
	std::unordered_map<int, StrBoard>* getBoards();
};

