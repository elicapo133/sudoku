#include "Sudoku.cpp"

#include <iostream>

/*
	Todo() : Make a more efficient method to get i,j from y, x. 
			manage num input to change values of a cell (sure value and posible values)
			program sudoku creator
			every time a sure value is changed, check if user won

*/

int main(int argc, char* argv[])
{
	
	Sudoku* sudoku = new Sudoku();
	sudoku->init();

	Uint64 nextFrame = SDL_GetTicks64() + 1000 / sudoku->FPS;
	
	sudoku->drawGrid();
	sudoku->drawNumbers();

	while (sudoku->isRunning())
	{

		if (nextFrame <= SDL_GetTicks64())
		{
			nextFrame = SDL_GetTicks64() + 1000 / sudoku->FPS;
		    sudoku->renderFrame();
		}

		sudoku->handleEvents();

	}

	sudoku->quit();
	delete sudoku;

	return 0;
}
