#ifndef CONFIGURATION_H
#define CONFIGURATION_H


#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

enum ACTION{NO_CHANGE=0, ROTATE_LEFT, ROTATE_RIGHT};
enum CELL_STATE{WATER=0, SAND_1, SAND_2};
enum POSITION{LEFT_UP=0, RIGHT_UP, LEFT_DOWN, RIGHT_DOWN};


struct Configuration
{
    float density [3];
    int rows,columns;
    int x1, x2;
    int y1, y2;
    int STEPS;
    int force;

};




void printConfiguration (Configuration& config);

void readConfiguration (const string & fileName, Configuration& config)
{
    string line;
    int nLine=0;


    ifstream myfile ("../data/project.conf");
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            if (nLine==0)
            {
                istringstream iss(line);

                for (int n = 0; n < 3; ++n) {
                    iss >> config.density[n];

                }

            }


            else if (nLine==1)
            {
                istringstream iss(line);
                iss >> config.rows;
                iss >> config.columns;

            }

            else if (nLine==2)
            {
                istringstream iss(line);
                iss >> config.x1;
                iss >> config.x2;

            }

            else if (nLine==3)
            {
                istringstream iss(line);
                iss >> config.y1;
                iss >> config.y2;

            }

            else if (nLine==4)
            {
                istringstream iss(line);
                iss >> config.STEPS;
            }
            else if (nLine==5)
            {
                istringstream iss(line);
                iss >> config.force;
            }
            nLine++;

        }
        myfile.close();
    }


    printConfiguration(config);

}

void printConfiguration (Configuration& config)
{

    cout<<"____________________________________" <<endl;
    cout<<"Density: ";
    for (int i = 0; i < 3; ++i) {
        cout<<config.density[i]<<"  ";
    }
    cout<<endl;

    cout<<"Rows: "<<config.rows<<" Columns: "<<config.columns<<endl;

    cout<<"X1: "<<config.x1<<" X2: "<<config.x2<<endl;

    cout<<"Y1: "<<config.y1<<" Y2: "<<config.y2<<endl;
    cout<<"Steps:"<<config.STEPS<<endl;

    cout<<"____________________________________" <<endl;

}




#endif
