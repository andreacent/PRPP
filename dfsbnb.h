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

void obtenerListaDeSucesores(
        int v,
        priority_queue<edge,vector<edge>,CompareEdges> &l,
        map<pair<int, int>, vector<int>> data,
        set<int> adyacentes);

bool cicloNegativo(edge ec, deque<edge> solParcial);

bool estaLadoEnSolParicial(edge ec, deque<edge> solParcial);

bool repiteCiclo(
        priority_queue<edge,vector<edge>,CompareEdges> l,
        edge ec, 
        deque<edge> solParcial);

bool cumpleAcotamiento(edge ec, int b_solParcial, int b_mejorSol, int b_disponible);

int beneficio(deque<edge> solucion);

void dfs(int v, //verticeMasExterno(solParcial);
         deque<edge> &solParcial, 
         deque<edge> &mejorSol, 
         map<pair<int, int>, vector<int>> data,
         map<int, set<int>> edges,
         int &beneficioDisponible);