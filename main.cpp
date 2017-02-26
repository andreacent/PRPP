/*  
    Proyecto I - Diseno de algoritmos I
    Isaac Gonzalez  11-10396
    Andrea Centeno  10-10138
*/

#include "print.h"

map<int, set<int>> edges; //Adjacency list
map<int, set<int>> edgesRQ; //Adjacency list RQ set
map<int, set<int>> edgesR; //Adjacency list R set
map<pair<int, int>, vector<int>> data; //Edge data

/*Return true if the vertice is in any set of the vector c*/
bool isInSet(deque<component> c,int vertice){
    for (auto const &s : c) {
        if(s.vertices.count(vertice)) return true;
    }
    return false;
}

/*Function used by dfsComponents*/
void getComponents( int v, int i,
                    map<int, set<int>> edges,
                    deque<component> &components){
    for(auto const &s : edges[v]){
        pair<int,int> ec = {v,s};
        bool is_leaf = (int)edges[s].size() == 1;
        if((int)components.size() <= i){
            set<int> comp = {s};
            if(is_leaf) components.push_back({comp,{s},data[ec][1] - data[ec][0]});
            else components.push_back({comp,{},data[ec][1] - data[ec][0]});
            getComponents(s,i,edges,components);
        }else if(!components[i].vertices.count(s)){
            components[i].vertices.insert(s);
            components[i].benefit += data[ec][1] - data[ec][0];
            if(is_leaf) components[i].leaves.insert(s);
            getComponents(s,i,edges,components);
        }
    }
}

/*dfs algorithm to find all connected components of set*/
void dfsComponents( map<int, set<int>> edges,
                    deque<component> &components){ 
    int i =0;
    for (auto const &s : edges) {
        if(!isInSet(components,s.first)){ 
            getComponents(s.first,i,edges,components);
            i++;
        }
    }
}

bool fixPaths(deque<deque<int>> &paths){
    bool change = false;
    set<int> del;
    for(int i=1; i<(int)paths.size(); i++){
        if(paths[0].back() == paths[i].front() || paths[0].front() == paths[i].front()){
            for(unsigned k=0;k<paths[i].size();k++){
               paths[0].push_back(paths[i][k]);
            }
            del.insert(i);
            change = true;
        }else if(paths[0].front() == paths[i].back() || paths[0].back() == paths[i].back()){
            for(unsigned k=paths[i].size()-1;k==0;k++){
               paths[0].push_front(paths[i][k]);
            }
            del.insert(i);
            change = true;
        }
    }
    for(auto const &c : del){
        paths.erase(paths.begin()+c);
    }
    return change;
}

