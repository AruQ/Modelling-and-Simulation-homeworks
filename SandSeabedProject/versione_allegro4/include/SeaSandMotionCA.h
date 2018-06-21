#ifndef SEASANDMOTION_H
#define SEASANDMOTION_H

#include <allegro.h>

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
#define MIN_VAL(a,b) ((a<b)? a:b)
#define ABS_VAL(a) ((a<0)? a*-1:a)



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
    
    int countParticle[3];
    double energy_total = 10.0;
    double thresholdEnergy = 2000.0;
    double deltaincrementEnergy = 10.0;
    double deltaincrementEnergyLessThan10 = 2.0;

    double energy_row_multiplicator;
    
};
void initMargolusNeighborhood(CALModel2D* model);
void transition_function (CALModel2D* model , int i, int j);
void rotate_left (CALModel2D* model , int i, int j);
void rotate_right(CALModel2D* model , int i, int j);

float getDeltaRotateLeft (CALModel2D* model, int i, int j);
float getDeltaRotateRight (CALModel2D* model, int i, int j);


void printStateCells (CALModel2D* model);
void initSubstate (CALModel2D* model);

ACTION getActionWithProbability (float probability_action[3]);




SeaSandMotionCA CA;

void incrementForce ();
void decrementForce ();



void createModel (CALModel2D* & model)
{
    
    readConfiguration("../data/project.conf", CA.config);
    printConfiguration(CA.config);
    
    model = calCADef2D (CA.config.rows, CA.config.rows, CAL_CUSTOM_NEIGHBORHOOD_2D, CAL_SPACE_FLAT, CAL_OPT_ACTIVE_CELLS);
}

void initModel (CALModel2D* model)
{
    
    initMargolusNeighborhood(model);
    
    CA.energy_row_multiplicator = (CA.energy_total/2) / model->rows;
    
    //INIZIALIZZAZIONE SOTTOSTATO
    CA.cellState = calAddSubstate2Di(model);
    initSubstate(model);
    CA.morgolusIndex = 4;
    
    
    calAddElementaryProcess2D(model, transition_function);
    
    
}

