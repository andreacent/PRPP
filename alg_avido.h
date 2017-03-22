/*  
    Proyecto II - Diseno de algoritmos I
    Isaac Gonzalez  11-10396
    Andrea Centeno  10-10138
*/

#include <map>
#include <vector> 
#include <set>
#include <queue>
#include "edge.h"
    
using namespace std;

void encontrarLado();

edge obtenerLado (set<edge> t, int b);

void caminoCostoMinimo(int i,int b,deque<pair<int,int>> cm_ib);

void obtenerCamino(set<deque<pair<int,int>>> ccm, deque<pair<int,int>> cm_ib);

void heuristicaAvida(
		map<pair<int, int>, vector<int>> data,
        map<int, set<int>> edgesRQ,
        deque<edge> &solucion);
