#include "Sudoku.h"


void printshit(SudokuCellState(&board)[9][9]);

bool Sudoku::init()
{
    if (!loadColors()) printf("There was an error loading the colors\n");
    fflush(stdout);
    
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) return EXIT_FAILURE;
	if (TTF_Init() != 0) return EXIT_FAILURE;
	IMG_Init(IMG_INIT_PNG);

	win = SDL_CreateWindow(
		"Sudoku",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		Constants::WINDOW_WIDTH,
		Constants::WINDOW_HEIGHT,
		0
	);
	if (win == NULL) return EXIT_FAILURE;

	ren = SDL_CreateRenderer(win, -1, 0);
	if (ren == NULL) return EXIT_FAILURE;

	bigFont = TTF_OpenFont("font.ttf", 35);
	miniFont = TTF_OpenFont("font.ttf", 16);

	// Initialize cells
	{
		int xPixelCount = Constants::WINDOW_PADDING;
		int yPixelCount = Constants::WINDOW_PADDING;

		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				sudokuBoard[i][j].bounds = {
					xPixelCount,
					yPixelCount,
					Constants::CELL_SIZE,
					Constants::CELL_SIZE
				};

				xPixelCount += (j == 2 || j == 5) ? Constants::THICK_LINE_WIDTH : Constants::THIN_LINE_WIDTH;
				xPixelCount += Constants::CELL_SIZE;
			}

			xPixelCount = Constants::WINDOW_PADDING;

			yPixelCount += (i == 2 || i == 5) ? Constants::THICK_LINE_WIDTH : Constants::THIN_LINE_WIDTH;
			yPixelCount += Constants::CELL_SIZE;
		}
	}

	srand(time(NULL));

	//SudokuCreator::fillDemo(sudokuBoard);
	SudokuCreator::fillNewSudoku(sudokuBoard);
	//printshit(sudokuBoard);

	running = true;

	return EXIT_SUCCESS;
}

void Sudoku::quit()
{

	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);

	TTF_Quit();
	SDL_Quit();

	return;
}