int dijkstra(   int s,
                map<int, set<int>> edges, 
                set<pair<int,int>> &edge_path,
                deque<deque<int>> &paths){ 

    priority_queue<edgeCost,vector<edgeCost>,CompareBenefit> edge_cost; //aristas por orden de beneficio
    int sum =0;
    paths.push_back({s});

    for(auto const &v : edges[s]){
        edgeCost ec = {{s,v}, data[ec.edge][1] - data[ec.edge][0]};
        edge_cost.push(ec);
    }
    
    while(!edge_cost.empty()){
        edgeCost edc = edge_cost.top();
        edge_cost.pop();

        if( edge_path.count(edc.edge) > 0 || edge_path.count(make_pair(edc.edge.second,edc.edge.first)) > 0) continue; //no repetir arista dirigida
        
        bool back = true;
        int element;
        unsigned i=0;
        for(i=0;i<paths.size();i++){
            if(paths[i].back() == edc.edge.first){
                element = edc.edge.second;
                break;
            }else if(paths[i].front() == edc.edge.first){
                element = edc.edge.second;
                back = false;
                break;
            }else if(paths[i].back() == edc.edge.second){
                element = edc.edge.first;
                break;
            }else if(paths[i].front() == edc.edge.second){
                element = edc.edge.first;
                back = false;
                break;
            }
        }
        if(i == paths.size()) paths.push_back({edc.edge.first,edc.edge.second});
        else if(back) paths[i].push_back(element);
        else paths[i].push_front(element);

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

    //fixPaths(paths);

    return sum;
}

int kruskal(set<int> components,
            map<int, set<int>> edges, 
            set<pair<int,int>> &edge_path,
            deque<deque<int>> &paths){ 

    priority_queue<edgeCost,vector<edgeCost>,CompareBenefit> edge_cost; //aristas por orden de beneficio
    int sum =0;

    for(auto const &c : components){
        for(auto const &v : edges[c]){
            edgeCost ec;
            ec.edge = {c,v};
            ec.cost = data[ec.edge][1] - data[ec.edge][0];
            edge_cost.push(ec);
        }
    }
    
    while(!edge_cost.empty()){
        edgeCost edc = edge_cost.top();
        edge_cost.pop();

        if( edge_path.count(edc.edge) > 0 || edge_path.count(make_pair(edc.edge.second,edc.edge.first)) > 0) continue; //no repetir arista dirigida
        
        //CREO EL ARBOL
        bool back = true;
        int element;
        unsigned i=0;
        for(i=0;i<paths.size();i++){
            if(paths[i].back() == edc.edge.first){
                element = edc.edge.second;
                break;
            }else if(paths[i].front() == edc.edge.first){
                element = edc.edge.second;
                back = false;
                break;
            }else if(paths[i].back() == edc.edge.second){
                element = edc.edge.first;
                break;
            }else if(paths[i].front() == edc.edge.second){
                element = edc.edge.first;
                back = false;
                break;
            }
        }
        if(i == paths.size()) paths.push_back({edc.edge.first,edc.edge.second});
        else if(back) paths[i].push_back(element);
        else paths[i].push_front(element);

        edge_path.insert(edc.edge);
        sum+=edc.cost;
    } 

    //while(fixPaths(paths));

    return sum;
}

void insertData(int i,int j, int c,int b, bool isP){
    data[make_pair(i,j)].push_back(c);
    data[make_pair(i,j)].push_back(b);
    //is P, R or Q
    if(!isP){
        edgesRQ[i].insert(j);
        if(c*2 <= b){
            data[make_pair(i,j)].push_back(1); //R
            edgesR[i].insert(j);
        }
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


//s: Hoja origen de la componente[0]
//edges: conjunto de aristas de PQR
//vertices: hojas de todos menos mi componente conexa
//Veo adyacencias de vertice origen
//Retorno Arista y Componente
pair<int,int> Discover_connections(int s,map<int,set<int>> edges, deque<component> components){
    int total, max = 0, v = -1;

    for(auto const &c : components){
        for(auto const &l : c.leaves){

            //Si existe conexion
            if((int)edges[l].count(s)){
                pair<int,int> ls;
                total = c.benefit +(data[ls][1] - 2*data[ls][0]);
                if (total>max){
                    max = total;
                    v = l;
                }
            }
        }
    }
    return make_pair(max,v);
}
//unir conjuntos de vertices de las dos componentes
//componente[0].leaves le quitamos la hoja por la que nos conectamos
//componente[i].leaves le quitamos la hoja por la que nos conectamos
//unimos componente[0].leaves con componente[i].leaves
//sumamos las ganancias 
//borramos del deque la componente[i] --- componente.erase(componente.begin()+i);
void join_byleaves(int i,int j, deque<component> components){
    int t = 0;
    for (auto const &c : components) {

        if(c.leaves.count(j)){

            //join vertices
            components[0].vertices.insert(c.vertices.begin(), c.vertices.end());

            //join leaves
            components[0].leaves.insert(c.leaves.begin(), c.leaves.end());

            //remove leave from 0
            components[0].leaves.erase(j);

            //Update benefit
            components[0].benefit += c.benefit;

            //remove component c
            components.erase(components.begin()+t);
            break;

        }
        t++;
    }
}

int Connect(deque<component> &components, map<int,set<int>> edges){
    set<int> not0_leaves;
    pair<int,int> max_j;
    //Itero sobre hojas de componente[0]
    for (auto const &i: components[0].leaves){
        max_j = Discover_connections(i, edges, components);
        if (max_j.first > 0){
            //Uno componentes
            join_byleaves(i, max_j.second, components);
            //Agregar arista (i , j) a edgesR -- (i,j) y (j,i)
            //add_edge(i,j, edges);
        }
    }
    return -1;

}

void algorithm(deque<component> components, map<int,set<int>> edges, deque<path> &paths_data, map<int,set<int>> alledges){

    for(auto const &comp : components){
        cout<<endl;

        set<pair<int,int>> edge_path; // edges of actual path
        int benefit; // path benefit 
        deque<deque<int>> paths;

        //Corremos dijkstra para obtener el arbol de maximo beneficio
        benefit = dijkstra(*comp.vertices.begin(),edges,edge_path,paths);
        paths_data.push_back({edge_path,comp.leaves,benefit});  
        //benefit = kruskal(comp,edges,edge_path,paths); 

        cout<<"PATH: "; printSetOfPair(edge_path);
        cout<<"Beneficio = "<<benefit<<endl;
        cout<<"Caminos: \n"; 
        for(auto const &p : paths){
            for(auto const &v : p){
                cout<<v<<" "; 
            }
            cout<<endl;
        }
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

    //printData(data);

    /********* QR *********
    deque<component> componentsRQ;
    deque<path> paths_data;
    cout << "\nComponentes conexas R --"<<endl;
    dfsComponents(edgesRQ,componentsRQ);
    printConnectedComponent(componentsRQ);
    algorithm(componentsRQ, edgesRQ,paths_data,edges);
    //printComponents(paths_data);
    */

    /********* R **********/
    deque<component> componentsR;
    deque<path> paths_data;
    cout << "\nComponentes conexas R --"<<endl;
    dfsComponents(edgesR,componentsR);
    printConnectedComponent(componentsR);
    algorithm(componentsR, edgesR,paths_data,edges);
    //printComponents(paths_data);

    return 0;
}