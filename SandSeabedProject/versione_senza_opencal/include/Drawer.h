#ifndef DRAWER_H
#define DRAWER_H

#include <stdio.h>
#include <iostream>

using namespace std;

#include "CellularAutomaton.h"

const inline int LINEAR_INDEX(int i, int j, int col)
{
    return i * col + j;
}

class Drawer
{
  private:
    BITMAP *buffer;
    int width;
    int height;
    int rect_width = -1;
    int rect_height = -1;

    // SandSeabed * model;
    Configuration *config;

  public:
    Drawer(Configuration *_config, int _width, int _height) : config(_config)
    {

        width = _width;
        height = _height;

        if (rect_width == -1 && rect_height == -1)
        {
            rect_width = width / config->columns;
            rect_height = height / config->rows;
            if (rect_width == 0 || rect_height == 0)
            {
                fprintf(stderr, "too many cells\n");
                return;
            }
        }

        allegro_init();
        install_keyboard();
        install_mouse();
        set_gfx_mode(GFX_AUTODETECT_WINDOWED, width, height, 0, 0);
        //Indica al computer la modalità grafica che intendiamo usare
        show_mouse(screen);
        //Mostra il mouse sullo schermo
        buffer = create_bitmap(width, height);
    }

    void destroyDisplay()
    {
        //    al_destroy_display(drawer.display);
        //    al_destroy_event_queue(drawer.event_queue);
    }

    void draw(CELL_STATE *matrix)
    {

        clear_to_color(buffer, makecol(27, 0, 201));

        for (int i = 0; i < config->rows; ++i)
        {
            for (int j = 0; j < config->columns; ++j)
            {
                CELL_STATE state = matrix[LINEAR_INDEX(i, j, config->columns)]; //(CELL_STATE)calGet2Di(model, CA.cellState, i, j);

                switch (state)
                {
                case SAND_1:
                    putpixel(buffer, j, i, makecol(254, 216, 19));
                    break;
                case SAND_2:
                    putpixel(buffer, j, i, makecol(254, 61, 4));
                    break;

                default:
                    break;
                }
            }
        }

        blit(buffer, screen, 0, 0, 0, 0, width, height);
    }
};

#endif