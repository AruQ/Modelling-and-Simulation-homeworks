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
    int heightPanel = 50.0f;
    int colorBackground = makecol(6, 39, 170);



};

struct Image
{
    int image_x, image_y,image_w,image_h;
    BITMAP * bitmap;
};

//struct Image
//{
//    BITMAP * image;
//    int x,y,width;

//};

Drawer drawer;

////BITMAP * image;
//BITMAP * image_plus_resized;
//BITMAP * image_minus_resized;

Image image_plus;
Image image_minus;


int countSand1 = 0;
int countSand2 = 0;



const char *filename_plus = "../data/plus.tga";
const char *filename_minus = "../data/minus1.tga";

char iteration_text [4][100];

int textout_hack( BITMAP *bmp , FONT* font , int x , int y , double multiplier , char *msg , int color )
{
    BITMAP *tmp;

    tmp = create_bitmap( text_length(font, msg), text_height(font));

    if( !tmp)
        return FALSE;

    clear_to_color( tmp , drawer.colorBackground );
    textout_ex( tmp, font, msg, 0, 0, color, -1) ;
    //    textprintf_ex(tmp, font,0,0,color,-1,"itera 4 %i",4);

    masked_stretch_blit( tmp , bmp , 0 , 0 , tmp -> w , tmp -> h , x , y , (int)(tmp -> w * multiplier), (int)(tmp -> h * multiplier));

    destroy_bitmap( tmp );

    return TRUE;

}

bool drawList (int startingX, int y, double multiplier)
{

    int x = startingX;
    int increment = text_height(font);

    //    cout<< "0) "<<iteration_text[0]<<" increment "<<increment<<endl;
    //    cout<<"1) "<< iteration_text[1]<<endl;
    //    cout<<"2) "<< iteration_text[2]<<endl;


    textout_hack(drawer.buffer, font, x, y, multiplier, iteration_text[0], makecol(255, 255, 255));

    y += increment*2;
    textout_hack(drawer.buffer, font, x, y, multiplier, iteration_text[1], makecol(255, 255, 255));

    y += increment*2;
    textout_hack(drawer.buffer, font, x, y, multiplier, iteration_text[2], makecol(255, 255, 255));
}

bool loadImages ()
{
    BITMAP * image;

    //load the image file
    image = load_bitmap(filename_plus, NULL);


    image_plus.bitmap = create_bitmap(image_plus.image_w,image_plus.image_h);
    stretch_blit(image, image_plus.bitmap, 0, 0, image->w, image->h, 0, 0, image_plus.bitmap->w, image_plus.bitmap->h);
    destroy_bitmap(image);

    image = load_bitmap(filename_minus, NULL);


    image_minus.bitmap = create_bitmap(image_minus.image_w, image_minus.image_h);
    stretch_blit(image, image_minus.bitmap, 0, 0, image->w, image->h, 0, 0, image_minus.bitmap->w, image_minus.bitmap->h);

    destroy_bitmap(image);


}

//bool displayBitmap (BITMAP * buffer)
//{
//    BITMAP *bmp;
//    PALETTE palette;

//    bmp = load_bitmap("../data/plus.png", palette);
//    if (!bmp)
//        return false;
//    //        allegro_error("Couldn't load image.pcx!");

//    blit(bmp,buffer,0,0,0,0,10,10);
//    destroy_bitmap(bmp);
//    return true;
//}


bool initDrawer (int _width, int _height) {



    drawer.width = _width;
    drawer.height = _height+ drawer.heightPanel;

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


    image_plus.image_w = 40;
    image_plus.image_h = 40;
    image_minus.image_w= 40;
    image_minus.image_h= 40;

    image_minus.image_x = drawer.buffer->w /2;
    image_minus.image_y = drawer.buffer->h - drawer.heightPanel;

    image_plus.image_x = drawer.buffer->w - image_minus.image_w - 10;
    image_plus.image_y = drawer.buffer->h - drawer.heightPanel;

    loadImages();

    sprintf(iteration_text [0], "N Sand_1: %d  ", CA.countParticle[SAND_1]);
    sprintf(iteration_text [1], "N Sand_2: %d  ", CA.countParticle[SAND_2]);

    sprintf(iteration_text [3], "Forza: %.2f  ", CA.energy_total);







}

void destroyDisplay ()
{
    //    al_destroy_display(drawer.display);
    //    al_destroy_event_queue(drawer.event_queue);
}


bool pressed= false;
bool releaed_plus = false;
bool releaed_minus = false;

void draw(CALModel2D * model)
{


    /* On most platforms (eg. DOS) things will still work correctly
     * without this call, but it is a good idea to include it in any
     * programs that you want to be portable, because on some platforms
     * you may not be able to get any mouse input without it.
     */
    poll_mouse();

    /* you don't need to do this, but on some platforms (eg. Windows) things
      * will be drawn more quickly if you always acquire the screen before
      * trying to draw onto it.
      */
    acquire_screen();

    //    cout<< "mouse_x: "<<mouse_x<<" mouse_y: "<<mouse_y<<endl;

    if (mouse_x >= image_plus.image_x && mouse_x <= image_plus.image_x+ image_plus.image_w
            && mouse_y >= image_plus.image_y && mouse_y <= image_plus.image_y + image_plus.image_h)
    {
        if (mouse_b & 1)
        {
            pressed = true;

        }
        else
        {
            if (pressed)
            {
                releaed_plus = true;
                pressed = false;
            }

        }

    }

    if (mouse_x >= image_minus.image_x && mouse_x <= image_minus.image_x+ image_minus.image_w
            && mouse_y >= image_minus.image_y && mouse_y <= image_minus.image_y + image_minus.image_h )
    {

        if (mouse_b & 1)
        {
            pressed = true;

        }
        else
        {
            if (pressed)
            {
                releaed_minus = true;
                pressed = false;
            }

        }
    }



    if (releaed_plus)
    {
        incrementForce(model);
        releaed_plus = false;
        sprintf(iteration_text [3], "Forza: %.2f  ", CA.energy_total);
    }

    if (releaed_minus)
    {
        decrementForce(model);
        releaed_minus = false;
        sprintf(iteration_text [3], "Forza: %.2f  ", CA.energy_total);
    }


//    cout<<iteration_text[3]<<endl;




    clear_to_color(drawer.buffer, drawer.colorBackground);




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


    sprintf(iteration_text [2], "# iterazioni: %d  ", CA.simulation->step);

    //    textout_hack(drawer.buffer, font, 10, drawer.height-drawer.heightPanel, 2, iteration_text[2], makecol(255, 255, 255));
    drawList (10, drawer.height-drawer.heightPanel, 1.5 );

    textout_hack(drawer.buffer, font, image_minus.image_x+ image_minus.image_w +10 ,
                 drawer.height-drawer.heightPanel+10, 2, iteration_text[3], makecol(255, 255, 255));


    blit(image_plus.bitmap,drawer.buffer,0,0, image_plus.image_x, image_plus.image_y, image_plus.bitmap->w, image_plus.bitmap->h);

    blit(image_minus.bitmap, drawer.buffer,0,0, image_minus.image_x, image_minus.image_y, image_minus.bitmap->w, image_minus.bitmap->h);

    blit(drawer.buffer,screen,0,0,0,0,drawer.width,drawer.height);

    //    displayBitmap();



    /* you must always release bitmaps before calling any input functions */
    release_screen();

    /* wait for a key press */
    //    readkey();


}





#endif
