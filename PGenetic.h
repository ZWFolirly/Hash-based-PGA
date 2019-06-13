//
// Created by Administrator on 2019/6/6.
//

#ifndef PARALLELHGA_PGENETIC_H
#define PARALLELHGA_PGENETIC_H


#include <random>
#include "string"
#include "Graph.h"
#include "fstream"
#include "vector"
#include "cstdlib"
#include "config.h"
using namespace std;

typedef default_random_engine Random;
typedef uniform_real_distribution<double> rDistribution;
typedef uniform_int_distribution<int> iDistribution;
class PGenetic {
public:

    int seed = 1;
    Graph graph;
    double cross_rate;
    double mutation_rate;
    string filename;
    bool **population;
    int*fitness_degree;
    int best_fit = MAX_FIT;
    bool* best_gen;
    int generation = 0;
    int fitness_sum;
    time_t st;
    int column;
    int row;
    ofstream out;
    Random e;
    rDistribution u_1;
    iDistribution u_pop;
    iDistribution u_row;
    PGenetic(Graph _graph,string _filename);

    void showBest(int* better, int iter);

    void minEstimate(int t,bool *child);

    void run();

    void geneCrossover(int t);

    void geneMutation(int t);

    int max(int x, int y);

    int sumTrue(bool gen[]);

    int fitnessFun(bool gen[]);

    int* bestDegree(int degree[]);

    virtual bool** generateInitialize();

};


#endif //PARALLELHGA_PGENETIC_H
