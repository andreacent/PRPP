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
map<int, set<int>> edgesRQ; //Adjacency list RQ set
map<pair<int, int>, vector<int>> data; //Edge data

/* LECTURA DE ARCHIVO */
void insertData(int i,int j, int c,int b, bool isP){
    data[make_pair(i,j)].push_back(c);
    data[make_pair(i,j)].push_back(b);
    //is P, R or Q
    if(!isP){
        edgesRQ[i].insert(j);
        if(c*2 <= b) data[make_pair(i,j)].push_back(1); //R
        else data[make_pair(i,j)].push_back(0); //Q
    }
    else data[make_pair(i,j)].push_back(-1); //P

    edges[i].insert(j);
}

int setDataAndEdge(ifstream &infile, int loop, bool isP){
    string line,token;
    int i,j,c,b;
    int beneficioDisponible = 0;
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

        if(!isP) beneficioDisponible += b-c;
    }
    return beneficioDisponible;
}

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

// Funcion que mata el programa cuando recibe una senal
void signalHandler( int ){
    fprintf(stderr, "%s\n", "Falló por tiempo.");
    exit(1);
}

/*
argv[1] -> intance
argv[2] -> solucion optima
*/
int main(int argc, char const *argv[]) {
    int vertices,rqdEdges, nonRqdEdges;
    signal( SIGINT, &signalHandler );

    if (argc < 2) { // Check the value of argc. If not enough parameters have been passed, inform user and exit.
        cout << "Usage is ./main <file>\n"; // Inform the user of how to use the program
        exit(0);
    }

    /*****INICIO Read file to get data*****/
    string filecito = argv[1];
    ifstream infile(argv[1]);
    string line,token;

    //get number of vertices 
    getline(infile, line); 
    vertices = stoi(line.substr(line.find(":")+1));
    cout << "vertices = "<<vertices<<endl;

    //get number of required edges
    getline(infile, line); 
    rqdEdges = stoi(line.substr(line.find("edges")+5));
    //cout << "rqdEdges = "<<rqdEdges<<endl;
    int beneficioDisponible = setDataAndEdge(infile, rqdEdges, false);

    //get number of non required edges
    getline(infile, line); 
    nonRqdEdges = stoi(line.substr(line.find("edges")+5));
    //cout << "nonrqdEdges = "<<nonRqdEdges<<endl;
    setDataAndEdge(infile, nonRqdEdges, true);
    /*****FIN Read file to get data*****/

    //printEdgesMap(edges);

    //SOLUCION CON HEURISTICA DE PROYECTO 1
    vector<edge> mejorSol;
    heuristicaAvida(data,edgesRQ,edges,mejorSol);
//./main instanciasPRPP/CHRISTOFIDES/P01NoRPP > resultados/P01NoRPP.txt
    cout<<"INICIAL: ";
    for(vector<edge>::iterator it = mejorSol.begin(); it != mejorSol.end(); ++it){
        cout<<"("<<(*it).coor.first<<","<<(*it).coor.second<<") ";
    }
    cout<<endl;

    //PROYECTO 2
    unsigned t0, t1;
    t0=clock();

    obtenerAristaInicial(0,data,edges[0]);
    obtenerMaximoBeneficio(beneficioDisponible);
    dfs(mejorSol,data,edges);
    /*

    #include <chrono>
    chrono::minutes ms(2);
    chrono::time_point<chrono::system_clock> end;
    end = chrono::system_clock::now() + ms; // this is the end point
    while(chrono::system_clock::now() < end){ // still less than the end?
        dfs(mejorSol,data,edges);
    }
    */

    t1 = clock();


    //RESULTADO SI TERMINA ANTES DE LAS 2 HORAS
    cout<<endl<<"RESULTADO: ";
    for(vector<edge>::iterator it = mejorSol.begin(); it != mejorSol.end(); ++it){
        cout<<"("<<(*it).coor.first<<","<<(*it).coor.second<<") ";
    }
    cout<<endl<<"Beneficio = "<<beneficio(mejorSol)<<endl;

    double time = (double(t1-t0)/CLOCKS_PER_SEC);
    cout << "Tiempo: " << time << endl;
    return 0;
}