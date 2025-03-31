#include <windows.h>
#include <thread>
#include <iostream>
#include <chrono>
#include "sudoku.h"

using namespace std;
using namespace std::chrono;

int main()
{
    SudokuGenerator builder(BoardSz::NINE);
    string strBoard = "";
    thread t(&SudokuGenerator::solve, &builder);
    t.detach();
    steady_clock::time_point start = high_resolution_clock::now();
    steady_clock::time_point now;
    milliseconds millis;
    while (!builder.isComplete())
    {
        now = high_resolution_clock::now();
        millis = duration_cast<milliseconds>(now - start);

        cout << "\rTiempo: " << millis.count() << " millis       ";
        Sleep(0);
    }

    builder.printBoard(strBoard);
    cout << "\n" << strBoard.c_str() << endl;

    if (builder.checkSolution())
        cout << "Solucion correcta" << endl;
    else
        cout << "Solucion incorrecta" << endl;

    builder.deleteCellsToSolveBoard(55);
    cout << "Tablero a resolver: " << endl;

    builder.printBoard(strBoard);
    cout << "\n" << strBoard.c_str() << endl;

    return 0;
}
