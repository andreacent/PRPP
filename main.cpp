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
void printData(){
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

/* Print vector of int */
void printVector(vector<int> vec){
    for (auto const &v : vec) {
        cout<<v<<" ";
    }
}

void printSetOfPair(set<pair<int,int>> set_pair){
    for(auto const &p : set_pair) cout<<"("<<p.first<<","<<p.second<<") ";
    cout<<endl; 
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
            //if(!components[i].count(0)) components.erase(components.begin()+i);
            //else i++;
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

int dijkstra(  set<int> components,
                map<int, vector<int>> edges, 
                int s,
                set<pair<int,int>> &edge_path,
                map<int,pair<int,int>> &degrees,
                vector<deque<int>> &paths){ 

    priority_queue<edgeCost,vector<edgeCost>,CompareBenefit> edge_cost; //aristas por orden de beneficio
    priority_queue<edgeCost,vector<edgeCost>,CompareBenefit> edge_cost_retorned; //aristas repetidas (pertenecen a R) por orden de beneficio
    vector<int> path; //vertices por donde he pasado
    int sum =0;
    path.push_back(s); 
    paths.push_back({s});

    for(auto const &v : edges[s]){
        edgeCost ec = {{s,v}, data[ec.edge][1] - data[ec.edge][0]};
        edge_cost.push(ec);
    }
    
    while(!edge_cost.empty()){
        edgeCost edc = edge_cost.top();
        edge_cost.pop();
        //cout<<"edc "<<edc.edge.first<<","<<edc.edge.second<<" cost="<<edc.cost<<endl;

        //if( edge_path.count(edc.edge) > 0 && sum > sum+edc.cost ) continue; //no repetir arista dirigida
        if( edge_path.count(edc.edge) > 0 || edge_path.count(make_pair(edc.edge.second,edc.edge.first)) > 0) continue; //no repetir arista dirigida

        //Increase degree of vertices
        if(degrees.count(edc.edge.first)) degrees[edc.edge.first].first += 1;
        else degrees[edc.edge.first].first = 1;
        if(degrees.count(edc.edge.second)) degrees[edc.edge.second].second += 1;
        else degrees[edc.edge.second].second = 1;
        
        path.push_back(edc.edge.second);

        int iter = -1;
        bool back = true;
        bool second = true;
        for(unsigned i=0;i<paths.size();i++){
            if(paths[i].back() == edc.edge.first){
                iter = i;
                break;
            }else if(paths[i].front() == edc.edge.first){
                iter = i;
                back = false;
                break;
            }else if(paths[i].back() == edc.edge.second){
                iter = i;
                second = false;
                break;
            }else if(paths[i].front() == edc.edge.second){
                iter = i;
                second = false;
                back = false;
                break;
            }
        }
        if(iter < 0) paths.push_back({edc.edge.first,edc.edge.second});
        else if(back && second) paths[iter].push_back(edc.edge.second);
        else if (second) paths[iter].push_front(edc.edge.second);
        else if(back) paths[iter].push_back(edc.edge.first);
        else paths[iter].push_front(edc.edge.first);

        edge_path.insert(edc.edge);
        sum+=edc.cost;

        for(auto const &v : edges[edc.edge.second]){
            edgeCost ec,ec2;
            ec.edge = { edc.edge.second, v };
            ec2.edge = { v, edc.edge.second };

            if( !edge_path.count(ec.edge) && !edge_path.count(ec2.edge) ){ //no ha pasado por [i,j], ni [j,i]
                ec.cost = data[ec.edge][1] - data[ec.edge][0];
                edge_cost.push(ec);
            }
        }
    } 

    //cout<<"PATH ";
    //printVector(path);
    //cout<<" SUM="<<sum<< endl; 

    return sum;
}

void findLeaves(set<int> components,map<int, vector<int>> edges,vector<int> &d, char t){
    for(auto const &c : components){
        if((int)edges[c].size() == 1){
            if (data.count(make_pair(c,edges[c][0])) && data[make_pair(c,edges[c][0])][2] == t)
                d.push_back(c);
        }
    }
}

int findEdgeLeaves(set<int> components,
                    map<int,vector<int>> edges,
                    set<pair<int,int>> &r,
                    set<pair<int,int>> &pq,
                    set<int> &verticesR){
    int return_costR = 0; //costo de retornar de las hojas R
    for(auto const &c : components){
        if((int)edges[c].size() == 1){
            pair<int,int> edge = {c,edges[c][0]};
            if (data[edge][2] < 1) pq.insert(edge);
            else if (data[edge][2] == 1){ 
                r.insert(edge);
                return_costR += data[edge][0];
                verticesR.insert(c);
            }
        }
    }
    return return_costR;
}

void insertData(int i,int j, int c,int b, bool isP){
    data[make_pair(i,j)].push_back(c);
    data[make_pair(i,j)].push_back(b);
    //is P, R or Q
    if(!isP){
        edgesRQ[i].push_back(j);
        if(c*2 <= b){
            data[make_pair(i,j)].push_back(1); //R
            edgesR[i].push_back(j);
        }
        else data[make_pair(i,j)].push_back(0); //Q
    }
    else data[make_pair(i,j)].push_back(-1); //P

    edges[i].push_back(j);

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

    /********* QR **********/
    vector<set<int>> componentsRQ;
    cout << "\nComponentes conexas (R unido Q) --"<<endl;
    dfsComponents(edgesRQ,componentsRQ);
    printComponents(componentsRQ);
    for(auto const &comp : componentsRQ){
        cout<<endl;

        set<pair<int,int>> edge_path; // edges of actual path
        set<pair<int,int>> edge_leavesPQ; //Aristas de hojas que generan perdidas
        set<pair<int,int>> edge_leavesR; //Aristas de hojas que no generan perdidas
        set<int> leavesR; // belonged leaves to R
        int benefit; // path benefit 
        int cost_leavesR; // return cost from belonged leaves to R
        map<int,pair<int,int>> degrees; //vertices degree (salen,entran)
        vector<deque<int>> paths;

        //Obtenemos las hojas que perteneces a R y a QP
        cost_leavesR = findEdgeLeaves(comp,edgesRQ,edge_leavesR,edge_leavesPQ,leavesR);
        //cout<<"EdgeLeaves PQ: ";
        //printSetOfPair(edge_leavesPQ);

        //Corremos dijkstra para obtener el arbol de maximo beneficio
        benefit = dijkstra(comp,edgesRQ,*comp.begin(),edge_path,degrees,paths);    
        cout<<"PATH: "; printSetOfPair(edge_path);
        cout<<"Beneficio = "<<benefit<<endl;


        cout<<"Caminos: \n"; 
        for(auto const &p : paths){
            for(auto const &v : p){
                cout<<v<<" "; 
            }
            cout<<endl;
        }

        cout<<endl;

    }

    return 0;
}