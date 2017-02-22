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
map<pair<int, int>, vector<int>> data; //Edge data

/* Print map of edges */
void printEdgesMap(){
    for (auto const &f : edges) {
        cout<<"Nodo "<< f.first << ":";
        for(auto const &s : f.second){
            cout<<" "<< s;
        }
        cout<<endl;
    }
}

/* Print edges data */
void printData(){
    for (auto const &d : data) {
        cout<<"("<< d.first.first <<" , "<<d.first.second << ") :";
        for(auto const &val : d.second){
            cout<<"\t"<< val;
        }
        cout<<endl;
    }
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

        data[make_pair(i,j)].push_back(c);
        data[make_pair(i,j)].push_back(b);
        //is P, R or Q
        if(isP) data[make_pair(i,j)].push_back(-1);
        else if(c*2 <= b) data[make_pair(i,j)].push_back(1);
        else data[make_pair(i,j)].push_back(0);

        //matriz - tipo de arco
        edges[i].push_back(j);
        edges[j].push_back(i);
    }
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
    setDataAndEdge(infile, rqdEdges, false);

    //get number of non required edges
    getline(infile, line); 
    nonRqdEdges = stoi(line.substr(line.find("edges")+5));
    cout << "nonrqdEdges = "<<nonRqdEdges<<endl;
    setDataAndEdge(infile, nonRqdEdges, true);

    printEdgesMap();
    printData();

    return 0;
}