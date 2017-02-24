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

/*Function used by dfsComponents*/
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
/*dfs algorithm to find all connected components of set*/
void dfsComponents(map<int, vector<int>> e,vector<set<int>> &components){
    int i =0;
    for (auto const &s : e) {
        if(!isInSet(components,s.first)){ 
            getComponents(s.first,i,e,components);
            i++;
        }
    }
}

struct edgeCost {
  pair<int, int> edge;
  int cost;
};

class CompareBenefit {
public:
    bool operator()(edgeCost &e1, edgeCost &e2) {
        if (e1.cost < e2.cost) return true;
        return false;
    }
};

void dijkstra(set<int> components,map<int, vector<int>> edges, int s){
    vector<set<int>> comp;
    priority_queue<edgeCost,vector<edgeCost>,CompareBenefit> edge_cost;
    vector<int> path;
    set<pair<int,int>> path_pair;
    path.push_back(s);    
    int sum =0;

    for(auto const &v : edges[s]){
        edgeCost ec,ec2;
        ec.edge = {s,v};
        if( data.count(ec.edge)) {
            ec.cost = data[ec.edge][1] - data[ec.edge][0];
            edge_cost.push(ec);
        }
        ec2.edge = {v,s};
        if( data.count(ec2.edge)) {
            ec.cost = data[ec2.edge][1] - data[ec2.edge][0];
            edge_cost.push(ec);
        }
    }
    
    while(!edge_cost.empty()){
        edgeCost edc = edge_cost.top();
        if(sum > sum+edc.cost) break;
        edge_cost.pop();
        //cout<<"edc "<<edc.edge.first<<","<<edc.edge.second<<" cost="<<edc.cost<<endl;
        path.push_back(edc.edge.second);
        path_pair.insert(edc.edge);
        sum+=edc.cost;

        for(auto const &v : edges[edc.edge.second]){
            edgeCost ec,ec2;
            ec.edge = {edc.edge.second,v};
            ec2.edge = {v,edc.edge.second};
            if( data.count(ec.edge) && !path_pair.count(ec.edge)){//data[i,j] existe, no se ha pasado por [i,j]
                if(path_pair.count(ec2.edge)) //si se paso por [j,i]
                    ec.cost = -data[ec.edge][0];
                else 
                    ec.cost = data[ec.edge][1] - data[ec.edge][0];
                edge_cost.push(ec);
            }

            if(data.count(ec2.edge) && !path_pair.count(ec2.edge) ){//data[j,i] existe, no se ha pasado por [j,i]
                if(path_pair.count(ec.edge)) //si se paso por [i,j]
                    edge_cost.push({ec.edge,- data[ec2.edge][0]});
                else 
                    edge_cost.push({ec.edge,data[ec2.edge][1] - data[ec2.edge][0]});
            }
        }
    } 

    cout<<"PATH ";
    for(auto const &v : path){
        cout << v<<" ";
    }
    cout<<" SUM="<<sum<< endl;    
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

    //printData();

    vector<set<int>> componentsRQ;
    vector<set<int>> componentsR;
    cout << "\nComponentes conexas (R unido Q) --"<<endl;
    dfsComponents(edgesRQ,componentsRQ);
    printComponents(componentsRQ);
    cout << "\nComponentes conexas R --"<<endl;
    dfsComponents(edgesR,componentsR);
    printComponents(componentsR);

    //prioridad de las componentes conexas del conj R
    cout<<"\nCOLAS DE PRIORIDAD: \n";
    //printEdgesMap(edgesR);
    dijkstra(componentsR[10],edgesR,*componentsR[10].begin());
    /*
    for(auto const &comp : componentsR){
        dijkstra(comp,edgesR,*comp.begin());
    }
*/
    return 0;
}