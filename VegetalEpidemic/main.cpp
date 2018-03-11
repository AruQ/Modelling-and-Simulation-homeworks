#include "include/VegetalEpidemic.h"


int main(int argc, char **argv){
   
    Drawer drawer(1000, 1000);
    VegetalEpidemic vegetalEpidemic (500,500,1000,
                     "../data/matrix.conf",
                                 &drawer);

    vegetalEpidemic.run();



   return 0;
}
