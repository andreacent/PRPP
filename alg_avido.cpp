/*  
    Proyecto II - Diseno de algoritmos I
    Isaac Gonzalez  11-10396
    Andrea Centeno  10-10138
*/

#include "alg_avido.h"
    
using namespace std;

void heuristicaAvida(
		map<pair<int, int>, vector<int>> data,
        map<int, set<int>> edgesRQ,
        deque<edge> &solucion){

	set<edge> t; //R unido Q

	if(!edgesRQ.count(0)) //d==0
		encontrarLado(); //(d,u) con mayor beneficio

	int b = 0;
	while(!t.empty()){
		if(true){ // ∃(e ∈ T ∧ u ∈ V ) | e = (b, u)
			edge e_bu = obtenerLado(t,b);
			//t.erase(e_bu);
			solucion.push_back(e_bu);
			b = e_bu.coor.second;
		}else{
			set<deque<pair<int,int>>> ccm;

			set<edge>::iterator it;
    		for (it = t.begin(); it != t.end(); ++it){
    			deque<pair<int,int>> cm_ib;
    			caminoCostoMinimo((*it).coor.first,b,cm_ib);
    			ccm.insert(cm_ib);

    			cm_ib.clear();
    			caminoCostoMinimo((*it).coor.second,b,cm_ib);
    			ccm.insert(cm_ib);

    			cm_ib.clear();
    			obtenerCamino(ccm,cm_ib);

    			//solucion ← unirCaminoAlCiclo(solucion , cm_ib ) 

    			//eliminar toda arista de cm_ib de T

    			//b = i; i es (*it).coor.first o (*it).coor.second
    		}
		}
	}

	if(solucion.back().coor.second != 0){
		deque<pair<int,int>> cm_ib;
		caminoCostoMinimo(solucion.back().coor.second,0,cm_ib);
		//solucion ← unirCaminoAlCiclo(solucion , cm_ib ) 
	}
}