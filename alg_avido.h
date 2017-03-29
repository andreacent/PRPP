/*  
    Proyecto II - Diseno de algoritmos I
    Isaac Gonzalez  11-10396
    Andrea Centeno  10-10138
*/
#define MAX 999999999
#define MIN -999999
#include <map>
#include <vector> 
#include <set>
#include <queue>
#include "edge.h"
#include "vertex.h"
    
using namespace std;

set<int> obtenerVertices(map<int, set<int>> &edges);

bool exist(int u, map <int,int> &prev);

bool pertenece(edge &e, vector<edge> &solution);

int getfake_benefit(int u, int v, vector<edge> &solution, map<pair<int, int>, vector<int>> &data);

int get_benefit (edge e, vector<edge> &solution);

int path_benefit(deque< pair<int,int>> c, vector<edge> &solution, map<pair<int, int>, vector<int>> &data);

void terase(edge e, vector<edge> &t);

edge encontrarLado(vector<edge> &t, vector<edge> &solution);

edge obtenerLado(vector<edge> &t, int b, vector<edge> &solution);

deque<edge> obtenerCamino(set<deque<pair<int,int>>> &ccm, vector<edge> &solution, map<pair<int, int>, vector<int>> &data);

bool exist_u(vector<edge> t, int b);

bool exist_e(edge e, vector<edge> t);

void rpath_fromt(deque<edge> rcm, vector<edge> &t);

int get_i(deque<edge> rcm);

void unirCaminoAlCiclo(vector<edge> &solution, deque<edge> c);

int caminoCostoMinimo(  int source, int target, deque<pair<int,int>> &cm_bi,
							     set<int> &v, map<pair<int, int>, vector<int>> &data,
							     map<int, set<int>> edges,
							     vector<edge> &solution);

void heuristicaAvida(
		map<pair<int, int>, vector<int>> data,
        map<int, set<int>> edgesRQ,
       	map<int, set<int>> edges,
        vector<edge> &solution);

void printh(vector<edge> mejorSol);
void printc(deque<pair<int,int>> cm);