void Sudoku::handleEvents()
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:

		running = false;

		break;

	case SDL_MOUSEBUTTONDOWN:
		{
			checkCellsToUpdate();

			int x = event.motion.x;
			int y = event.motion.y;
		
			switch(event.button.button)
			{
			case SDL_BUTTON_LEFT:
            {
				SDL_Point cell = getIJ(x, y);

				if (cell.x < 9 && cell.x >= 0)
				{
					isCellSelected = true;
					selectedCell = cell;
				}

				break;
            }


			case SDL_BUTTON_RIGHT:
				isCellSelected = false;
				break;

			case SDL_BUTTON_MIDDLE:
			{
				SDL_Point cell = getIJ(x, y);
				if (cell.x == 10) break;
				
				if (!sudokuBoard[cell.y][cell.x].editable)
				{
					break;
				}

				isCellSelected = true;
				selectedCell = cell;

				SudokuCellState* c = &sudokuBoard[cell.y][cell.x];

				if (isShiftDown)
				{
					c->empty = true;
					c->number = 0;

					for (int i = 0; i < 9; i++)
					{
						c->posibleNumbers[i] = false;
					}
				}
				else
				{
					c->sure = !c->sure;
				}

				break;
			}

			default:
				break;
			}

			checkCellsToUpdate();
			updateCells();

			checkIfWin();
			if (winned) userWon();

		}

		break;

	case SDL_KEYDOWN:

		switch (event.key.keysym.sym)
		{
		case SDLK_RSHIFT:
		case SDLK_LSHIFT:
			isShiftDown = true;
			break;

		case SDLK_UP:
		case SDLK_w:
			if (isCellSelected && selectedCell.y > 0)
			{
				selectedCell.y--;
			
				checkCellsToUpdate();
				updateCells();
			}

			break;

		case SDLK_LEFT:
		case SDLK_a:
			if (isCellSelected && selectedCell.x > 0)
			{
				selectedCell.x--;

				checkCellsToUpdate();
				updateCells();
			}

			break;

		case SDLK_DOWN:
		case SDLK_s:
			if (isCellSelected && selectedCell.y < 8)
			{
				selectedCell.y++;

				checkCellsToUpdate();
				updateCells();
			}

			break;

		case SDLK_RIGHT:
		case SDLK_d:
			if (isCellSelected && selectedCell.x < 8)
			{
				selectedCell.x++;

				checkCellsToUpdate();
				updateCells();
			}

			break;
		
		case SDLK_LCTRL:
		case SDLK_RCTRL:
		{

			if (!isCellSelected) break;

			SDL_Point cell = selectedCell;

			if (!sudokuBoard[cell.y][cell.x].editable)
			{
				break;
			}

			SudokuCellState* c = &sudokuBoard[cell.y][cell.x];

			if (isShiftDown)
			{
				c->empty = true;
				c->number = 0;

				for (int i = 0; i < 9; i++)
				{
					c->posibleNumbers[i] = false;
				}
			}
			else
			{
				c->sure = !c->sure;
			}

		}
        checkIfWin();
        if (winned) userWon();

        checkCellsToUpdate();
        updateCells();
        break;


		case SDLK_1:
		case SDLK_KP_1:
		case SDLK_2:
		case SDLK_KP_2:
		case SDLK_3:
		case SDLK_KP_3:
		case SDLK_4:
		case SDLK_KP_4:
		case SDLK_5:
		case SDLK_KP_5:
		case SDLK_6:
		case SDLK_KP_6:
		case SDLK_7:
		case SDLK_KP_7:
		case SDLK_8:
		case SDLK_KP_8:
		case SDLK_9:
		case SDLK_KP_9:
		{
			SudokuCellState* cell = &sudokuBoard[selectedCell.y][selectedCell.x];

			if (winned) break;

			if (isCellSelected && cell->editable)
			{
				int num;

				switch (event.key.keysym.sym)
				{
				case SDLK_1:
				case SDLK_KP_1:
					num = 1;
					break;

				case SDLK_2:
				case SDLK_KP_2:
					num = 2;
					break;

				case SDLK_3:
				case SDLK_KP_3:
					num = 3;
					break;

				case SDLK_4:
				case SDLK_KP_4:
					num = 4;
					break;

				case SDLK_5:
				case SDLK_KP_5:
					num = 5;
					break;

				case SDLK_6:
				case SDLK_KP_6:
					num = 6;
					break;

				case SDLK_7:
				case SDLK_KP_7:
					num = 7;
					break;

				case SDLK_8:
				case SDLK_KP_8:
					num = 8;
					break;

				case SDLK_9:
				case SDLK_KP_9:
					num = 9;
					break;


				default:
					num = 0;
					break;
				}


				if (cell->sure)
				{
					if (cell->number == num) cell->number = 0;
					else cell->number = num;
				}
				else
				{
					cell->posibleNumbers[num - 1] = !cell->posibleNumbers[num - 1];
				}

				cell->empty = false;
				cell->shouldUpdate = true;
				updateCells();

				checkIfWin();
				if (winned) userWon();

			}

			break;
		}

		case SDLK_r:
			
			SudokuCreator::fillNewSudoku(sudokuBoard);
			isCellSelected = false;

			winned = false;

			drawGrid();
			drawNumbers();

			break;

		default:
			break;
		}

		break;

		drawGrid();
		drawNumbers();

		checkIfWin();
		if (winned) userWon();

	case SDL_KEYUP:
		if (event.key.keysym.sym == SDLK_LSHIFT || event.key.keysym.sym == SDLK_RSHIFT) isShiftDown = false;
		break;

	}
}

