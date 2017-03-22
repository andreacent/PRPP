/*  
    Proyecto II - Diseno de algoritmos I
    Isaac Gonzalez  11-10396
    Andrea Centeno  10-10138
*/


#include <iostream>
using namespace std;

#ifndef EDGE_H
#define EDGE_H

struct edge {
  pair<int, int> coor;
  int benefit;
  int cost;
};

class CompareEdges {
public:
    bool operator()(edge &e1, edge &e2) {
        if (e1.benefit - e1.cost  < e2.benefit - e2.cost ) return true;
        return false;
    }
};

#endif