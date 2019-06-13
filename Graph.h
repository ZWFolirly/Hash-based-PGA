//
// Created by Administrator on 2019/6/6.
//

#ifndef PARALLELHGA_GRAPH_H
#define PARALLELHGA_GRAPH_H

#include "string"

using namespace std;

class Graph {
public:
    int column;
    int row;
    const int MAX = 99999999;
    bool **map;

    Graph(string filename);

    Graph();

    void copy(Graph _g);

    void print();

};


#endif //PARALLELHGA_GRAPH_H