// GUI
SDL_Point Sudoku::getIJ(int x, int y)
{
	SDL_Point cell;
	bool handled = false;

	if (x < Constants::WINDOW_PADDING || x > Constants::WINDOW_WIDTH - Constants::WINDOW_PADDING) return {9, 9};
	if (y < Constants::WINDOW_PADDING || y > Constants::WINDOW_HEIGHT - Constants::WINDOW_PADDING) return {9, 9};

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			SDL_Rect b = sudokuBoard[i][j].bounds;

			if (x > b.x && x < b.x + b.w && y > b.y && y < b.y + b.h) 
			{
				cell = { j, i };
				return cell;
			}
		}
	}


	return { 10, 10 };
}


// COMPONENT DRAWING
void Sudoku::drawGrid()
{
	int wPadding = Constants::WINDOW_PADDING;
	int wWidth = Constants::WINDOW_WIDTH;
	int wHeight = Constants::WINDOW_HEIGHT;

	int cSize = Constants::CELL_SIZE;

	// Fill background
	SDL_SetRenderDrawColor(ren, Colors::background.r, Colors::background.g, Colors::background.b, 255);
	SDL_RenderClear(ren);

	// Draw borders
	{
		fillRect({ 0, 0, wWidth, wPadding }, Colors::padding);
		fillRect({ 0, wPadding, wPadding, wHeight - 2 * wPadding }, Colors::padding);
		fillRect({ wWidth - wPadding, wPadding, wPadding, wHeight - 2*wPadding }, Colors::padding);
		fillRect({ 0, wHeight - wPadding, wWidth, wPadding }, Colors::padding);
	}

	// Draw lines
	{
		int pixelCount = wPadding + cSize;
		int lineWidth;

        SDL_Rect thickLineRects[4];

		for (int i = 0; i < 8; i++)
		{
			if (i == 2 || i == 5)
			{
                // We store the rect to be filled, and fill it later so that thin lines dont overlap with thicker ones.
				lineWidth = Constants::THICK_LINE_WIDTH;
                if (i == 2)
                {
                    thickLineRects[0].x = pixelCount;
                    thickLineRects[0].y = wPadding;
                    thickLineRects[0].w = lineWidth;
                    thickLineRects[0].h = wHeight - 2 * wPadding;

                    thickLineRects[1].x = wPadding;
                    thickLineRects[1].y = pixelCount;
                    thickLineRects[1].w = wWidth - 2 * wPadding;
                    thickLineRects[1].h = lineWidth;
                }
                else
                {
                    thickLineRects[2].x = pixelCount;
                    thickLineRects[2].y = wPadding;
                    thickLineRects[2].w = lineWidth;
                    thickLineRects[2].h = wHeight - 2 * wPadding;

                    thickLineRects[3].x = wPadding;
                    thickLineRects[3].y = pixelCount;
                    thickLineRects[3].w = wWidth - 2 * wPadding;
                    thickLineRects[3].h = lineWidth;
                }
			}
			else
			{
				lineWidth = Constants::THIN_LINE_WIDTH;
				fillRect({ pixelCount, wPadding, lineWidth, wHeight - 2 * wPadding }, Colors::thinLine);
				fillRect({ wPadding, pixelCount, wWidth - 2 * wPadding, lineWidth }, Colors::thinLine);

			}

			pixelCount += lineWidth + cSize;

		}

        for (int i = 0; i < 4; i++) fillRect(thickLineRects[i], Colors::thickLine);

	}

	// Paint selected cells if there's one
	
	if (isCellSelected)
	{
		paintThisCell(selectedCell.x, selectedCell.y, Colors::selectedCell);
	}
}

void Sudoku::drawNumbers()
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			SudokuCellState currentCell = sudokuBoard[i][j];

			SDL_Color color;
			bool sameNumber = (currentCell.number == sudokuBoard[selectedCell.y][selectedCell.x].number);
			
			if (isCellSelected)
			{
				if (sameNumber)
				{
					if (currentCell.editable) color = Colors::sharingUserNumber;
					else color = Colors::sharingNumber;
				}
				else
				{
					if (currentCell.editable) color = Colors::userNumber;
					else color = Colors::number;
				}

			}
			else color = Colors::number;

			if (!currentCell.empty) 
			{
				if (currentCell.sure) { drawMainNumber(i, j, currentCell.number, color);}
				else drawMiniNumbers(i, j, currentCell.posibleNumbers);
			}
		}
	}

	return;
}

