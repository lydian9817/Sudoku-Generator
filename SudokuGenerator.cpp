#include <windows.h>
#include <thread>
#include <iostream>
#include <chrono>
#include "sudoku.h"

using namespace std;
using namespace std::chrono;

int main()
{
    SudokuGenerator generator(BoardSz::NINE);
    string strBoard = "";
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

    return 0;
}
