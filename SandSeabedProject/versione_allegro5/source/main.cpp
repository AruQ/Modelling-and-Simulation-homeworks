
#include "../include/SeaSandMotionCA.h"
#include "../include/Drawer.h"



using namespace std;
template <typename T>
string NumberToString ( T Number )
{

    ostringstream ss;
    ss << Number;
    return ss.str();
}


#include <math.h>

void steering_function (CALModel2D * model)
{
    update_indices(model);

//    cout<<"STEP: "<<CA.simulation->step<<endl;
    if(CA.simulation->step%2 ==0)
        draw(model);
}



int main(int argc, char** argv)
{


    srand(time(NULL));

    CALModel2D* model;

    createModel(model);
    //    initModel(model);
    CA.simulation = calRunDef2D(model, 1, CA.config.STEPS, CAL_UPDATE_IMPLICIT);

    initDrawer(1000, 1000);

    calRunAddInitFunc2D(CA.simulation, initModel);

    //    calRunAddSteeringFunc2D(CA.simulation,printStateCells);

    calRunAddSteeringFunc2D(CA.simulation, steering_function);

    //    calRunAddGlobalTransitionFunc2D(CA.simulation, draw);

    calRunAddStopConditionFunc2D(CA.simulation, stopCondition);

    calRun2D(CA.simulation);

    draw (model);

    destroyDisplay();



//    printStateCells(model);



    return 0;
}

//-----------------------------------------------------------------------
