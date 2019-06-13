//
// Created by Administrator on 2019/6/6.
//

#include "PHashGenetic.h"
#include <random>
#include <iostream>
#include "string"
#include "Graph.h"
#include "fstream"
#include "vector"
#include "cstdlib"
#include "ctime"
#include "config.h"
using namespace std;

typedef default_random_engine Random;
typedef uniform_real_distribution<double> rDistribution;
typedef uniform_int_distribution<int> iDistribution;

PHashGenetic::PHashGenetic(Graph _graph,string _filename) {
    graph.copy(_graph);
    column = graph.column;
    row = graph.row;
    seed = SEED;
    cross_rate = CROSS_RATE;
    mutation_rate = MUTATE_RATE;
    filename = _filename;
    fitness_degree = new int[POP_NUM];
    time(&st);
    fitness_sum = 0;
    generation = 0;
    population = generateInitialize();
    for (int j = 0; j < HASH_SIZE; ++j) {
        timestamp[j] = -1;
    }
    for (int i = 0; i < POP_NUM; i++) {
        fitness_degree[i] = fitnessFun(population[i]);
        fitness_sum += fitness_degree[i];
    }

    out.open(filename);
    e.seed(seed);
    u_1 = rDistribution(0.0, 1.0);
    u_pop = iDistribution(0, POP_NUM-1);
    u_row = iDistribution(0, row-1);
}

void PHashGenetic::showBest(int *better, int iter) {
    int min = MAX_FIT;
    int pos = 0;


    for (int i = 0; i < POP_NUM; ++i) {
        if (fitness_degree[i] < min) {
            min = fitness_degree[i];
            pos = i;
        }
    }

    cout<< "best:" << min << endl;
    for (int j = 0; j < row ; ++j) {
        cout << population[pos][j] << " ";

    }
    cout << endl;

}

void PHashGenetic::minEstimate(int t,bool *child) {
    int fit ;
    int n = 2;
    int sum = 0;
    fit = fitnessFun(child);
    sum += pow(fitness_degree[t], n);
    sum += pow(fit, n);
    iDistribution us(0,sum);
    if(us(e)>pow(fit, n)){
        delete[] population[t];
        population[t] = child;
        fitness_degree[t]=fit;
    }
    else
        delete[] child;
}

void PHashGenetic::run() {
    time_t now;
    for (generation = 0; generation < MAX_GEN; ++generation) {
        for (int j = 0; j < POP_NUM; ++j) {
            if (u_1(e) < cross_rate)
                geneCrossover(j);
            if (u_1(e) < mutation_rate)
                geneMutation(j);
        }
        time(&now);
        //cout<< best_fit << endl;
        if(generation%100==0)
            cout << "generation:" << generation << "\ttime costs:" << (now - st)  << "s\tbest fit:" << best_fit << "\taccuracy num:" << accuracy_rate<<"\thash size:"<<hash_size << "\n";
        out << "generation:" << generation << "\ttime costs:" << (now - st)  << "s\tbest fit:" << best_fit << "\taccuracy num:" << accuracy_rate<<"\thash size:"<<hash_size << "\n";
        //showBest(nullptr, 0);
    }
    time(&now);
    cout<< "generation:" << generation << "\ttime costs:" << (now - st)  << "s\tbest fit:" << best_fit << "\taccuracy num:" << accuracy_rate<<"\thash size:"<<hash_size << "\n";
    return;
}

void PHashGenetic::geneCrossover(int t) {
    int yy = u_pop(e);
    bool *x = population[t], *y = population[yy];
    bool* child1 = new bool[row];
    bool* child2 = new bool[row];
    int cut = u_row(e);
    for (int i = 0; i < cut; i++) {
        child1[i] = x[i];
        child2[i] = y[i];
    }
    for (int i = cut; i < row; i++) {
        child1[i] = y[i];
        child2[i] = x[i];
    }
    if(u_1(e)<0.5){
        delete[] child2;
        minEstimate(t,child1);
    }
    else{
        delete[] child1;
        minEstimate(t,child2);
    }
}

void PHashGenetic::geneMutation(int t) {
    bool *child = new bool[row];
    int cut = u_row(e);
    for (int i = 0; i < row; ++i) {
        child[i] = population[t][i];
    }
    child[cut] = !child[cut];
    minEstimate(t, child);

}

int PHashGenetic::max(int x, int y) {
    return x>y?x:y;
}

int PHashGenetic::sumTrue(bool gen[]) {
    int sum=0;
    for (int i = 0; i < row; ++i) {
        sum+=gen[i]?1:0;
    }
    return sum;
}
int PHashGenetic::hashFun(bool gen[]){
    int64_t sum = 0;

    int64_t t = 0;
    iDistribution u(0,HASH_SIZE-1);

    Random te(0);

    for (int i = 0; i < row; ++i) {
        if(i%32==0){
            te.seed(t+u(te));
            t=0;
        }

        t<<1;
        if(gen[i])
            t+=1;
    }
    te.seed(t+u(te));
    return u(te);
}
int PHashGenetic::fitnessFun(bool gen[]) {
    int pos = hashFun(gen);
    if(timestamp[pos] != -1 and generation-timestamp[pos]<MAX_GEN/100){
        ++accuracy_rate;
        return hash[pos];
    }


    int fit = 0;
    int j;
    for (int i = 0; i < column; ++i) {
        for (j = 0; j < row; ++j)
            if(gen[j] && graph.map[i][j])
                break;
        if(j==row)
            fit+=5;
    }
    fit += sumTrue(gen);
    if(timestamp[pos]==-1)
        ++hash_size;
    timestamp[pos] = generation;
    hash[pos] = fit;
    if(fit<best_fit){
        best_fit = fit;
        best_gen = new bool[row];
        for (int i = 0; i < row; ++i) {
            best_gen[i] = gen[i];
        }
    }
    return fit;
}

int *PHashGenetic::bestDegree(int degree[]) {

}

bool **PHashGenetic::generateInitialize() {
    bool **r = new bool *[POP_NUM];
    for (int k = 0; k < POP_NUM; ++k) {
        r[k] = new bool[row];
    }
    bernoulli_distribution u;

    for (int i = 0; i < POP_NUM; ++i) {
        for (int j = 0; j < row; ++j) {
            r[i][j] = u(e);
        }
    }
    return r;
}