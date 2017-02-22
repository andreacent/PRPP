/*  
    Proyecto I - Inteligencia Artificial II
    Isaac Gonzalez  11-10396
    Andrea Centeno  10-10138
*/
#include <map>
#include <vector> 
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;
int num_R=0;
map<int, vector<int>> edges; //Adjacency list
//map<pair<int,int>, pair<int,int>> data; 

int isRorQ(int edge[]){
    if(edge[2]*2 <= edge[3]){
        num_R+=1;
        return 1;
    }
    return 0;
}

/* Print map of edges */
void printEdgesMap(){
    for (unsigned i = 0; i < edges.size(); ++i) {
        cout<<"Nodo "<< i << ":";
        for(unsigned j = 0; j < edges[i].size(); ++j){
            cout<<" "<< edges[i][j];
        }
        cout<<endl;
    }
}

/* Connected component */
void getComponents(){
}

/*
argv[1] -> intance
*/
int main(int argc, char const *argv[]) {
    int vertices,rqdEdges, nonRqdEdges;

    if (argc < 2) { // Check the value of argc. If not enough parameters have been passed, inform user and exit.
        cout << "Usage is ./main <file> \n"; // Inform the user of how to use the program
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

    int rqd_edges[rqdEdges][4]; //node1 node2 cost benefit isR
                                //isR = 1 significa que al pasar dos veces por ese lado, el beneficion es >= 0
    for(int i=0;i<rqdEdges;i++) {
        getline(infile, line);
        istringstream ss(line);
        for(int j =0;j<4;j++){
            getline(ss,token,' ');
            rqd_edges[i][j] = stoi(token); 
            cout <<rqd_edges[i][j] <<"\t";
        }
        rqd_edges[i][5] = isRorQ(rqd_edges[i]);
        cout<< rqd_edges[i][5]<<endl;


        //matriz - tipo de arco
        edges[rqd_edges[i][0]].push_back(rqd_edges[i][1]);
        edges[rqd_edges[i][1]].push_back(rqd_edges[i][0]);

    }

    //get number of non required edges
    getline(infile, line); 
    nonRqdEdges = stoi(line.substr(line.find("edges")+5));
    cout << "nonrqdEdges = "<<nonRqdEdges<<endl;

    int non_rqd_edges[nonRqdEdges][4];
    for(int i=0;i<nonRqdEdges;i++) {
        getline(infile, line);
        istringstream ss(line);
        for(int j =0;j<4;j++){
            getline(ss,token,' ');
            non_rqd_edges[i][j] = stoi(token); 
            cout <<non_rqd_edges[i][j] <<"\t";
        }
        cout<<endl;

        //matriz - tipo de arco
        edges[non_rqd_edges[i][0]].push_back(non_rqd_edges[i][1]);
        edges[non_rqd_edges[i][1]].push_back(non_rqd_edges[i][0]);

    }

    printEdgesMap();

    return 0;
}