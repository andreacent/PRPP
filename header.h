/*  
    Proyecto I - Diseno de algoritmos I
    Isaac Gonzalez  11-10396
    Andrea Centeno  10-10138
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector> 
#include <set>
#include <queue>

using namespace std;

struct edgeCost {
  pair<int, int> edge;
  int cost;
};


struct path {
  set<pair<int,int>> edges;
  set<int> leaves;
  int benefit;
};

struct component {
  set<int> vertices;
  set<int> leaves;
  int benefit;
};

class CompareBenefit {
public:
    bool operator()(edgeCost &e1, edgeCost &e2) {
        if (e1.cost < e2.cost) return true;
        return false;
    }
};