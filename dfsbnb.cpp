/*  
    Proyecto II - Diseno de algoritmos I
    Isaac Gonzalez  11-10396
    Andrea Centeno  10-10138
*/

#include "dfsbnb.h"
    
void obtenerListaDeSucesores(
        int v,
        priority_queue<edge,vector<edge>,CompareEdges> &l,
        map<pair<int, int>, vector<int>> data,
        set<int> adyacentes){

    std::set<int>::iterator it;
    for (it = adyacentes.begin(); it != adyacentes.end(); ++it){
        edge ec = {{v,*it}, data[ec.coor][1], data[ec.coor][0]};
        l.push(ec);

        ec = {{v,*it}, 0, data[ec.coor][0]};
        l.push(ec);
    }
}

bool cicloNegativo(edge ec, deque<edge> solParcial){
    int beneficio = ec.benefit - ec.cost;
    bool ciclo = false;

    for (deque<edge>::iterator it = solParcial.begin(); it != solParcial.end(); ++it){
        //al encontrar un ciclo, calculo el beneficio desde 
        //donde se genera el ciclo hasta donde se cierra
        if(ciclo) beneficio += (*it).benefit - (*it).cost;
        else if( ec.coor.second ==  (*it).coor.first){ 
            ciclo = true;
            beneficio += (*it).benefit - (*it).cost;
        }
    }

    if(ciclo && beneficio < 0) return true;
    return false;
}

bool estaLadoEnSolParicial(edge ec, deque<edge> solParcial){
    int arista_count=0;

    for (deque<edge>::iterator it = solParcial.begin(); it != solParcial.end(); ++it)
        if( ec.coor == (*it).coor ||
            ( ec.coor.first == (*it).coor.second && 
              ec.coor.second == (*it).coor.first )
          ) arista_count++;

    if (!arista_count) return false;
    else if (arista_count == 1){
       if (ec.benefit == 0) return false;
       else return true;
    }

    return true;
}

bool repiteCiclo(
        priority_queue<edge,vector<edge>,CompareEdges> l,
        edge ec, 
        deque<edge> solParcial){

    for (deque<edge>::iterator it = solParcial.begin(); it != solParcial.end(); ++it)
        if( ec.coor.second ==  (*it).coor.first ){
            if(ec.benefit - ec.cost < (*it).benefit - (*it).cost)
                return false;
            else return true;
        }

    return false;
}

bool cumpleAcotamiento(edge ec, int b_solParcial, int b_mejorSol, int b_disponible){
    int beneficioE = ec.benefit - ec.cost;
    int beneficioSolParcial = b_solParcial + beneficioE;
    int maxBeneficio = b_disponible - max(0, beneficioE) + beneficioSolParcial;
    if(maxBeneficio <= b_mejorSol) return false;
    return true;
}

int beneficio(deque<edge> solucion){
    int total =0;
    if(solucion.empty()) return total;

    for (deque<edge>::iterator it = solucion.begin(); it != solucion.end(); ++it){
        cout<<endl;
        cout<<(*it).coor.first<<","<<(*it).coor.second<<" beneficio "<<(*it).benefit<<" costo "<<(*it).cost;
        total += (*it).benefit - (*it).cost;
    }
    return total;
}

void dfs(int v, //verticeMasExterno(solParcial);
         deque<edge> &solParcial, 
         deque<edge> &mejorSol, 
         map<pair<int, int>, vector<int>> data,
         map<int, set<int>> edges,
         int &beneficioDisponible){

    int b_solParcial = beneficio(solParcial);
    int b_mejorSol = beneficio(mejorSol);

    cout<<v<<" ";
    // Vemos si se encuentra una mejor solucion factible
    if(v==0) //v==d
        if(b_solParcial > b_mejorSol){
            mejorSol.clear();
            mejorSol = solParcial;
        }

    // Lista de aristas
    priority_queue<edge,vector<edge>,CompareEdges> l;
    obtenerListaDeSucesores(v,l,data,edges[v]); 

    edge e;
    while(!l.empty()){
        e = l.top();
        l.pop();
        if(!cicloNegativo(e,solParcial) && 
           !estaLadoEnSolParicial(e,solParcial) &&
           !repiteCiclo(l,e,solParcial) &&
           cumpleAcotamiento(e,b_solParcial,b_mejorSol,beneficioDisponible)){
            solParcial.push_back(e);
            beneficioDisponible -= max(0,e.benefit-e.cost); //beneficio y costo de e
            dfs(e.coor.second,solParcial,mejorSol,data,edges,beneficioDisponible);
        }
    }

    //e=eliminarUltimoLado(solParcial);
    e = solParcial.back();
    solParcial.pop_back();
    beneficioDisponible += max(0,e.benefit-e.cost);
}