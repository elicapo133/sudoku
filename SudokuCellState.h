#pragma once

#ifdef linux
    #include "SDL2/SDL.h"
#elif defined(_WIN32)
    #include "SDL.h"
#endif

// Stores the state of cell
class SudokuCellState
{
public:
	int number = 0;
	bool posibleNumbers[9] = { 0 };	// True if that number is marked as posible.

	bool editable = true;							// Is the cell editable by the user.
	bool sure = true;						// Is the cell in mini numbers mode or one number.
	bool selected = false;					// Is the cell currently selected.
	bool indirectlySelected = false;		// Is the cell indirectly selected.
	bool empty = true;						// Is the cell empty.

	bool shouldUpdate = false;

	SDL_Rect bounds;

};