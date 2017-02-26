/*  
    Proyecto I - Diseno de algoritmos I
    Isaac Gonzalez  11-10396
    Andrea Centeno  10-10138
*/

#include "header.h"
    
/* Print map of edges */
void printEdgesMap(map<int, vector<int>> e){
    for (auto const &f : e) {
        cout<<"Nodo "<< f.first << ":";
        for(auto const &s : f.second){
            cout<<" "<< s;
        }
        cout<<endl;
    }
}

/* Print edges data */
void printData(map<pair<int, int>, vector<int>> data){
    for (auto const &d : data) {
        cout<<"("<< d.first.first <<" , "<<d.first.second << ") :";
        for(auto const &val : d.second){
            cout<<"\t"<< val;
        }
        cout<<endl;
    }
}

/* Print connected components vector */
void printConnectedComponent(vector<set<int>> components){
    for (unsigned i=0;i<components.size();i++) {
        cout<<"Componente "<< i <<": ";
        for(auto const &ver : components[i]){
            cout<<ver<<",";
        }
        cout<<endl;
    }
}

/* Print component struct */
void printComponents(deque<component> components){
    for (auto const &comp : components) {
        cout<<"Hojas: ";
        for (auto const &l : comp.leaves) cout<<l<<" "; 
        cout<<"\nAristas: ";
        for(auto const &e : comp.edges) cout<<"("<< e.first <<" , "<<e.second << ") "; 
        cout<<"\nGanancia: "<<comp.benefit<<endl;
    }
}

/* Print vector of int */
void printVector(vector<int> vec){
    for (auto const &v : vec) cout<<v<<" ";
}

/* Print set of edges */
void printSetOfPair(set<pair<int,int>> set_pair){
    for(auto const &p : set_pair) cout<<"("<<p.first<<","<<p.second<<") ";
    cout<<endl; 
}