#ifndef SEASANDMOTION_H
#define SEASANDMOTION_H

extern "C"{
#include <OpenCAL/cal2DIO.h>
#include <OpenCAL/cal2D.h>
#include <OpenCAL/cal2DRun.h>
#include <OpenCAL/cal2DUnsafe.h>
}

#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>



#include <time.h>
#include <stdlib.h>
#include "Configuration.h"

#define MOD(a,b) ((((a)%(b))+(b))%(b))



//enum ACTION{NO_CHANGE=0, ROTATE_LEFT, ROTATE_RIGHT};
//enum CELL_STATE{WATER=0, SAND_1, SAND_2};

struct SeaSandMotionCA {
    //struct CALModel2D* model;
    struct CALSubstate2Di *cellState;
    struct CALRun2D* simulation;

    struct Configuration config;

    bool changed = false;
    bool stop = false;

    int morgolusIndex;

};
void initMargolusNeighborhood(CALModel2D* model);
void transition_function (CALModel2D* model , int i, int j);
void rotate_left (CALModel2D* model , int i, int j);
void rotate_right(CALModel2D* model , int i, int j);
void printStateCells (CALModel2D* model);
void initSubstate (CALModel2D* model);


SeaSandMotionCA CA;



void createModel (CALModel2D* & model)
{

    readConfiguration("../data/project.conf", CA.config);
    printConfiguration(CA.config);

    model = calCADef2D (CA.config.rows, CA.config.rows, CAL_CUSTOM_NEIGHBORHOOD_2D, CAL_SPACE_FLAT, CAL_OPT_ACTIVE_CELLS);
}

void initModel (CALModel2D* model)
{

    initMargolusNeighborhood(model);


    //INIZIALIZZAZIONE SOTTOSTATO
    //    CA.cellState = calAddSingleLayerSubstate2Di(model);
    CA.cellState = calAddSubstate2Di(model);
    initSubstate(model);
    CA.morgolusIndex = 4;

//    printStateCells(model);

    //    CA.weights[0] = 2.6f;
    //    CA.weights[1] = 2.6f;
    //    CA.weights[2]= 2.3f;
    //    CA.weights[3]= 2.3f;



    //    CA.probability_action [NO_CHANGE]=0.0f;
    //    CA.probability_action [ROTATE_LEFT]=0.5f;
    //    CA.probability_action [ROTATE_RIGHT]=1.0f;

    //    CA.density[WATER] = 0.0f;
    //    CA.density[SAND_1] = 1.0f;
    //    CA.density[SAND_2] = 3.0f;


    calAddElementaryProcess2D(model, transition_function);


}

void initSubstate (CALModel2D* model)
{


    calInitSubstate2Di(model, CA.cellState, WATER );
    for(int i = CA.config.y1; i<CA.config.y2; i++)
    {
        for (int j = CA.config.x1; j < CA.config.x2; ++j) {

            //            calSet2Di(model, CA.cellState, i,j,SAND_1);
            float p = (float)rand()/(float)RAND_MAX;
            if (p<0.5f)
                calSet2Di(model, CA.cellState, i,j,SAND_1);
            else
                calSet2Di(model, CA.cellState, i,j,SAND_2);


            //            cout<<calGet2Di(model, CA.cellState, i,j)<<"  ";

        }
        //        cout<<endl;
    }
}

void initMargolusNeighborhood(CALModel2D* model)
{

    calAddNeighbor2D(model, -1,-1);
    calAddNeighbor2D(model, -1, 0);
    calAddNeighbor2D(model, 0,0);
    calAddNeighbor2D(model, 0,-1);

    calAddNeighbor2D(model, 0,0);
    calAddNeighbor2D(model, 0,1);
    calAddNeighbor2D(model, 1,1);
    calAddNeighbor2D(model, 1,0);


}

void update_indices (CALModel2D* model)
{

    if (CA.simulation->step % 2 != 0) //mettere come steering
    {
        CA.morgolusIndex=4;
    }
    else
    {
        CA.morgolusIndex=0; //mettere come campo del CA.

    }

    if(CA.changed == false)
        CA.stop = true;

    CA.changed = false;

}

CALbyte stopCondition (CALModel2D * model)
{
    return CA.stop;
}

