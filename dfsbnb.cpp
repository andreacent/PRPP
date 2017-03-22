/*  
    Proyecto II - Diseno de algoritmos I
    Isaac Gonzalez  11-10396
    Andrea Centeno  10-10138
*/

#include "print.h"
#include <algorithm>
#include <ctime>
#include <csignal>

int solParcialBenef;
deque<edges> solParcialAristas;
int beneficioDisponible;

void obtenerListaDeSucesores(
        int v,
        priority_queue<edge,vector<edge>,CompareEdges> &l,
        map<pair<int, int>, vector<int>> data,
        set<int> adyacentes){

    for (auto const &vertice : adyacentes){
        edge ec = {{v,vertice}, data[ec.pair][1], data[ec.pair][0]};
        l.push(ec);

        ec = {{v,vertice}, 0, data[ec.pair][0]};
        l.push(ec);
    }
}

bool cicloNegativo(edge ec, deque<int> solParcial){
    int beneficio = ec.benefit - ec.cost;
    bool ciclo = false;

    deque<int>::iterator it = solParcial.begin();
    while (it != solParcial.end()){
        if(ciclo) beneficio += (*it).benefit - (*it).cost;
        else if( ec.pair.second ==  (*it).pair.first){
            ciclo = true;
            beneficio += (*it).benefit - (*it).cost;
        }
    }

    if(ciclo && beneficio < 0) return true;
    return false;
}

bool estaLadoEnSolParicial(edge ec, deque<edges> solParcial){
    int v_qty=0;

    deque<int>::iterator it = solParcial.begin();
    while (it != solParcial.end())
        if( ec.pair == (*it).pair ||
            ( ec.pair.first == (*it).pair.second && 
              ec.pair.second == (*it).pair.first )
          ) v_qty++;

    if (!v_qty) return false;
    else if (v_qty == 1){
       if (ec.benefit == 0) return false;
       else return true;
    }

    return true;
}

// no me queda del todo claro esta funcion
bool repiteCiclo(
        priority_queue<edge,vector<edge>,CompareEdges> l,
        edge ec, 
        deque<edges> solParcial){

    bool ciclo = false;
    edge ec_prima;

    deque<int>::iterator it = solParcial.begin();
    while (it != solParcial.end()){
        if( ec.pair.second ==  (*it).pair.first ){
            if(ec.benefit - ec.cost < (*it).benefit - (*it).cost)
                return false;
            else return true;
        }

    return false;
}

bool cumpleAcotamiento(edge ec, deque<edges> solParcial){
    int beneficioE = ec.benefit - ec.cost;
    int beneficioSolParcial = beneficio(solParcial) + beneficioE;
    int maxBeneficio = beneficioDisponible - max(0, beneficioE) + beneficioSolParcial;
    if(maxBeneficio <= beneficio(mejorSol)) return false;
    return true;
}

void dfs(int v, //verticeMasExterno(solParcial);
         deque<int> &solParcial, 
         int &mejorSol, 
         map<pair<int, int>, vector<int>> data,
         map<int, set<int>> edges){

    // Vemos si se encuentra una mejor solucion factible
    if(v==1){ //v==d
        if(beneficio(solParcial) > beneficio(mejorSol)){
            mejorSol = solParcial;
        }
    }

    //L(v) = obtenerListaDeSucesores(v); // Lista de aristas
    priority_queue<edge,vector<edge>,CompareEdges> l;
    obtenerListaDeSucesores(v,l,data,edges[v]); 

    for (auto const &e : L(v)) {
        if(!cicloNegativo(e,solParcial) && 
           !estaLadoEnSolParicial(e,solParcial) &&
           !repiteCiclo(L(v),e,solParcial) &&
           cumpleAcotamiento(e,solParcial)){
            agregarLado(e,solParcial);
            beneficioDisponible = beneficioDisponible -max(0,b-c); //beneficio y costo de e
            dfs();
        }
    }
    //e=eliminarUltimoLado(solParcial);
    beneficioDisponible = beneficioDisponible + (0,b-c);
}