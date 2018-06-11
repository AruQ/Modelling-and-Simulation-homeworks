
#include "../include/SeaSandMotionCA.h"



using namespace std;
template <typename T>
string NumberToString ( T Number )
{
    ostringstream ss;
    ss << Number;
    return ss.str();
}


#include <math.h>



int main(int argc, char** argv)
{


    srand(time(NULL));

    CALModel2D* model;

    createModel(model);
//    initModel(model);
    CA.simulation = calRunDef2D(model, 1, CA.config.STEPS, CAL_UPDATE_IMPLICIT);


    calRunAddInitFunc2D(CA.simulation, initModel);

    calRunAddSteeringFunc2D(CA.simulation,printStateCells);

    calRun2D(CA.simulation);

//    printStateCells(model);



    return 0;
}

//-----------------------------------------------------------------------
