#pragma once

#include "SudokuCellState.h"
#include <random>
#include <iostream>
#include <algorithm>



class SudokuCreator
{
public:
	static void fillWithOnes(SudokuCellState(&board)[9][9]);
	static void fillWithAllMini(SudokuCellState(&board)[9][9]);
	static void fillNewSudoku(SudokuCellState(&board)[9][9]);
	static void fillDemo(SudokuCellState(&board)[9][9]);

private:
	static void permutateRows(SudokuCellState(&board)[9][9], int r1, int r2);
	static void permutateCols(SudokuCellState(&board)[9][9], int c1, int c2);

	static bool isSafe(SudokuCellState(&board)[9][9], int row, int col, int num);
	static bool solveSudoku(SudokuCellState(&board)[9][9]);
	static bool checkForSolutions(SudokuCellState(&board)[9][9]);


};

