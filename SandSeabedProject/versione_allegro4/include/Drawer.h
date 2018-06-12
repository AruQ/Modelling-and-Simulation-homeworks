#ifndef DRAWER_H
#define DRAWER_H

#include <stdio.h>
#include <iostream>

#include "SeaSandMotionCA.h"
using namespace std;

struct Drawer
{

    BITMAP *buffer;
    int width;
    int height;
    int rect_width = -1;
    int rect_height = -1;


};

Drawer drawer;


bool initDrawer (int _width, int _height) {

    drawer.width = _width;
    drawer.height = _height;

    if (drawer.rect_width == -1 && drawer.rect_height == -1)
    {
        drawer.rect_width= drawer.width/CA.config.columns;
        drawer.rect_height = drawer.height/CA.config.rows;
        if (drawer.rect_width ==0 || drawer.rect_height == 0)
        {
            fprintf(stderr, "too many cells\n");
            return false;
        }

    }

    allegro_init();
    install_keyboard();
    install_mouse();
    set_gfx_mode( GFX_AUTODETECT_WINDOWED, drawer.width, drawer.height, 0, 0);
    //Indica al computer la modalità grafica che intendiamo usare
    show_mouse(screen);
    //Mostra il mouse sullo schermo
    drawer.buffer = create_bitmap(drawer.width, drawer.height);


}

void destroyDisplay ()
{
    //    al_destroy_display(drawer.display);
    //    al_destroy_event_queue(drawer.event_queue);
}

void draw(CALModel2D * model)
{

    clear_to_color(drawer.buffer, makecol(27, 0, 201));

    for (int i = 0; i < CA.config.rows; ++i) {
        for (int j = 0; j < CA.config.columns; ++j) {
            CELL_STATE state = (CELL_STATE)calGet2Di(model, CA.cellState, i,j);

            switch (state) {
            case SAND_1: //piante sane
                putpixel(drawer.buffer, j, i, makecol(254, 216, 19));
                break;
            case SAND_2: //rocce
                putpixel(drawer.buffer, j,i, makecol(254, 61, 4) );
                break;

            default:
                break;
            }


        }
    }

    blit(drawer.buffer,screen,0,0,0,0,drawer.width,drawer.height);


}


#endif
