#include <windows.h>
#include <thread>
#include <iostream>
#include <chrono>
//#include "sudoku.h"
#include "write_util.h"

using namespace std;
using namespace std::chrono;

void generateEasyBoards();
void generateNormalBoards();
void generateHardBoards();

int main()
{
	//SudokuGenerator generator(BoardSz::NINE, Difficulty::EASY, 15);
	/*string strBoard = "";
	thread t(&SudokuGenerator::generate, &generator);
	t.detach();
	steady_clock::time_point start = high_resolution_clock::now();
	steady_clock::time_point now;
	milliseconds millis;
	while (!generator.isComplete())
	{
		now = high_resolution_clock::now();
		millis = duration_cast<milliseconds>(now - start);

		cout << "\rTiempo: " << millis.count() << " millis       ";
		Sleep(0);
	}

	generator.printBoard(strBoard);
	cout << "\n" << strBoard.c_str() << endl;

	if (generator.checkSolution())
		cout << "Solucion correcta" << endl;
	else
		cout << "Solucion incorrecta" << endl;

	generator.deleteCellsToSolveBoard(55);
	cout << "Tablero a resolver: " << endl;

	generator.printBoard(strBoard);
	cout << "\n" << strBoard.c_str() << endl;
	*/

	generateEasyBoards();
	//generateNormalBoards();
	//generateHardBoards();

	cout << "Tableros generados" << endl;

	return 0;
}

void generateEasyBoards()
{
	string fileName = "easy_boards.json";
	SudokuGenerator generator(BoardSz::FOUR, Difficulty::EASY, 1);
	unordered_map<int, StrBoard>* bs = generator.getBoards();
	writeToFile(bs, fileName);
}
void generateNormalBoards()
{
	string fileName = "normal_boards.json";
	SudokuGenerator generator(BoardSz::NINE, Difficulty::NORMAL, 15);
	unordered_map<int, StrBoard>* bs = generator.getBoards();
	writeToFile(bs, fileName);
}
void generateHardBoards()
{
	string fileName = "hard_boards.json";
	SudokuGenerator generator(BoardSz::NINE, Difficulty::HARD, 15);
	unordered_map<int, StrBoard>* bs = generator.getBoards();
	writeToFile(bs, fileName);
}