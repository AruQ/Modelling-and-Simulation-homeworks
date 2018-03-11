#ifndef DRAWER_H
#define DRAWER_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <iostream>
using namespace std;

class Drawer
{
private:
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    int width;
    int height;
    int rect_width = -1;
    int rect_height = -1;
public:
    Drawer(int _width, int _height) :width(_width), height(_height){

        if(!al_init()) {
            fprintf(stderr, "failed to initialize allegro!\n");
            exit(0);
        }

        display = al_create_display(width, width);



        if(!display) {
            fprintf(stderr, "failed to create display!\n");
            exit(0);
        }
        if(!al_init_primitives_addon()) {
            fprintf(stderr, "failed to initialize primitives!\n");
            exit(0);
        }

        event_queue = al_create_event_queue();
        if(!event_queue) {
            fprintf(stderr, "failed to create event_queue!\n");

            exit(0);
        }
        al_register_event_source(event_queue, al_get_display_event_source(display));
    }

    ~Drawer ()
    {
        al_destroy_display(display);
        al_destroy_event_queue(event_queue);
    }

    bool draw(int** matrix, const int & nRows, const int & nCols)
    {
        if (rect_width == -1 && rect_height == -1)
        {
            rect_width= width/nCols;
            rect_height = height/nRows;
            if (rect_width ==0 || rect_height == 0)
            {
                fprintf(stderr, "too many cells\n");
                return false;
            }

        }

        ALLEGRO_EVENT ev;
        ALLEGRO_TIMEOUT timeout;
        al_init_timeout(&timeout, 0.02);
        bool get_event = al_wait_for_event_until(event_queue, &ev, &timeout);

        if(get_event && ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            return false;
        }


        al_clear_to_color(al_map_rgb(255,255,255));

        for (int i = 0; i < nRows; ++i) {
            for (int j = 0; j < nCols; ++j) {

                switch (matrix[i][j]) {
                case 0: //piante sane
                    al_draw_filled_rectangle(i*rect_width,j*rect_height,i*rect_width+rect_width,j*rect_height+rect_height,al_map_rgb(0,128,0));
                    break;
                case -1: //rocce
                    al_draw_filled_rectangle(i*rect_width,j*rect_height,i*rect_width+rect_width,j*rect_height+rect_height,al_map_rgb(105,105,105));
                    break;
                case -2: //piante guarite
                    al_draw_filled_rectangle(i*rect_width,j*rect_height,i*rect_width+rect_width,j*rect_height+rect_height,al_map_rgb(0,100,0));
                    break;
                default:
                    al_draw_filled_rectangle(i*rect_width,j*rect_height,i*rect_width+rect_width,j*rect_height+rect_height,al_map_rgb(139,0,0));
                    break;
                }


            }
        }

        al_flip_display();

//        al_rest(0.1);

        return true;
    }
};

#endif
