#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

enum ACTION
{
    NO_CHANGE = 0,
    ROTATE_LEFT,
    ROTATE_RIGHT
};
enum CELL_STATE
{
    WATER = 0,
    SAND_1,
    SAND_2
};
enum POSITION
{
    LEFT_UP = 0,
    RIGHT_UP,
    LEFT_DOWN,
    RIGHT_DOWN
};

class Configuration
{

  public:
    float weights[4]; //da leggere da file
    float probability_action[3];
    float density[3];
    int rows, columns;
    int x1, x2;
    int y1, y2;
    int STEPS;

    Configuration(const string &fileName)
    {
        readConfiguration(fileName);
        printConfiguration();
    }
    void printConfiguration()
    {

        cout << "____________________________________" << endl;
        cout << "Density: ";
        for (int i = 0; i < 3; ++i)
        {
            cout << density[i] << "  ";
        }
        cout << endl;

        cout << "Probability_action: ";
        for (int i = 0; i < 3; ++i)
        {
            cout << probability_action[i] << "  ";
        }
        cout << endl;

        cout << "Weights: ";
        for (int i = 0; i < 4; ++i)
        {
            cout << weights[i] << "  ";
        }
        cout << endl;

        cout << "Rows: " << rows << " Columns: " << columns << endl;

        cout << "X1: " << x1 << " X2: " << x2 << endl;

        cout << "Y1: " << y1 << " Y2: " << y2 << endl;
        cout<<"STEPS: "<<STEPS<<endl;

        cout << "____________________________________" << endl;
    }

    void readConfiguration(const string &fileName)
    {
        string line;
        int nLine = 0;
        

        ifstream myfile("./data/project.conf");
            
        
        if (myfile.is_open())
        {
     
            while (getline(myfile, line))
            {

                
                if (nLine == 0)
                {
                    istringstream iss(line);

                    for (int n = 0; n < 3; ++n)
                    {
                        iss >> density[n];
                    }
                }
                else if (nLine == 1)
                {
                    istringstream iss(line);

                    for (int n = 0; n < 3; ++n)
                    {
                        iss >> probability_action[n];
                    }
                }

                else if (nLine == 2)
                {
                    istringstream iss(line);

                    for (int n = 0; n < 4; ++n)
                    {
                        iss >> weights[n];
                    }
                }

                else if (nLine == 3)
                {
                    istringstream iss(line);
                    iss >> rows;
                    iss >> columns;
                }

                else if (nLine == 4)
                {
                    istringstream iss(line);
                    iss >> x1;
                    iss >> x2;
                }

                else if (nLine == 5)
                {
                    istringstream iss(line);
                    iss >> y1;
                    iss >> y2;
                }

                else if (nLine == 6)
                {
                    istringstream iss(line);
                    iss >> STEPS;
                }
                nLine++;
            }
            myfile.close();
        }
    }
};

#endif
