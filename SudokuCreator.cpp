#include "SudokuCreator.h"

void printshit(SudokuCellState(&board)[9][9]);
bool checkSudoku(SudokuCellState(&board)[9][9]);
void fillDefault(SudokuCellState(&board)[9][9]);

int currentSudokuSolutionsCount = 0;

void SudokuCreator::fillWithOnes(SudokuCellState(&board)[9][9])
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			board[i][j].number = 1;
			board[i][j].empty = false;
			board[i][j].sure = true;
		}
	}

	return;
}

void SudokuCreator::fillNewSudoku(SudokuCellState(&board)[9][9])
{
	int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int n = sizeof(arr) / sizeof(arr[0]);

	fillDefault(board);

	// Use a random device to seed the random number generator
	std::random_device rd;
	std::mt19937 g(rd());

	// Shuffle the array using the random number generator
	std::shuffle(arr, arr + n, g);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			board[i * 3 + j / 3][i * 3 + j % 3].number = arr[j];
			board[i * 3 + j / 3][i * 3 + j % 3].empty = false;
			board[i * 3 + j / 3][i * 3 + j % 3].sure = true;
		}

		std::shuffle(arr, arr + n, g);

	}

	solveSudoku(board);
	
	int permutations = rand() % 10 + 10; // 10 - 20 permutations.

	int numberOfHints = rand() % 10 + 25;
	std::vector<SDL_Point> cellsTokeep;
	bool validCell;

	for (int i = 0; i < numberOfHints; i++)
	{
		SDL_Point currentCell;

		do 
		{
			validCell = true;
			currentCell = { rand() % 9, rand() % 9 };

			for (int j = 0; j < i; j++)
			{
				if (currentCell.x == cellsTokeep[j].x && currentCell.y == cellsTokeep[j].y)
				{
					validCell = false;
					break;
				}
			}

		} while (!validCell);
		
		cellsTokeep.push_back(currentCell);
		board[currentCell.y][currentCell.x].editable = false;

	}

	// SudokuCellState aux[9][9];

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (board[i][j].editable)
			{
				board[i][j].number = 0;
				board[i][j].empty = true;
			}

			board[i][j].sure = true;
			board[i][j].selected = false;
			board[i][j].indirectlySelected = true;
		
			//aux[i][j].number = board[i][j].number;
		}
	}



	//checkForSolutions(aux);
	//std::cout << currentSudokuSolutionsCount << std::endl;

	return;
}

bool SudokuCreator::solveSudoku(SudokuCellState(&board)[9][9])
{
	int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int n = sizeof(arr) / sizeof(arr[0]);
	std::random_device rd;
	std::mt19937 g(rd());

	int row, col;
	bool areThereEmpty = false;

	// Check every cell to know if there are any empty
	for (row = 0; row < 9; row++) 
	{
		for (col = 0; col < 9; col++) 
		{
			if (board[row][col].empty) 
			{
				areThereEmpty = true;
				break;
			}
		}

		if (areThereEmpty) break;

	}

	// If we finish the loop and there are no cells empty, return true, the board is finished
	if (!areThereEmpty) return true; 

	// If we encounter an empty cell
	std::shuffle(arr, arr + n, g);

	for (int index = 0; index < 9; index++) 
	{
		if (isSafe(board, row, col, arr[index]))
		{
			//std::cout << "It's safe" << std::endl;

			board[row][col].number = arr[index];
			board[row][col].empty = false;
			board[row][col].sure = true;

			// If we solved the sudoku, return true
			if (solveSudoku(board)) return true;

			// If we didn't, and were lefted without alternatives, this cell that caused troubles is now empty
			board[row][col].empty = true;
			board[row][col].number = 0;
		}
	}

	// If there are not any available number
	return false;

}

bool SudokuCreator::isSafe(SudokuCellState(&board)[9][9], int row, int col, int num)
{
	const int N = 9;

	// Check if the number is already in the same row or column
	for (int i = 0; i < N; i++) {
		if (board[row][i].number == num || board[i][col].number == num) {
			return false;
		}
	}

	// Check if the number is already in the same box
	int box_start_row = row - row % 3;
	int box_start_col = col - col % 3;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (board[box_start_row + i][box_start_col + j].number == num) {
				return false;
			}
		}
	}

	// If the number is not already in the same row, column, or box, it is valid
	return true;
}

void SudokuCreator::fillWithAllMini(SudokuCellState(&board)[9][9])
{

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			board[i][j].sure = false;
			board[i][j].empty = false;

			for (int k = 0; k < 9; k++)
			{
				board[i][j].posibleNumbers[k] = true;

			}
		}
	}

	return;
}

