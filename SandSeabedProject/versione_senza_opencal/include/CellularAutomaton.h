#ifndef CELLULARAUTOMATON_H
#define CELLULARAUTOMATON_H
#include <allegro.h>
#include "Configuration.h"
#include "Drawer.h"

#define MOD(a, b) ((((a) % (b)) + (b)) % (b))

// #define LINEAR_INDEX(i, j, col) ((i * col + j))

struct Neighbour
{
    int i;
    int j;
};

// const inline int LINEAR_INDEX(int i, int j, int col)
// {
//     return i * col + j;
// }

const int neighbourhoodSize = 8;
class SandSeabed
{

  private:
    Configuration config;
    CELL_STATE *buffer;
    int morgolusIndex;
    Neighbour X[neighbourhoodSize];
    bool changed = false;
    int step = 0;

    bool stop = false;

    Drawer drawer;

  public:
    SandSeabed() : config("./data/project.conf"), drawer(&config, 1000, 1000)
    {
        morgolusIndex = 0;
        buffer = new CELL_STATE[config.rows * config.columns];
        initNeighbourhood();
        initSubstate();
    }

    ~SandSeabed()
    {
        delete[] buffer;
    }

    void transition_function(int i, int j)
    {

        int sumWeights[2] = {0, 0};

        //    cout<<"indice di margolus "<<CA.morgolusIndex<<endl;

        // cout << "__________________________________________" << endl;
        // cout << "cella: (" << i << "," << j << ") " << config.rows << "    " << config.columns << "\n";

        int countNeighbouringCell = -1;
        for (int n = 0; n < neighbourhoodSize / 2; n++)
        {
            countNeighbouringCell++;

            if (j + X[morgolusIndex + n].j < 0 || j + X[morgolusIndex + n].j >= config.columns)
                return;

            if (i + X[morgolusIndex + n].i < 0 || i + X[morgolusIndex + n].i >= config.rows)
                return;

            // cout << "il vicino che stiamo considerando (" << i + X[morgolusIndex + n].i << "," << j + X[morgolusIndex + n].j << ") | " << endl;

            CELL_STATE state = buffer[LINEAR_INDEX(i + X[morgolusIndex + n].i, j + X[morgolusIndex + n].j, config.columns)];

            // cout << " LO STATO DELLA CELLA( " << i + X[morgolusIndex + n].i << " - " << j + X[morgolusIndex + n].j << ") "
            // <<"linearindex "<<LINEAR_INDEX(i + X[morgolusIndex + n].i, j + X[morgolusIndex + n].j, config.columns)<<" È " << state << endl;

            if (countNeighbouringCell < 2)
            {

                // cout<<" aggiungo al rigo di sopra "<<config.density[state] * config.weights[countNeighbouringCell]<<endl;

                sumWeights[0] += config.density[state] * config.weights[countNeighbouringCell];
            }

            if (countNeighbouringCell >= 2)
            {
                sumWeights[1] += config.density[state] * config.weights[countNeighbouringCell];
            }
        }

        // cout << sumWeights[0] << "   " << sumWeights[1] << " e morgolus? " << morgolusIndex << endl;
        if (sumWeights[0] > sumWeights[1])
        {
            changed = true;

            float p = (float)rand() / (float)RAND_MAX;
            if (p > config.probability_action[NO_CHANGE] && p <= config.probability_action[ROTATE_LEFT])
            {
                // cout << "ruoto left (" << i << "," << j << ")" << endl;

                rotate_left(i, j);

                //            printStateCells(model);
            }
            else if (p > config.probability_action[ROTATE_LEFT] && p <= config.probability_action[ROTATE_RIGHT])
            {
                // cout << "ruoto right (" << i << "," << j << ")" << endl;
                rotate_right(i, j);
                //            printStateCells(model);
            }
        }
    }

    void run()
    {

        // printStateCells();

        while (step < config.STEPS)
        {
            // cout << "STEP:" << step << endl;
            for (int i = 0; i < config.rows; i++)
            {
                for (int j = 0; j < config.columns; j++)
                {
                    transition_function(i, j);
                }
            }

            step++;

            // printStateCells();

            // if (step % 2 == 0)
            drawer.draw(buffer);

            update_indices();

            if (stop == true)
                break;
        }
    }
    void initNeighbourhood()
    {

        X[0].i = -1;
        X[0].j = -1;

        X[1].i = -1;
        X[1].j = 0;

        X[2].i = 0;
        X[2].j = 0;

        X[3].i = 0;
        X[3].j = -1;

        X[4].i = 0;
        X[4].j = 0;

        X[5].i = 0;
        X[5].j = 1;

        X[6].i = 1;
        X[6].j = 1;

        X[7].i = 1;
        X[7].j = 0;
    }

    void update_indices()
    {

        if (step % 2 != 0) //mettere come steering
        {
            morgolusIndex = 4;
        }
        else
        {
            morgolusIndex = 0; //mettere come campo del CA.
        }

        if (changed == false)
            stop = true;

        changed = false;
    }

    void rotate_right(int i, int j)
    {

        CELL_STATE tmpVector[4];
        //    int countNeighbouringCell = -1;
        for (int n = 0; n < neighbourhoodSize / 2; n++)
        {
            //        countNeighbouringCell++;
            tmpVector[n] = buffer[LINEAR_INDEX(i + X[morgolusIndex + n].i, j + X[morgolusIndex + n].j, config.columns)];
        }

        for (int n = 0; n < neighbourhoodSize / 2; ++n)
        {
            int newN = morgolusIndex + MOD((n + 1), neighbourhoodSize / 2);

            buffer[LINEAR_INDEX(i + X[newN].i, j + X[newN].j, config.columns)] = tmpVector[n];
        }
    }

    void rotate_left(int i, int j)
    {

        //    int countNeighbouringCell = -1;

        CELL_STATE tmpVector[4];

        //    cout<<CA.morgolusIndex<<endl;
        for (int n = neighbourhoodSize / 2 - 1; n >= 0; n--)
        {

            tmpVector[n] = buffer[LINEAR_INDEX(i + X[morgolusIndex + n].i, j + X[morgolusIndex + n].j, config.columns)];
        }
        for (int n = neighbourhoodSize / 2 - 1; n >= 0; n--)
        {
            int newN = morgolusIndex + MOD((n - 1), neighbourhoodSize / 2);

            buffer[LINEAR_INDEX(i + X[newN].i, j + X[newN].j, config.columns)] = tmpVector[n];
        }
    }

    void printStateCells()
    {
        for (int i = 0; i < config.rows; i++)
        {
            for (int j = 0; j < config.columns; ++j)
            {
                cout << buffer[LINEAR_INDEX(i, j, config.columns)] << "  ";
            }
            cout << endl;
        }

        cout << endl;
    }

    void initSubstate()
    {

        for (int i = 0; i < config.rows; i++)
        {
            for (int j = 0; j < config.columns; ++j)
            {

                buffer[LINEAR_INDEX(i, j, config.columns)] = WATER;

                if (i >= config.y1 && i < config.y2 && j >= config.x1 && j < config.x2)
                {
                    float p1 = (float)rand() / (float)RAND_MAX;
                    if (p1 > 0.3f)
                    {

                        float p = (float)rand() / (float)RAND_MAX;
                        if (p < 0.5f)
                            buffer[LINEAR_INDEX(i, j, config.columns)] = SAND_1;
                        else
                            buffer[LINEAR_INDEX(i, j, config.columns)] = SAND_2;
                    }
                }

                //            cout<<calGet2Di(model, CA.cellState, i,j)<<"  ";
            }

            //        cout<<endl;
        }
    }
};

#endif