void Sudoku::checkCellsToUpdate()
{

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (i == selectedCell.y && j != selectedCell.x) sudokuBoard[i][j].shouldUpdate = true;
			if (j == selectedCell.x && i != selectedCell.y) sudokuBoard[i][j].shouldUpdate = true;

			if (selectedCell.x / 3 == j / 3 && selectedCell.y / 3 == i / 3) sudokuBoard[i][j].shouldUpdate = true;
		
			if (sudokuBoard[selectedCell.y][selectedCell.x].number == sudokuBoard[i][j].number) sudokuBoard[i][j].shouldUpdate = true;
		}
	}

	return;
}

void Sudoku::updateCells()
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			SudokuCellState cell = sudokuBoard[i][j];

			// If cell should be updated -> why & how?
			if (cell.shouldUpdate)
			{
				if (isCellSelected)
				{
					if (i == selectedCell.y && j == selectedCell.x) paintThisCell(j, i, Colors::selectedCell);
					else if (i == selectedCell.y && j != selectedCell.x) paintThisCell(j, i, Colors::indirectSelectedCell);
					else if (j == selectedCell.x && i != selectedCell.y) paintThisCell(j, i, Colors::indirectSelectedCell);
					else if (selectedCell.x / 3 == j / 3 && selectedCell.y / 3 == i / 3) paintThisCell(j, i, Colors::indirectSelectedCell);
					else paintThisCell(j, i, Colors::background);

					
				}
				else
				{
					paintThisCell(j, i, Colors::background);

				}


				if (!cell.empty)
				{

					if (cell.sure) 
					{
						SDL_Color color;
						bool sameNumber = (cell.number == sudokuBoard[selectedCell.y][selectedCell.x].number);
						
						if (sameNumber && isCellSelected)
						{
							if (cell.editable) color = Colors::sharingUserNumber;
							else color = Colors::sharingNumber;
						}
						else
						{
							if (cell.editable) color = Colors::userNumber;
							else color = Colors::number;
						}
						
						drawMainNumber(i, j, cell.number, color); 
					}
					else drawMiniNumbers(i, j, cell.posibleNumbers);
				}


				cell.shouldUpdate = false;
			}
		}
	}

	return;
}

// DRAWING

void Sudoku::fillRect(SDL_Rect rect, SDL_Color color)
{
	SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, 255);
	SDL_RenderFillRect(ren, &rect);

	return;
}

void Sudoku::paintThisCell(int x, int y, SDL_Color color) 
{
	if (x < 0 || y < 0) return;
	if (x > 8 || y > 8) return;

	fillRect(sudokuBoard[y][x].bounds, color);

	return;
}

void Sudoku::drawMiniNumbers(int i, int j, bool nums[9])
{
	if (j < 0 || i < 0) return;
	if (j > 8 || i > 8) return;

	SDL_Rect bounds = sudokuBoard[i][j].bounds;
	SDL_Color color = (sudokuBoard[i][j].editable) ? Colors::userNumber : Colors::number;


	if (nums[0]) printMiniNumber(bounds.x + 3, bounds.y - 4, 1, color);
	if (nums[1]) printMiniNumber(bounds.x + 16, bounds.y - 4, 2, color);
	if (nums[2]) printMiniNumber(bounds.x + 30, bounds.y - 4, 3, color);

	if (nums[3]) printMiniNumber(bounds.x + 3, bounds.y + 10, 4, color);
	if (nums[4]) printMiniNumber(bounds.x + 16, bounds.y + 10, 5, color);
	if (nums[5]) printMiniNumber(bounds.x + 30, bounds.y + 10, 6, color);

	if (nums[6]) printMiniNumber(bounds.x + 3, bounds.y + 22, 7, color);
	if (nums[7]) printMiniNumber(bounds.x + 16, bounds.y + 22, 8, color);
	if (nums[8]) printMiniNumber(bounds.x + 30, bounds.y + 22, 9, color);

	return;
}

