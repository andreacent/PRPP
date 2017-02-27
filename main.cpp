/*  
    Proyecto I - Diseno de algoritmos I
    Isaac Gonzalez  11-10396
    Andrea Centeno  10-10138
*/

#include "print.h"
#include <algorithm>

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
                    deque<component> &components,
                    set<pair<int,int>> &edges_comp){
    for(auto const &s : edges[v]){
        pair<int,int> ec = {v,s}; //arista
        bool is_leaf = (int)edges[s].size() == 1; //llega a una hoja?

        if((int)components.size() <= i){//creo la componente si no existe
            set<int> comp = {s};
            edges_comp.insert(ec);

            if(is_leaf) components.push_back({comp,{s},data[ec][1] - data[ec][0]});
            else components.push_back({comp,{},data[ec][1] - data[ec][0]});

            getComponents(s,i,edges,components,edges_comp);
        }
        else if(!components[i].vertices.count(s)){//agrego a una componente existente
            components[i].vertices.insert(s);

            if(is_leaf) components[i].leaves.insert(s);

            if(!edges_comp.count(ec) && !edges_comp.count(make_pair(s,v))) 
                components[i].benefit += data[ec][1] - data[ec][0]; //si la arista no ha sido agregada a la componente, sumo su beneficio

            edges_comp.insert(ec);

            getComponents(s,i,edges,components,edges_comp);
        }
    }
}

/*dfs algorithm to find all connected components of set*/
void dfsComponents( map<int, set<int>> edges,
                    deque<component> &components){ 
    int i =0;
    set<pair<int,int>> edges_comp; //aristas de la componente
    for (auto const &s : edges) {
        if(!isInSet(components,s.first)){ 
            getComponents(s.first,i,edges,components,edges_comp);
            i++;
            edges_comp.clear(); //limpio el conjunto de aristas 
        }
    }
}

