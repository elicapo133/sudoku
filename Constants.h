#pragma once

#define NUMBER_OF_COLORS 10
#define FILE_COLORS "colors.dat"

namespace Constants
{
    const int WINDOW_PADDING = 20;
    const int THIN_LINE_WIDTH = 5;
    const int THICK_LINE_WIDTH = 10;

    const int CELL_SIZE = 40;

    const int WINDOW_WIDTH = 2 * WINDOW_PADDING + 9 * CELL_SIZE + 6 * THIN_LINE_WIDTH + 2 * THICK_LINE_WIDTH;
    const int WINDOW_HEIGHT = WINDOW_WIDTH;


};

namespace Colors
{
    SDL_Color background = { 245, 245, 220 };
    SDL_Color padding = { 30, 69, 62 };

    SDL_Color thinLine = { 24, 44, 37 };
    SDL_Color thickLine = { 24, 44, 37 };

    SDL_Color selectedCell = { 104, 187, 89 };
    SDL_Color indirectSelectedCell = { 196, 255, 150 };

    SDL_Color number = { 0, 0, 0 };
    SDL_Color userNumber = { 70, 70, 70 };
    SDL_Color sharingNumber = { 0, 70, 0 };
    SDL_Color sharingUserNumber = { 0, 70, 0 };
}

bool loadColors()
{
    FILE* f = fopen(FILE_COLORS, "r");
    if (f == NULL) 
    {
        printf("Could not open '%s' (colors).\n", FILE_COLORS);
        return false;
    }

    int i=0, r=0, g=0, b=0;
    int h = 0;
    char buff[100];
    char value_read = 0; // 0 for nothing, 1 rgb, 2 hex
    while(fgets(buff, 100, f) != NULL)
    {

        value_read = 0;

        if (buff[0] == '#')
        {
            if (sscanf(buff, "#%x", &h) == 1) value_read = 2;
        } 
        else if (sscanf(buff, "%d, %d, %d", &r, &g, &b) == 3) value_read = 1;
        
        if (value_read == 1 || value_read == 2)
        {

            SDL_Color* color;

            switch (i)
            {
                case 0: 
                    color = &Colors::background;
                    break;

                case 1: 
                    color = &Colors::padding;
                    break;

                case 2: 
                    color = &Colors::thinLine;
                    break;

                case 3: 
                    color = &Colors::thickLine;
                    break;

                case 4: 
                    color = &Colors::selectedCell;
                    break;

                case 5: 
                    color = &Colors::indirectSelectedCell;
                    break;

                case 6: 
                    color = &Colors::number;
                    break;

                case 7: 
                    color = &Colors::userNumber;
                    break;

                case 8: 
                    color = &Colors::sharingNumber;
                    break;

                case 9: 
                    color = &Colors::sharingUserNumber;
                    break;

                default: 
                    printf("No such color???\n");
                    break;
            }

            if (value_read == 1)
            {
                color->r = r;
                color->g = g;
                color->b = b;
                color->a = 255;
            }
            else if (value_read == 2)
            {
                color->r = (h & 0xff0000) >> 16;
                color->g = (h & 0x00ff00) >> 8;
                color->b = h & 0x0000ff;
                color->a = 255;
            }

            i++;
        }
    }

    if (i != NUMBER_OF_COLORS) return false;

    fclose(f);

    return true;

}