void SudokuCreator::fillDemo(SudokuCellState(&board)[9][9])
{
	int demoNumbers[9][9] = {
		{1, 2, 3, 9, 7, 8, 5, 6, 4},
		{4, 5, 6, 3, 1, 2, 8, 9, 7},
		{7, 8, 9, 6, 4, 5, 2, 3, 1},

		{9, 7, 8, 5, 6, 4, 1, 2, 3},
		{3, 1, 2, 8, 9, 7, 4, 5, 6},
		{6, 4, 5, 2, 3, 1, 7, 8, 9},

		{5, 6, 4, 1, 2, 3, 9, 7, 8},
		{8, 9, 7, 4, 5, 6, 3, 1, 2},
		{2, 3, 1, 7, 8, 9, 6, 4, 5}
	};

	for (int i = 0; i < 9; i++) 
	{
		for (int j = 0; j < 9; j++)
		{
			board[i][j].number = demoNumbers[i][j];
			board[i][j].sure = true;
			board[i][j].empty = false;
		}
	}


	return;
}

bool checkSudoku(SudokuCellState(&board)[9][9])
{
	// Check rows
	for (int i = 0; i < 9; i++) {
		bool used[10] = { false }; // initialize all values to false
		for (int j = 0; j < 9; j++) {
			int val = board[i][j].number;
			if (val != 0 && used[val]) {
				return false; // duplicate value found
			}
			used[val] = true;
		}
	}

	// Check columns
	for (int j = 0; j < 9; j++) {
		bool used[10] = { false }; // initialize all values to false
		for (int i = 0; i < 9; i++) {
			int val = board[i][j].number;
			if (val != 0 && used[val]) {
				return false; // duplicate value found
			}
			used[val] = true;
		}
	}

	// Check boxes
	for (int box_row = 0; box_row < 3; box_row++) {
		for (int box_col = 0; box_col < 3; box_col++) {
			bool used[10] = { false }; // initialize all values to false
			for (int i = box_row * 3; i < box_row * 3 + 3; i++) {
				for (int j = box_col * 3; j < box_col * 3 + 3; j++) {
					int val = board[i][j].number;
					if (val != 0 && used[val]) {
						return false; // duplicate value found
					}
					used[val] = true;
				}
			}
		}
	}

	// If we made it this far, the board is valid
	return true;
}

void SudokuCreator::permutateRows(SudokuCellState(&board)[9][9], int r1, int r2)
{
	if (r1 < 0 || r2 < 0) return;
	if (r1 > 8 || r2 > 8) return;
	if (r1 == r2) return;

	int aux;
	
	for (int j = 0; j < 9; j++)
	{
		aux = board[r1][j].number;
		board[r1][j].number = board[r2][j].number;
		board[r2][j].number = aux;
	}

	return;
}

void SudokuCreator::permutateCols(SudokuCellState(&board)[9][9], int c1, int c2)
{
	if (c1 < 0 || c2 < 0) return;
	if (c1 > 8 || c2 > 8) return;
	if (c1 == c2) return;

	int aux;

	for (int i = 0; i < 9; i++)
	{
		aux = board[i][c1].number;
		board[i][c1].number = board[i][c2].number;
		board[i][c2].number = aux;
	}

	return;
}

void printshit(SudokuCellState(&board)[9][9])
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			std::cout << board[i][j].number << "    ";
		}

		std::cout << std::endl;
	}

	return;
}

void fillDefault(SudokuCellState(&board)[9][9])
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			SudokuCellState* c = &board[i][j];

			c->number = 0;

			for (int k = 0; k < 9; k++)
			{
				c->posibleNumbers[k] = false;
			}

			c->editable = true;
			c->sure = true;
			c->selected = false;
			c->indirectlySelected = false;
			c->empty = true;
			c->shouldUpdate = false;
		}
	}
}

bool SudokuCreator::checkForSolutions(SudokuCellState(&board)[9][9])
{
	int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int n = sizeof(arr) / sizeof(arr[0]);

	int row, col;
	bool areThereEmpty = false;

	// Check every cell to know if there are any empty
	for (row = 0; row < 9; row++)
	{
		for (col = 0; col < 9; col++)
		{
			if (board[row][col].empty)
			{
				areThereEmpty = true;
				break;
			}
		}

		if (areThereEmpty) break;

	}

	// If we finish the loop and there are no cells empty, return true, the board is finished
	if (!areThereEmpty)
	{
		currentSudokuSolutionsCount++;
		
		if (currentSudokuSolutionsCount > 1) return true;
		else return false;
	}


	// If we encounter an empty cell
	for (int index = 0; index < 9; index++)
	{
		if (isSafe(board, row, col, arr[index]))
		{
			//std::cout << "It's safe" << std::endl;

			board[row][col].number = arr[index];
			board[row][col].empty = false;
			board[row][col].sure = true;

			if (checkForSolutions(board)) return true;

			// If we didn't, and were lefted without alternatives, this cell that caused troubles is now empty
			board[row][col].empty = true;
			board[row][col].number = 0;
		}
	}

	// If there are not any available number
	return false;

}