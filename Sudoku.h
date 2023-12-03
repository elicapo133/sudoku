#pragma once

#ifdef linux
    #include "SDL2/SDL.h"
    #include "SDL2/SDL_ttf.h"
    #include "SDL2/SDL_image.h"
#elif defined(_WIN32)
    #include "SDL.h"
    #include "SDL_ttf.h"
    #include "SDL_image.h"
#endif

#include <iostream>
#include <time.h>

#include "Constants.h"
#include "SudokuCellState.h"
#include "SudokuCreator.cpp"


// This is the main object handling the sudoku, GUI, etc
class Sudoku
{
public:
	// app
	bool init();
	void quit();
	bool isRunning() { return running; }
	
	void handleEvents();
	void renderFrame() { SDL_RenderPresent(ren); }

	// Component drawing
	void drawGrid();
	void drawNumbers();

	TTF_Font* miniFont;
	TTF_Font* bigFont;

	int FPS = 24;

	SDL_Window* win;
	SDL_Renderer* ren;
	void userWon();
	bool winned = false;

private:
	// app
	bool running = false;

	// game
	SudokuCellState sudokuBoard[9][9];
	void checkIfWin();

	// GUI
	bool isCellSelected = false;
	SDL_Point selectedCell = { 0, 0 };
	bool isShiftDown = false;
	SDL_Point getIJ(int x, int y);

	// Drawing help
	void fillRect(SDL_Rect rect, SDL_Color color);
	void drawMiniNumbers(int x, int y, bool nums[9]);
	void drawMainNumber(int x, int y, int num, SDL_Color color);

	void checkCellsToUpdate();
	void updateCells();

	void printMiniNumber(int x, int y, int num, SDL_Color color);

	void paintThisCell(int x, int y, SDL_Color color);

	// Sudoku creator
	

};


