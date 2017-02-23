/*  
    Proyecto I - Inteligencia Artificial II
    Isaac Gonzalez  11-10396
    Andrea Centeno  10-10138
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector> 
#include <set>
#include <queue>

using namespace std;
map<int, vector<int>> edges; //Adjacency list
map<int, vector<int>> edgesRQ; //Adjacency list RQ set
map<int, vector<int>> edgesR; //Adjacency list R set
map<pair<int, int>, vector<int>> data; //Edge data

//////////////PRINT FUNCTIONS//////////////
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

/* Print components vector */
void printComponents(vector<set<int>> &components){
    for (unsigned i=0;i<components.size();i++) {
        cout<<"Componente "<< i <<": ";
        for(auto const &ver : components[i]){
            cout<<ver<<",";
        }
        cout<<endl;
    }
}

////////////////////////////////////////////

/*Return true if the vertice is in any set of the vector c*/
bool isInSet(vector<set<int>> c,int vertice){
    for (auto const &s : c) {
        if(s.count(vertice)) return true;
    }
    return false;
}

/*Function used by dijkstraComponents*/
void getComponents(int v,int i,map<int, vector<int>> e,vector<set<int>> &components){
    for(auto const &s : e[v]){
        if((int)components.size() <= i){
            set<int> comp = {s};
            components.push_back(comp);
            getComponents(s,i,e,components);
        }else if(!components[i].count(s)){
            components[i].insert(s);
            getComponents(s,i,e,components);
        }
    }
}
/*dijkstra algorithm to find all connected components of set*/
void dijkstraComponents(map<int, vector<int>> e,vector<set<int>> &components){
    int i =0;
    for (auto const &s : e) {
        if(!isInSet(components,s.first)){ 
            getComponents(s.first,i,e,components);
            i++;
        }
    }
}

class CompareBenefit {
public:
    bool operator()(pair<int, int> &p1, pair<int, int> &p2) {
        int b1 = data[p1][1] - data[p1][0];
        int b2 = data[p2][1] - data[p2][0];
        if (b1 < b2) return true;
        return false;
    }
};

void kruskal(set<int> components,map<int, vector<int>> edges){
    vector<set<int>> comp;
    priority_queue<pair<int, int>,vector<pair<int, int>>,CompareBenefit> edge_cost;

    for(auto const &c : components){
        set<int> comp = {c};
        comp.insert(c);
        for(auto const &v : edges[c]){
            cout << "(" << c<<","<<v<<") "<<endl;
            pair<int,int> p1 = {c,v};
            if(data.count(p1)) edge_cost.push(p1);
        }
    }
    
    cout<<"Cola prioridad: ";
    while(!edge_cost.empty()){
        pair<int, int> p = edge_cost.top();
        edge_cost.pop();
        int b = data[p][1] - data[p][0];
        cout << " (" << p.first<<","<<p.second<<") c="<<b;
    }
    cout<< endl;

    
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
        if(!isP){
            edgesRQ[i].push_back(j);
            edgesRQ[j].push_back(i);
            if(c*2 <= b){
                data[make_pair(i,j)].push_back(1); //R
                edgesR[i].push_back(j);
                edgesR[j].push_back(i);
            }
            else data[make_pair(i,j)].push_back(0); //Q
        }
        else data[make_pair(i,j)].push_back(-1); //P

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

    //printEdgesMap(edges);
    //printData();

    vector<set<int>> componentsRQ;
    vector<set<int>> componentsR;
    cout << "\nComponentes conexas (R unido Q) --"<<endl;
    dijkstraComponents(edgesRQ,componentsRQ);
    printComponents(componentsRQ);
    cout << "\nComponentes conexas R --"<<endl;
    dijkstraComponents(edgesR,componentsR);
    printComponents(componentsR);


    kruskal(componentsR[4],edgesR);

    return 0;
}