void initSubstate (CALModel2D* model)
{
    
    CA.countParticle[WATER]=-1;
    CA.countParticle[SAND_1]=0;
    CA.countParticle[SAND_2]=0;
    
    calInitSubstate2Di(model, CA.cellState, WATER );
    for(int i = CA.config.y1; i<CA.config.y2; i++)
    {
        for (int j = CA.config.x1; j < CA.config.x2; ++j) {
            
            float p = (float)rand()/(float)RAND_MAX;
            if(p < 0.5f)
            {
                
                p = (float)rand()/(float)RAND_MAX;
                if (p<0.5f)
                {
                    calSet2Di(model, CA.cellState, i,j,SAND_1);
                    CA.countParticle[SAND_1]++;
                }
                else
                {
                    
                    calSet2Di(model, CA.cellState, i,j,SAND_2);
                    CA.countParticle[SAND_2]++;
                }
            }
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
    


    //    cout<<"la riga è "<<i<<" moltiplicatore "<<local_energy<<endl;
    
    
    if (i%2 == 0 || j%2 == 0)
    {
        return;
    }
    
    
    
    //    cout<<"indice di margolus "<<CA.morgolusIndex<<endl;
    
    
    //    cout<<"cella: ("<<i<<","<<j<<") ";
    
    int countNeighbouringCell = -1;
    
    float sumWeights [2] = {0,0};
    
    bool onlyWater = true;


    //controllo dei bordi (se il vicinato è contenuto all'interno dello spazio cellulare non toroidale allora viene considerato il blocco)
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

        if (state != WATER)
        {
            onlyWater = false;
        }



        if(countNeighbouringCell < 2)
        {
            
            sumWeights[0] +=  CA.config.density[state];
        }
        
        if(countNeighbouringCell >= 2)
        {
            sumWeights[1] += CA.config.density[state];
        }
        
        
        
        
        
        
        
        //        std::cout<<"il vicino n="<<n<<" di "<<i<<" --- "<<j<<" è "<<calGetX2Di(model, CA.cellState,i,j,n)<<" || ";
        
        //        std::cout<<"con il pattern i="<<model->X[n].i<<" j= "<<model->X[n].j<<std::endl;
    }



    //non eseguire la funzione di transizione se tutte le celle sono acqua
    if (onlyWater)
        return;
    


    

    
    
    //    if (i ==4 && j ==4)
    //    {
    //        rotate_right(model,i,j);
    //    }
    

    float local_energy = CA.energy_total - i * CA.energy_row_multiplicator;

    float delta[3];
    delta [NO_CHANGE] = sumWeights[1] - sumWeights[0];
    delta[ROTATE_LEFT] = getDeltaRotateLeft(model, i,j);
    delta[ROTATE_RIGHT] = getDeltaRotateRight(model, i,j);

    CA.changed = true;
    float probability_action[3];

    if (local_energy == 0.0f)
    {

        float sum = 0.0f;
        float min_delta = MIN(delta[NO_CHANGE], delta[ROTATE_LEFT]);
        min_delta = MIN(min_delta, delta[ROTATE_RIGHT]);

        //        cout<<"min delta: "<< min_delta << endl;

        for (int i = 0; i < 3; ++i) {

            //            cout<<"delta prima["<<i<<"]="<<delta[i]<<endl;
            delta[i] =  delta[i] - min_delta;
            //            cout<<"delta dopo["<<i<<"]="<<delta[i]<<endl;

            sum+= delta[i];
        }

        for (int i = 0; i < 3; ++i) {
            probability_action [i] = delta[i] /sum;
            //            cout<< "prob ["<<i<<"]="<<probability_action[i]<<endl;
        }

    }
    else if (local_energy != 0.0f)
    {

        delta[NO_CHANGE] = ABS_VAL(delta[NO_CHANGE]) / local_energy;

        delta[ROTATE_LEFT] = delta[ROTATE_LEFT] + local_energy;
        delta[ROTATE_RIGHT] = delta[ROTATE_RIGHT] + local_energy;

        float sum =  delta[NO_CHANGE] + delta[ROTATE_LEFT] + delta[ROTATE_RIGHT];


        for (int i = 0; i < 3; ++i) {
            probability_action [i] = delta[i] /sum;
            //            cout<< "prob ["<<i<<"]="<<probability_action[i]<<endl;
        }

    }

    ACTION actionToPerform = getActionWithProbability(probability_action);
    //    cout<<"l'azione suggerita: "<<actionToPerform<<endl;

    if (actionToPerform == ROTATE_LEFT)
    {
        rotate_left(model,i,j);
    }

    if (actionToPerform == ROTATE_RIGHT)
    {
        rotate_right(model,i,j);
    }

    
    
    
    //    if (sumWeights[0] > sumWeights[1] )
    //    {
    //        CA.changed = true;

    //        //        cout<< sumWeights[0]<<"   "<<sumWeights[1]<< "e morgolus? "<< CA.morgolusIndex<<endl;
    //        float p = (float)rand()/(float)RAND_MAX;
    //        if(p > CA.config.probability_action[NO_CHANGE] &&  p<= CA.config.probability_action[ROTATE_LEFT])
    //        {
    //            //            cout<<"ruoto left ("<<i<<","<<j<<")"<<endl;

    //            rotate_left(model,i,j);

    //            //            printStateCells(model);
    //        }
    //        if(p > CA.config.probability_action[ROTATE_LEFT] &&  p<= CA.config.probability_action[ROTATE_RIGHT])
    //        {
    //            //            cout<<"ruoto right ("<<i<<","<<j<<")"<<endl;
    //            rotate_right(model, i,j);
    //            //            printStateCells(model);
    //        }
    //    }
    
    
    
    
}

ACTION getActionWithProbability (float probability_action[3])
{

    ACTION actions[3]= {NO_CHANGE, ROTATE_LEFT, ROTATE_RIGHT};


    for (int i= 0; i< 3; i++)
    {
        for (int j= 0; j< 3; j++)
        {
            if (probability_action[i] < probability_action[j])
            {
                float tmp = probability_action[i];
                probability_action[i] = probability_action[j];
                probability_action[j] = tmp;

                ACTION actionTmp = actions[i];
                actions[i] = actions[j];
                actions[j] = actionTmp;
            }
        }
    }

    probability_action[1] = probability_action[0] + probability_action[1];
    probability_action[2] = probability_action[1] + probability_action[2];


    //    for (int i = 0; i < 3; ++i) {
    //        cout << "prob_action ["<<i<<"]="<<probability_action[i]<<endl;
    //        cout<<"Lazione associata è "<<actions[i]<<endl;
    //    }


    float p = (float)rand()/(float)RAND_MAX;

    if (p < probability_action[0])
    {
        return actions[0];
    }

    if(p > probability_action[0] &&  p<= probability_action[1])
    {

        return actions[1];
    }
    if(p > probability_action[1] &&  p<= probability_action[2])
    {

        return actions[2];

    }




}

float getDeltaRotateLeft (CALModel2D* model, int i, int j)
{

    //    cout<<"-----"<<endl;
    float sumWeights [2] = {0,0};
    int countNeighbouringCell = -1;
    for (int n = 0; n < model->sizeof_X/2; n++ )
    {
        countNeighbouringCell++;

        CELL_STATE state = (CELL_STATE)calGetX2Di(model, CA.cellState, i,j, CA.morgolusIndex+(MOD((n+1), model->sizeof_X/2)));
        
        //        CELL_STATE state = (CELL_STATE) calGetX2Di(model, CA.cellState,i,j,CA.morgolusIndex+n);
        //        cout<<state<<endl;
        if(countNeighbouringCell < 2)
        {
            
            sumWeights[0] += CA.config.density[state];
        }
        
        if(countNeighbouringCell >= 2)
        {
            sumWeights[1] += CA.config.density[state];
        }
        //        tmpVector[n]= (CELL_STATE) calGetX2Di(model, CA.cellState,i,j,CA.morgolusIndex+n);
        
    }

    //    cout<<"rotate left => "<< sumWeights[0]<<"   "<<sumWeights[1]<< "e morgolus? "<< CA.morgolusIndex<<endl;
    return sumWeights[1]- sumWeights[0];
    
    
}

float getDeltaRotateRight (CALModel2D* model, int i, int j)
{
    //    cout<<"-----"<<endl;
    float sumWeights [2] = {0,0};
    int countNeighbouringCell = -1;
    //        printStateCells(model);
    for (int n = 0; n < model->sizeof_X/2; n++ )
    {
        countNeighbouringCell++;

        CELL_STATE state = (CELL_STATE)calGetX2Di(model, CA.cellState, i,j, CA.morgolusIndex+(MOD((n-1), (model->sizeof_X/2))));


        //        cout<<state<<endl;

        if(countNeighbouringCell < 2)
        {

            sumWeights[0] += CA.config.density[state];
        }

        if(countNeighbouringCell >= 2)
        {
            sumWeights[1] += CA.config.density[state];
        }


    }

    //    cout<<"rotate right => "<< sumWeights[0]<<"   "<<sumWeights[1]<< "e morgolus? "<< CA.morgolusIndex<<endl;
    return sumWeights[1]- sumWeights[0];


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


void incrementForce (CALModel2D* model)
{
    if (CA.energy_total < 10.0f)
    {
        CA.energy_total += CA.deltaincrementEnergyLessThan10;
    }
    else{
        CA.energy_total += CA.deltaincrementEnergy;
    }
    if (CA.energy_total > CA.thresholdEnergy)
    {
        CA.energy_total = CA.thresholdEnergy;
    }

    CA.energy_row_multiplicator = (CA.energy_total/2) / model->rows;
}

void decrementForce (CALModel2D* model)
{
    if (CA.energy_total <= 10.0f)
    {
        CA.energy_total -= CA.deltaincrementEnergyLessThan10;
    }
    else{
        CA.energy_total -= CA.deltaincrementEnergy;
    }


    if (CA.energy_total < 0)
    {
        CA.energy_total = 0;
    }
    CA.energy_row_multiplicator = (CA.energy_total/2) / model->rows;
    
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
