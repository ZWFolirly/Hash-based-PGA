//
// Created by Administrator on 2019/6/6.
//

#include <iostream>
#include "Graph.h"

#include "stdlib.h"
#include "string"
#include "fstream"

using namespace std;
Graph::Graph(string filename) {
    ifstream fi;
    fi.open(filename);
    if (fi.is_open()) {
        fi >> column >> row;
        map = new bool*[column];
        for (int k = 0; k < column; ++k) {
            map[k] = new bool[row];
        }
        int t = 0;
        for (int i = 0; i < column; ++i) {
            for (int j = 0; j < row; ++j) {
                fi >>t;
                map[i][j] = (bool)t;
            }
        }
        fi.close();
    }
}

Graph::Graph() {
    column = 0;
    row = 0;
    map = nullptr;
}

void Graph::copy(Graph _g){
    column = _g.column;
    row = _g.row;
    map = new bool*[column];
    for (int k = 0; k < column; ++k) {
        map[k] = new bool[row];
    }
    for (int i = 0; i < column; ++i) {
        for (int j = 0; j < row; ++j) {
            map[i][j] = _g.map[i][j];
        }
    }
}
void Graph::print(){
    for (int i = 0; i < column; ++i) {
        for (int j = 0; j < row; ++j) {
            cout << map[i][j] << " ";
        }
        cout << endl;
    }
}