void transition_function (CALModel2D* model , int i, int j)
{


    //    if (CA.morgolusIndex == 4 && (i%2 != 0 || j%2 != 0))
    //    {
    //        return;
    //    }

    if (i%2 == 0 || j%2 == 0)
    {
        return;
    }

    int sumWeights [2] = {0,0};

    //        cout<<"indice di margolus "<<CA.morgolusIndex<<"    |     ";


    //    printStateCells(model);
    //    cout<<"cella: ("<<i<<","<<j<<") \n";

    int countNeighbouringCell = -1;
    for (int n = 0; n < model->sizeof_X/2; n++ )
    {
        countNeighbouringCell++;

        if (j+model->X[CA.morgolusIndex+n].j < 0 || j+model->X[CA.morgolusIndex+n].j >= model->columns)
            return;

        if (i+model->X[CA.morgolusIndex+n].i < 0 || i+model->X[CA.morgolusIndex+n].i >= model->rows)
            return;

        //        cout<<"il vicino che stiamo considerando ("<<i+model->X[CA.morgolusIndex+n].i <<","<<
        //              j+model->X[CA.morgolusIndex+n].j<<") | "<<endl;




        CELL_STATE state = (CELL_STATE)calGetX2Di(model, CA.cellState,i,j,CA.morgolusIndex+n);
        if(countNeighbouringCell < 2)
        {

            sumWeights[0] += CA.config.density[state] * CA.config.weights[countNeighbouringCell];
        }

        if(countNeighbouringCell >= 2)
        {
            sumWeights[1] += CA.config.density[state] * CA.config.weights[countNeighbouringCell];
        }







        //        std::cout<<"il vicino n="<<n<<" di "<<i<<" --- "<<j<<" è "<<calGetX2Di(model, CA.cellState,i,j,n)<<" || ";

        //        std::cout<<"con il pattern i="<<model->X[n].i<<" j= "<<model->X[n].j<<std::endl;
    }





    //    if (i ==4 && j ==4)
    //    {
    //        rotate_right(model,i,j);
    //    }

    CA.changed = true;
    if (sumWeights[0] > sumWeights[1] )
    {

//        cout<< sumWeights[0]<<"   "<<sumWeights[1]<< " e morgolus? "<< CA.morgolusIndex<<endl;
        float p = (float)rand()/(float)RAND_MAX;
        if(p > CA.config.probability_action[NO_CHANGE] &&  p<= CA.config.probability_action[ROTATE_LEFT])
        {
            //                        cout<<"ruoto left ("<<i<<","<<j<<")"<<endl;

            rotate_left(model,i,j);

            //                        printStateCells(model);
        }
        if(p > CA.config.probability_action[ROTATE_LEFT] &&  p<= CA.config.probability_action[ROTATE_RIGHT])
        {
            //                        cout<<"ruoto right ("<<i<<","<<j<<")"<<endl;
            rotate_right(model, i,j);
            //                        printStateCells(model);
        }
    }




}

void rotate_right (CALModel2D* model, int i, int j)
{

    CELL_STATE tmpVector[4];
    //    int countNeighbouringCell = -1;
    for (int n = 0; n < model->sizeof_X/2; n++ )
    {
        //        countNeighbouringCell++;
        tmpVector[n]= (CELL_STATE) calGetX2Di(model, CA.cellState,i,j,CA.morgolusIndex+n);

    }

    for (int n = 0; n < model->sizeof_X/2; ++n) {

        calSetX2Di(model, CA.cellState, i,j, CA.morgolusIndex+(MOD((n+1), model->sizeof_X/2)) , tmpVector[n]);
    }

}


void rotate_left (CALModel2D* model , int i, int j)
{

    //    int countNeighbouringCell = -1;

    int tmpVector[4];

    //    cout<<CA.morgolusIndex<<endl;
    for (int n = model->sizeof_X/2 -1 ; n >= 0; n-- )
    {

        tmpVector[n]= (CELL_STATE) calGetX2Di(model, CA.cellState,i,j,CA.morgolusIndex+n);


    }
    for (int n = model->sizeof_X/2 -1 ; n >= 0; n-- )
    {

        calSetX2Di(model, CA.cellState, i,j, CA.morgolusIndex+(MOD((n-1), (model->sizeof_X/2))) , tmpVector[n]);
    }



}

void printStateCells (CALModel2D* model)
{
    for(int i = 0; i<model->rows; i++)
    {
        for (int j = 0; j < model->columns; ++j) {
            cout<<calGet2Di(model, CA.cellState, i,j)<<"  ";

        }
        cout<<endl;
    }

    cout<<endl;
}

#endif
