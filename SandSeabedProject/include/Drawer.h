#ifndef DRAWER_H
#define DRAWER_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>
#include <stdio.h>
#include <iostream>

#include "SeaSandMotionCA.h"
using namespace std;


const ALLEGRO_COLOR COLOR_WATER = al_map_rgb(27, 0, 201);
const ALLEGRO_COLOR COLOR_SAND_1 = al_map_rgb(254, 216, 19);
const ALLEGRO_COLOR COLOR_SAND_2 = al_map_rgb(254, 61, 4);

struct Drawer
{
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
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

    if(!al_init()) {
        fprintf(stderr, "failed to initialize allegro!\n");
        exit(0);
    }

    drawer.display = al_create_display(drawer.width, drawer.width);



    if(!drawer.display) {
        fprintf(stderr, "failed to create display!\n");
        exit(0);
    }
    if(!al_init_primitives_addon()) {
        fprintf(stderr, "failed to initialize primitives!\n");
        exit(0);
    }

    drawer.event_queue = al_create_event_queue();
    if(!drawer.event_queue) {
        fprintf(stderr, "failed to create event_queue!\n");

        exit(0);
    }
    al_register_event_source(drawer.event_queue, al_get_display_event_source(drawer.display));
    return true;
}

void destroyDisplay ()
{
    al_destroy_display(drawer.display);
    al_destroy_event_queue(drawer.event_queue);
}

void draw(CALModel2D * model)
{



    ALLEGRO_EVENT ev;
    ALLEGRO_TIMEOUT timeout;
    al_init_timeout(&timeout, 0.02);
    bool get_event = al_wait_for_event_until(drawer.event_queue, &ev, &timeout);

    if(get_event && ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        CA.stop = true;
        return;
    }


    al_clear_to_color(al_map_rgb(27, 0, 201));

    for (int i = 0; i < CA.config.rows; ++i) {
        for (int j = 0; j < CA.config.columns; ++j) {
            CELL_STATE state = (CELL_STATE)calGet2Di(model, CA.cellState, i,j);

            switch (state) {
            case SAND_1: //piante sane
                al_draw_filled_rectangle(j*drawer.rect_width,i*drawer.rect_height,
                                         j*drawer.rect_width+drawer.rect_width,i*drawer.rect_height+drawer.rect_height,
                                        al_map_rgb(254, 216, 19));
                break;
            case SAND_2: //rocce
                al_draw_filled_rectangle(j*drawer.rect_width,i*drawer.rect_height,
                                         j*drawer.rect_width+drawer.rect_width,i*drawer.rect_height+drawer.rect_height,
                                         al_map_rgb(254, 61, 4) );
                break;
//            case -2: //piante guarite
//                al_draw_filled_rectangle(i*rect_width,j*rect_height,i*rect_width+rect_width,j*rect_height+rect_height,al_map_rgb(0,100,0));
//                break;
            default:
//                al_draw_filled_rectangle(i*rect_width,j*rect_height,i*rect_width+rect_width,j*rect_height+rect_height,al_map_rgb(139,0,0));
                break;
            }


        }
    }

    al_flip_display();


    if (CA.stop)
    {
        al_rest(1.50);
    }
    else
    {
        al_rest(0.0);
    }

}


#endif