bool fixPaths(deque<deque<int>> &paths){
    bool change = true;
    set<int> del;
    for(int i=1; i<(int)paths.size(); i++){
        if(paths[0].back() == paths[i].front()){
            paths[i].pop_front();
            while(!paths[i].empty()){
                int v = paths[i].front();
                paths[i].pop_front();
                paths[0].push_back(v);                
            }
        }else if(paths[0].front() == paths[i].front()){
            paths[i].pop_front();
            while(!paths[i].empty()){
                int v = paths[i].front();
                paths[i].pop_front();
                paths[0].push_front(v);                
            }
        }else if(paths[0].front() == paths[i].back()){
            paths[i].pop_back();
            while(!paths[i].empty()){
                int v = paths[i].back();
                paths[i].pop_back();
                paths[0].push_front(v);                
            }
        }else if(paths[0].back() == paths[i].back()){
            paths[i].pop_back();
            while(!paths[i].empty()){
                int v = paths[i].back();
                paths[i].pop_back();
                paths[0].push_back(v);                
            }
        }else{
            change = false;
        }
        if(change) del.insert(i);
    }
    for(auto const &c : del) paths.erase(paths.begin()+c);
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

        //no repetir arista dirigida
        if( edge_path.count(edc.edge) > 0 || edge_path.count(make_pair(edc.edge.second,edc.edge.first)) > 0 ) continue; 
        //las hojas, cuyas aristas no pertenecen al conjunto R, se ignoran
        if( data[edc.edge][2] < 1 && (edges[edc.edge.first].size() == 1 || edges[edc.edge.second].size() == 1) ) continue;
        
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

int kruskal(component component,
            map<int, set<int>> edges, 
            set<pair<int,int>> &edge_path,
            deque<deque<int>> &paths){ 
    priority_queue<edgeCost,vector<edgeCost>,CompareBenefit> edge_cost; //aristas por orden de beneficio
    deque<set<int>> sets;
    int sum =0;
    cout<<"tamano de component "<<component.vertices.size()<<endl;

    for(auto const &c : component.vertices){

        cout<<"ENTRA";

        set<int> set_c = {c};
        sets.push_back(set_c);
        for(auto const &v : edges[c]){

            if(component.vertices.count(v)){
                edgeCost ec;
                ec.edge = {c,v};
                ec.cost = data[ec.edge][1] - data[ec.edge][0];
                edge_cost.push(ec);
            }
        }
    }

    cout<<"ya tengo la cola";
    while(!edge_cost.empty()){
        edgeCost edc = edge_cost.top();
        edge_cost.pop();

        if( edge_path.count(edc.edge) > 0 || edge_path.count(make_pair(edc.edge.second,edc.edge.first)) > 0) continue; //no repetir arista no-dirigida

        int i,j;
        //obtener conjunto de edc.edge.first
        for(i=0; i<(int)sets.size(); i++){
            if(sets[i].count(edc.edge.first)) break;
        }
        //obtener conjunto de edc.edge.second
        for(j=0; j<(int)sets.size(); i++){
            if(sets[j].count(edc.edge.second)) break;
        }

        if( i==j ) continue; //si estan en el mismo conjunto, no hago nada

        edge_path.insert(edc.edge); //agrego arista al camino
        sum+=edc.cost; //sumo el beneficio

        //uno el conjunto j a i
        sets[i].insert(sets[j].begin(), sets[j].end());
        //elimino el conjunto de j
        sets.erase(sets.begin()+j);

        //camino
        bool back = true;
        int element;
        unsigned k=0;
        for(k=0;k<paths.size();k++){
            if(paths[k].back() == edc.edge.first){
                element = edc.edge.second;
                break;
            }else if(paths[k].front() == edc.edge.first){
                element = edc.edge.second;
                back = false;
                break;
            }else if(paths[k].back() == edc.edge.second){
                element = edc.edge.first;
                break;
            }else if(paths[k].front() == edc.edge.second){
                element = edc.edge.first;
                back = false;
                break;
            }
        }
        if(k == paths.size()) paths.push_back({edc.edge.first,edc.edge.second});
        else if(back) paths[k].push_back(element);
        else paths[k].push_front(element);
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
//all_edges: conjunto de aristas de PQR
//vertices: hojas de todos menos mi componente conexa
//Veo adyacencias de vertice origen
//Retorno Arista y Componente
pair<int,int> discoverConnections(int s,map<int,set<int>> all_edges, deque<component> components){
    int total, max = 0, v = -1;

    for (int i=1; i<(int)components.size(); i++){
        for(auto const &l : components[i].leaves){

            //Si existe conexion
            if((int)all_edges[l].count(s)){
                pair<int,int> ls = {l,s};
                total = components[i].benefit +(data[ls][1] - 2*data[ls][0]);
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
void join_byleaves(int i,int j, deque<component> &components){
    int t = 1;
    for (int k=1; k<(int)components.size(); k++) {

        if(components[k].leaves.count(j)){
            //join vertices
            components[0].vertices.insert(components[k].vertices.begin(), components[k].vertices.end());

            //join leaves
            components[0].leaves.insert(components[k].leaves.begin(), components[k].leaves.end());

            //remove leave from i from 0
            components[0].leaves.erase(i);

            //remove leave from j from 0
            components[0].leaves.erase(j);

            //Update benefit
            components[0].benefit += components[k].benefit;

            //remove component c
            components.erase(components.begin()+t);
            break;

        }
        t++;
    }
}

void connect(deque<component> &components, 
            map<int, set<int>> &edges, 
            map<int,set<int>> all_edges){
    set<int> leaves;
    set<int> visited_leaves;
    pair<int,int> max_j;
    set<int>::iterator it;
    int j=0;

    leaves = components[0].leaves;
    //for (int x= 0; x<4; x++){
    while (!leaves.empty()){
        it = leaves.begin();
        max_j = discoverConnections(*it, all_edges, components);

        if (max_j.first > 0){
            j++;
            //Uno componentes
            join_byleaves(*it, max_j.second, components);

            //Agregar arista a edges
            //(i,j)
            edges[*it].insert(max_j.second);
            //(j,i)
            edges[max_j.second].insert(*it);
        }

        visited_leaves.insert(*it);

        //Resta conjuntos leaves-visited_leaves
        leaves.clear();
        std::set_difference(components[0].leaves.begin(), components[0].leaves.end(), visited_leaves.begin(), visited_leaves.end(), 
                        inserter(leaves, leaves.begin()));

    }
    cout << "Joined "<< j <<" leaves"<<endl;
}

int go_Deposit(int l, component &component0,map<int,set<int>> &edges, set<pair<int,int>> &edge_path, deque<int> &path){
    int max,ve,costo,benefit;
    max = -999999;
    benefit=0;
    for(auto const &v : component0.vertices){
        if (v != l && l !=0){
            if((int)edges[l].count(v)){
                if (v == 0){
                    if ((int)edge_path.count(make_pair(l,v)) || (int)edge_path.count(make_pair(v,l))){
                        benefit += data[make_pair(l,v)][1] - 2*data[make_pair(l,v)][0];
                    }
                    else{
                        benefit = data[make_pair(l,v)][1]- data[make_pair(l,v)][0];
                    }
                    edge_path.insert(make_pair(l,0));
                    edge_path.insert(make_pair(0,l));
                    path.push_back(l);
                    path.push_back(0);
                    return benefit;
                }
                if ((int)edge_path.count(make_pair(l,v)) || (int)edge_path.count(make_pair(v,l))){
                    costo = data[make_pair(l,v)][1] - 2*data[make_pair(l,v)][0];
                    if (max < costo){
                        max = costo;
                        ve = v;
                    }
                }
                else{
                    costo = data[make_pair(l,v)][1] - data[make_pair(l,v)][0];
                    if (max < costo){
                        max = costo;
                        ve = v;
                    }
                }
            benefit += max;
            edges[l].erase(ve);
            edges[ve].erase(l);
            edge_path.insert(make_pair(l,ve));
            edge_path.insert(make_pair(ve,l));
            path.push_back(l);
            benefit += go_Deposit(ve, component0, edges, edge_path, path);
            }
        }
    }
    return benefit;


}

int wayBack(deque<component> &components, map<int,set<int>> &edges, set<pair<int,int>> &edge_path,deque<deque<int>> &paths){
    int benefit = 0,c=0;
    for(auto const &l : components[0].leaves){
        deque<int> leave_path;
        paths.push_back(leave_path);
        benefit+=go_Deposit(l, components[0], edges, edge_path, paths[c]);
        c++;
    }
    return benefit;

}
void algorithm( map<int,set<int>> &edges, 
                map<int,set<int>> all_edges,
                string filename){

    deque<component> components;
    //calculamos las componentes conexas con su informacion
    dfsComponents(edges,components);
    printConnectedComponents(components);

    //unimos las componentes que convengan
    connect(components,edges,all_edges);
    printComponent(components.front());

    //cout<<"Edges: "<<endl;
    //printEdgesMap(edges);

    //deque<path> paths_data;
    deque<deque<int>> paths;
    set<pair<int,int>> edge_path; // edges of actual path
    int benefit = dijkstra(*components[0].vertices.begin(),edges,edge_path,paths);
    //int benefit = kruskal(components.front(), edges, edge_path, paths);

    cout<<"PATH: "; printSetOfPair(edge_path);
    cout<<"Beneficio = "<<benefit<<endl;
    cout<<"Caminos: \n"; 
    for(auto const &p : paths){
        for(auto const &v : p){
            cout<<v<<" "; 
        }
        cout<<endl;
    }

    deque<deque<int>> paths2;
    int backbenefit = wayBack(components,edges, edge_path, paths2);
    cout<<"PATH: "; printSetOfPair(edge_path);
    cout<<"backbenefit = "<<backbenefit<<endl;
    benefit += backbenefit;
    cout<<"BackCaminos: \n"; 
    for(auto const &p : paths2){
        for(auto const &v : p){
            cout<<v<<" "; 
        }
        cout<<endl;
    }

    ofstream myfile;
    myfile.open (filename+"-salida.txt");
    myfile << benefit<<".\n";
    for(auto const &p : paths){
        for(auto const &v : p){
            myfile<<v<<" "; 
        }
        myfile<<endl;
    }
    myfile.close();
}

/*
argv[1] -> intance
*/
int main(int argc, char const *argv[]) {
    int vertices,rqdEdges, nonRqdEdges;
    string filecito = argv[1];
    string filename = filecito.substr(0, filecito.find("."));

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

    /********* R **********/
    cout << "\nComponentes conexas R"<<endl;
    algorithm(edgesR, edges,filename);

    /********* QR *********
    cout << "\nComponentes conexas RQ"<<endl;
    algorithm(edgesRQ, edges);
    */

    return 0;
}