/*  
    Proyecto II - Diseno de algoritmos I
    Isaac Gonzalez  11-10396
    Andrea Centeno  10-10138
*/

#include "dfsbnb.h"
#include <algorithm>
#include <ctime>
#include <csignal>

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

    if (argc < 3) { // Check the value of argc. If not enough parameters have been passed, inform user and exit.
        cout << "Usage is ./main <file> <int>\n"; // Inform the user of how to use the program
        exit(0);
    }

    /*****Read file to get data*****/
    ifstream infile(argv[1]);
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

    //printData(data);

    unsigned t0, t1;
    t0=clock();

    deque<edge> solParcial;
    //def(0,solParcial,mejorSol,data,edges);

    t1 = clock();
    double time = (double(t1-t0)/CLOCKS_PER_SEC);
    cout << "Execution Time: " << time << endl;

    return 0;
}