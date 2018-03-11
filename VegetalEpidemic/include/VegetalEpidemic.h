#ifndef VEGETALEPIDEMIC_H
#define VEGETALEPIDEMIC_H


#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>
#include"Drawer.h"


template <typename T>
struct Pair {
    T first;
    T second;
};

class VegetalEpidemic
{
private:

    int ** matrix [2];
    int nRows;
    int nCols;
    int numberOfSteps;
    Drawer* drawer = NULL;


    void swapMatrices()
    {
        for (int i = 0; i < nRows; ++i) {
            for (int j = 0; j < nCols; ++j) {
                matrix[0][i][j] = matrix[1][i][j];
            }
        }
    }
    void init (const string & fileName)
    {
        Pair<int>* config =0;
        int numberOfObjects=-1;

        readConfiguration(fileName, config,numberOfObjects);

        srand((int)time(0));
        for (int i = 0; i < nRows; ++i) {
            for (int j = 0; j < nCols; ++j) {
                int n = (rand() % 100) + 1;


                for (int k = 0, percentage=0; k < numberOfObjects; ++k) {

                    if(k == 0 && n <= config[k].second || n >= percentage && n <= (percentage+config[k].second)){
                        matrix[0][i][j] = config[k].first;
                        matrix[1][i][j] = config[k].first;
                    }
                    percentage += config[k].second;

                }
            }
        }
        delete [] config;

    }

    void readConfiguration (const string & fileName, Pair<int>*& config, int & numberOfObjects)
    {
        string line;
        int nLine=0;


        ifstream myfile (fileName);
        if (myfile.is_open())
        {
            while ( getline (myfile,line) )
            {
                if (nLine==0)
                {
                    numberOfObjects = std::stoi(line);
                    config = new Pair<int>[numberOfObjects];
                }
                else
                {
                    istringstream iss(line);
                    iss >> config[nLine-1].first;
                    iss >> config[nLine-1].second;
                }
                nLine++;

            }
            myfile.close();
        }

    }

public:

    //-1 rocce
    //-2 pianta guarita
    //1-24 pianta infetta
    //0 pianta sana

    VegetalEpidemic (int _nRows, int _nCol, int _numberOfStep, string fileName, Drawer * _drawer):nRows(_nRows),
        nCols(_nCol), numberOfSteps(_numberOfStep), drawer(_drawer)
    {
        matrix[0] = new int*[nRows];
        matrix[1] = new int*[nRows];

        for (int i = 0; i < nRows; i++) {
            matrix[0][i] = new int[nCols];
            matrix[1][i] = new int[nCols];
        }
        init(fileName);
    }

    ~VegetalEpidemic()
    {
        for (int i = 0; i < nRows; ++i) {
            delete matrix[0][i];
            delete matrix[1][i];
        }
        delete [] matrix[0];
        delete [] matrix[1];
    }

    void transitionFunction(int i, int j)
    {
        if (matrix [0][i][j] == -1) //roccia
            return;
        if (matrix [0][i][j] == -2) //pianta guarita
            return;
        if (matrix [0][i][j] >=1 && matrix [0][i][j] <24){ //pianta ancora malata
            matrix[1][i][j] = matrix[0][i][j] +1;
            return;
        }
        if (matrix [0][i][j] == 24){ //pianta che deve guarire
            matrix[1][i][j] = -2;
            return;
        }


        //pianta ancora sana che potrebbe essere infettata
        for (int k = -1; k <= 1; k++)
            for (int z = -1; z <= 1; z++) {
                //controlliamo che la cella non sia fuori dalla matrice e che la cella che stiamo considerando non sia quella centrale
                if (k == 0 && z == 0 || i + k>=nRows || j + z>= nCols || j + z <0 || i + k< 0 )
                    continue;
                if (matrix[0][i + k][j + z] >0 && matrix[0][i + k][j + z] <=24)
                {
                    matrix[1][i][j] = 1;
                    return;
                }


            }

    }

    void run ()
    {

        drawer->draw(matrix[0],nCols, nRows);
        for (int n = 0; n < numberOfSteps; ++n) {
            for (int i = 0; i < nRows; ++i) {
                for (int j = 0; j < nCols; ++j) {
                    transitionFunction(i,j);
                }
            }
            swapMatrices();
            if(!drawer->draw(matrix[0],nCols, nRows))
                return;
        }
    }

    void print()
    {
        for (int i = 0; i < nRows; ++i) {
            for (int j = 0; j < nCols; ++j) {
                cout<<matrix[0][i][j]<<" ";
            }
            cout<<endl;
        }
        cout<<"__________________"<<endl;

    }



};
#endif
