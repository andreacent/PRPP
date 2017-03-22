/*  
    Proyecto II - Diseno de algoritmos I
    Isaac Gonzalez  11-10396
    Andrea Centeno  10-10138
*/

#include "alg_avido.h"
#include "dfsbnb.h"
#include <ctime>
#include <csignal>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

map<int, set<int>> edges; //Adjacency list
map<pair<int, int>, vector<int>> data; //Edge data

/* LECTURA DE ARCHIVO */
void insertData(int i,int j, int c,int b, bool isP){
    data[make_pair(i,j)].push_back(c);
    data[make_pair(i,j)].push_back(b);
    //is P, R or Q
    if(!isP){
        if(c*2 <= b) data[make_pair(i,j)].push_back(1); //R
        else data[make_pair(i,j)].push_back(0); //Q
    }
    else data[make_pair(i,j)].push_back(-1); //P

    edges[i].insert(j);
}

void setDataAndEdge(ifstream &infile, int loop, bool isP){
    string line,token;
    int i,j,c,b;
    for(int x=0;x<loop;x++) {
        getline(infile, line);
        istringstream ss(line);

        getline(ss,token,' ');
        i = stoi(token)-1; 
        getline(ss,token,' ');
        j = stoi(token)-1; 
        getline(ss,token,' ');
        c = stoi(token); 
        getline(ss,token,' ');
        b = stoi(token); 

        insertData(i,j,c,b, isP);
        insertData(j,i,c,b, isP);
    }
}

/*
argv[1] -> intance
argv[2] -> solucion optima
*/
int main(int argc, char const *argv[]) {
    int vertices,rqdEdges, nonRqdEdges;
    string filecito = argv[1];
    string filename = filecito.substr(0, filecito.find("."));

    if (argc < 2) { // Check the value of argc. If not enough parameters have been passed, inform user and exit.
        cout << "Usage is ./main <file>\n"; // Inform the user of how to use the program
        exit(0);
    }

    /*****Read file to get data*****/
    ifstream infile("instanciasPRPP/ALBAIDA/ALBAIDAANoRPP"); //ifstream infile(argv[1]);
    string line,token;

    //get number of vertices 
    getline(infile, line); 
    vertices = stoi(line.substr(line.find(":")+1));
    cout << "vertices = "<<vertices<<endl;

    //get number of required edges
    getline(infile, line); 
    rqdEdges = stoi(line.substr(line.find("edges")+5));
    cout << "rqdEdges = "<<rqdEdges<<endl;
    setDataAndEdge(infile, rqdEdges, false);

    //get number of non required edges
    getline(infile, line); 
    nonRqdEdges = stoi(line.substr(line.find("edges")+5));
    cout << "nonrqdEdges = "<<nonRqdEdges<<endl;
    setDataAndEdge(infile, nonRqdEdges, true);

    deque<edge> solParcial;
    deque<edge> mejorSol; //SE DEBE OBTENER DEL ALGORITMO DEL PROYECTO1
    int beneficioDisponible = 0; // NO ESTOY CLARA COMO SE DEBE INICIALIZAR

    /* ELIMINAR */
    deque<pair<int,int>> solAlbaidaA ={
                {0,1},{1,7},{7,23},{23,33},{33,47},{47,41},
                {41,49},{49,54},{54,55}}; 

    deque<pair<int,int>> solAlbaidaAreturn ={{55,54},{54,49},
                {49,41},{41,47},{47,33},{33,23},{23,7},{7,1},{1,0}};

    deque<pair<int,int>>::iterator it;
    for (it = solAlbaidaA.begin(); it != solAlbaidaA.end(); ++it){
        edge e = {*it, data[e.coor][1], data[e.coor][0]};
        mejorSol.push_back(e);
    }

    for (it = solAlbaidaAreturn.begin(); it != solAlbaidaAreturn.end(); ++it){ 
        edge e = {*it, 0, data[e.coor][0]};
        mejorSol.push_back(e);
    }
    /* ELIMINAR */

    unsigned t0, t1;
    t0=clock();

    cout<<"vamos por el dfs"<<endl;
    dfs(0,solParcial,mejorSol,data,edges,beneficioDisponible);

    t1 = clock();
    double time = (double(t1-t0)/CLOCKS_PER_SEC);
    cout << "Execution Time: " << time << endl;

    return 0;
}