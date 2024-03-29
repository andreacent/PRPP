/*  
    Proyecto I - Diseno de algoritmos I
    Isaac Gonzalez  11-10396
    Andrea Centeno  10-10138
*/

#include "header.h"
    
/* Print map of edges */
void printEdgesMap(map<int, set<int>> e){
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

/* Print one connected component vector */
void printComponent(component comp){
    cout<<"\nVertices: ";
    for (auto const &l : comp.vertices) cout<<l<<" "; 
    cout<<"\nHojas: ";
    for (auto const &l : comp.leaves) cout<<l<<" "; 
    cout<<"\nGanancia: "<<comp.benefit<<endl;
    cout<<endl;
}

/* Print connected components vector */
void printConnectedComponents(deque<component> components){
    cout<<components.size()<<" componentes";
    for (auto const &comp : components) {
        cout<<"\nVertices: ";
        for (auto const &l : comp.vertices) cout<<l<<" "; 
        cout<<"\nHojas: ";
        for (auto const &l : comp.leaves) cout<<l<<" "; 
        cout<<"\nGanancia: "<<comp.benefit<<endl;
    }
    cout<<endl;
}

/* Print component struct */
void printPathsDeque(deque<path> components){
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

/* Print set*/
void printSet(set<int> set){
    for(auto const &p : set) cout<<"("<<p<<") ";
    cout<<endl; 
}