// helper function to print nums quickly
void Sudoku::printMiniNumber(int x, int y, int num, SDL_Color color)
{
	char text[2];
	sprintf(text, "%i", num);

	SDL_Surface* textSurface = TTF_RenderText_Blended(miniFont, text, color);

	SDL_Rect rect = { x, y, textSurface->w, textSurface->h };

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(ren, textSurface);
	SDL_RenderCopy(ren, textTexture, NULL, &rect);

	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);

	return;
};

void Sudoku::drawMainNumber(int i, int j, int num, SDL_Color color)
{
	if (j < 0 || i < 0) return;
	if (j > 8 || i > 8) return;
	if (num < 1 || num > 9) { return; }

	char text[2];
	sprintf(text, "%i", num);

	SDL_Surface* textSurface = TTF_RenderText_Blended(bigFont, text, color);
	
	SDL_Rect bounds = sudokuBoard[i][j].bounds;

	SDL_Rect rect = {bounds.x + 13, bounds.y - 3, textSurface->w, textSurface->h};

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(ren, textSurface);
	SDL_RenderCopy(ren, textTexture, NULL, &rect);

	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);

	return;
}

void Sudoku::checkIfWin()
{
	// Check rows
	for (int i = 0; i < 9; i++) {
		bool used[10] = { false }; // initialize all values to false
		for (int j = 0; j < 9; j++) {
			int val = sudokuBoard[i][j].number;
			if (val != 0 && used[val]) {
				winned = false;
				return; // duplicate value found
			}
			used[val] = true;
		}
	}

	// Check columns
	for (int j = 0; j < 9; j++) {
		bool used[10] = { false }; // initialize all values to false
		for (int i = 0; i < 9; i++) {
			int val = sudokuBoard[i][j].number;
			if (val != 0 && used[val]) {
				winned = false; // duplicate value found
				return;
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
					int val = sudokuBoard[i][j].number;
					if (val != 0 && used[val]) {
						winned = false; // duplicate value found
						return;
					}
					used[val] = true;
				}
			}
		}
	}

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			SudokuCellState* c = &sudokuBoard[i][j];

			if (c->empty) 
			{ 
				winned = false; 
				return; 
			}

			if (c->number == 0)
			{
				winned = false;
				return;
			}
		}
	}

	winned = true;

	return;
}

void Sudoku::userWon()
{

	SDL_SetRenderDrawColor(ren, Colors::background.r, Colors::background.g, Colors::background.b, 255);
	SDL_RenderClear(ren);

	SDL_Color color = { 0, 0, 0 };
	TTF_Font* font = TTF_OpenFont("font.ttf", 35);

	SDL_Surface* textSurface = TTF_RenderText_Blended(bigFont, "Hay nieve rica. (ganaste!)", color);

	SDL_Rect rect = { 60, 80, textSurface->w, textSurface->h };

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(ren, textSurface);
	SDL_RenderCopy(ren, textTexture, NULL, &rect);

	
	textSurface = TTF_RenderText_Blended(bigFont, "\'r\' para otro sudoku...", color);

	rect = { 70, 350, textSurface->w, textSurface->h };

	textTexture = SDL_CreateTextureFromSurface(ren, textSurface);
	SDL_RenderCopy(ren, textTexture, NULL, &rect);

	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
	

	// Image
	SDL_Surface* surface = IMG_Load("dino.png");
	int w = surface->w;
	int h = surface->h;
	if (surface == NULL) std::cout << "There was an error loading the image";


	SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, surface);
	
	SDL_Rect* destination = new SDL_Rect;
	destination->x = - 10;
	destination->y = 150;
	destination->w = w;
	destination->h = h;

	SDL_RenderCopy(ren, texture, NULL, destination);

	delete destination;
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);

	return;
}
