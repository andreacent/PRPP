/*  
    Proyecto II - Diseno de algoritmos I
    Isaac Gonzalez  11-10396
    Andrea Centeno  10-10138
*/


#include <iostream>
using namespace std;

#ifndef VERTEX_H
#define VERTEX_H

struct vertex {
  int vertex;
  int benefit;
};

class CompareVertex {
public:
    bool operator()(vertex &v1, vertex &v2) {
        if (v1.benefit  < v2.benefit) return true;
        return false;
    }
};

#endif