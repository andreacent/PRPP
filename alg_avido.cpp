/*  
    Proyecto II - Diseno de algoritmos I
    Isaac Gonzalez  11-10396
    Andrea Centeno  10-10138
*/
#include "alg_avido.h"
    
using namespace std;

/***************************************AUXILIAR FUNCTIONS**************************************/
//Return vertex of the graph
set<int> obtenerVertices(map<int, set<int>> &edges){
	set<int> vertex;
	for(map<int, set<int>>::iterator it = edges.begin(); it != edges.end(); ++it) {
		vertex.insert(it->first);
	}
    return vertex;
}

//Check if an element exist for key 'u' in map prev
bool exist(int u, map <int,int> &prev){

	for(auto iter = prev.begin(); iter != prev.end(); ++iter){
		int k =  iter->first;
		if (k == u) return true;
	}
	return false;
}

//Check if edge belongs to solution
bool pertenece(edge &e, vector<edge> &solution){
	for (auto edge = solution.begin(); edge != solution.end(); ++edge) {

        if(e.coor.first == (*edge).coor.first){
        	if (e.coor.second == (*edge).coor.second) return true;
        }
        else if(e.coor.first == (*edge).coor.second){
        	if (e.coor.second == (*edge).coor.first) return true;
        }

	}
    return false;
}


//Calculate fake benefit (used in caminoCostoMinimo - Dijkstra)
int getfake_benefit(int u, int v, vector<edge> &solution, map<pair<int, int>, vector<int>> &data){

	pair<int,int> p = make_pair(u,v);
	edge e = {p, data[p][1], data[p][0]};

	if (pertenece(e,solution)){
		return (e.cost);
	}
	int total = (e.benefit - e.cost);
	if (total < 0){
		return -total;
	}
	else{
		return 0;
	}
}


//calculate benefit
int get_benefit (edge e, vector<edge> &solution){

	if (pertenece(e,solution)){
		return -(e.cost);
	}
	else{
		return (e.benefit - e.cost);
	}
}

//Calculate benefit of the path
int path_benefit(deque< pair<int,int>> c, vector<edge> &solution, map<pair<int, int>, vector<int>> &data){
	int benefit = 0;
	for (auto p = c.begin(); p != c.end(); ++p) {
    	edge e = {*p,data[*p][1], data[*p][0]};
        benefit += get_benefit(e, solution);
    }
    return benefit;
}

void terase(edge e, vector<edge> &t){ //ANDREA
	vector<edge>::iterator normal_e, back_e;
	bool normal_b = false;
	bool back_b = false;

	for (vector<edge>::iterator it = t.begin(); it != t.end(); ++it) {
		if (((*it).coor.first == e.coor.first) && ((*it).coor.second == e.coor.second)){
			normal_e = it;
			normal_b = true;
		}
		else if (((*it).coor.first == e.coor.second) && ((*it).coor.second == e.coor.first)){
			back_e = it;
			back_b = true;
		}

		if(normal_b && back_b) break;
    }
    if(normal_b) t.erase(normal_e);
    if(back_b) t.erase(back_e);
}

/***********************************************************************************************/

/***************************************heuristicaAvida FUNCTIONS*******************************/
//Return edge with biggest benefit
edge encontrarLado(vector<edge> &t, vector<edge> &solution){
	int max, benefit;
	edge e;
	max = MIN;
	for (auto it = t.begin(); it != t.end(); ++it) {
		benefit = get_benefit(*it, solution);
		if (max < benefit){
			max = benefit;
			e = *it;
		}
	}
    return e;
}

//Return edge with biggest benefit (b,u)
pair<edge,bool> obtenerLado(vector<edge> &t, int b, vector<edge> &solution){
	int max, benefit;
	edge e;
	max = MIN;
	for (auto it = t.begin(); it != t.end(); ++it) {
		if( (*it).coor.first == b){
			benefit = get_benefit(*it, solution);
			if (max < benefit){
				max = benefit;
				e = *it;
			}
		}
	}
    return make_pair(e,max < 0);
}

//Return path with biggest benefit
pair<deque<edge>,bool> obtenerCamino(set<deque<pair<int,int>>> &ccm, vector<edge> &solution, map<pair<int, int>, vector<int>> &data){
    int max, benefit;
    set<deque< pair<int,int> >>:: iterator it;
    deque<pair<int,int>> c;

    max = MIN;
    benefit = 0;

	for (it = ccm.begin(); it != ccm.end(); ++it) {
		benefit = path_benefit(*it, solution, data);
		if (max < benefit){
			max = benefit;
			c = *it;
		}
	}
	//create path and return it
	deque<edge> path;

	for (auto p = c.begin(); p != c.end(); ++p) {
    	edge e = {*p,data[*p][1], data[*p][0]};
		path.push_back(e);
	}

	return make_pair(path, max < 0);
}

//Check if exist a vertex u / edge(b,u)
bool exist_u(vector<edge> t, int b){
	for (auto e = t.begin(); e != t.end(); ++e){

		if ((*e).coor.first == b) return true;
	}
	return false;
}

//Check if edge e is present in t
bool exist_e(edge e, vector<edge> t){
	for (auto et = t.begin(); et != t.end(); ++et){
		if ((e.coor.first == (*et).coor.first) && (e.coor.first == (*et).coor.first)) return true;
	}
	return false;	
}

//remove edges in path rcm from set t
void rpath_fromt(deque<edge> rcm, vector<edge> &t){
	for (auto e = rcm.begin(); e != rcm.end(); ++e){
		if ( exist_e(*e,t) ) {
			terase(*e,t);
		}
	}
}

//get las vertex of the path
int get_i(deque<edge> rcm){
	return rcm.back().coor.second;
}

//Add path to solution
void unirCaminoAlCiclo(vector<edge> &solution, deque<edge> c){
	for (auto edge = c.begin(); edge != c.end(); ++edge){
    	solution.push_back(*edge);
    }
}

//Dijkstra
//@ int i: source
//@ int b: destiny
//map<int, set<int>> edges; //Adjacency list
int caminoCostoMinimo(  int source, int target, deque<pair<int,int>> &cm_bi,
							     set<int> &v, map<pair<int, int>, vector<int>> &data,
							     map<int, set<int>> edges,
							     vector<edge> &solution){

 	priority_queue<vertex,vector<vertex>,CompareVertex> q;
	map <int,int> dist, prev;
	map <int,bool> inQ;

	//Initalization
	for (set<int>::iterator it = v.begin(); it != v.end(); ++it) {
		if (*it == source){
	    	dist.insert(std::pair<int, int>(*it, 0));
	    	vertex ve = {*it, 0};
	    	q.push(ve);
		}
		else {
			dist.insert(std::pair<int, int>(*it, MAX));
	    	vertex ve = {*it, MAX};
	    	q.push(ve);
	    }

	    inQ.insert(std::pair<int, bool>(*it, true));
	}	

    while(!q.empty()){  
    	vertex u = q.top();
	    q.pop();
	    if (inQ[u.vertex]){
	    	inQ[u.vertex] = false;
		    //if target reached get path and return it
	     	if (u.vertex == target){
				int iu = u.vertex;
				while (exist(iu, prev)){
					cm_bi.push_front(make_pair(prev[iu],iu));
				    iu = prev[iu];            
				}
				return 0;
			}

			for(auto v : edges[u.vertex]) {
				if (inQ[v]){ // where v is still in Q.
		  			int alt = dist[u.vertex] + getfake_benefit(u.vertex,v,solution,data);
		  			if (alt < dist[v]){
		  				dist[v] = alt;
		  				prev[v] = u.vertex;
		    			vertex ve = {v,alt};
		                q.push(ve);
		        	}
	  			}
	  		}
	  	}
  	}
 	return -1;
}  

void printc(deque<pair<int,int>> cm){
	cout << "camino: ";
	for(auto p : cm) {
		cout << p.first<<",";
	}
	cout << cm.back().second;
	cout <<endl;
}

void printh(vector<edge> mejorSol){
	cout << "ciclo:";
	for(auto e : mejorSol) {
		cout << " ("<<e.coor.first<<","<<e.coor.second<<")";
	}
	cout <<endl;
}
bool visited(edge e, vector<pair<int,int>> control){
	for(auto p : control){
		if ((e.coor.first == p.first) && (e.coor.second == p.second)) return true;
		else if ((e.coor.second == p.first) && (e.coor.first == p.second)) return true;
	}
	return false;
}
void print_benefit(vector<edge> &solution){
	vector<pair<int,int>> control;
	int benefit = 0;

	for(auto e : solution) {
		if(visited(e,control)) benefit -= e.cost;
		else {
			benefit += (e.benefit - e.cost);
			control.push_back(make_pair(e.coor.first,e.coor.second));
		}
	}
	cout << "Beneficio de la heuristicaAvida: "<<benefit<<endl;
}
/***********************************************************************************************/

/***************************************heuristicaAvida*****************************************/
void heuristicaAvida(
		map<pair<int, int>, vector<int>> data,
        map<int, set<int>> edgesRQ,
       	map<int, set<int>> edges,
        vector<edge> &solution){

	//Initialize t
	vector<edge> t; //R unido Q
	map<int, set<int>>::iterator it_t;

	for (auto e1 = edgesRQ.begin(); e1 != edgesRQ.end(); ++e1) {
		for (auto e2 = (*e1).second.begin(); e2 != (*e1).second.end(); ++e2) {
			pair <int,int> p = make_pair((*e1).first,*e2);
			edge e = {p,data[p][1], data[p][0]};
			t.push_back(e);
		}
	}
    
	set<int> vertex = obtenerVertices(edges);

	//if(!edgesRQ.count(0)) //d==0
	//	u = encontrarLado(edgesP); //(d,u) con mayor beneficio

	int b = 0;
	while(!t.empty()){
		if(exist_u(t,b)){
			pair<edge,bool> e_bu = obtenerLado(t,b,solution);
			terase(e_bu.first,t);
			if (!e_bu.second){
				solution.push_back(e_bu.first);
				b = e_bu.first.coor.second;
			}
		}

		else{
			set<deque<pair<int,int>>> ccm;
    		for (auto e = t.begin(); e != t.end(); ++e){
    			deque<pair<int,int>> cm_bi1, cm_bi2;
    			//for each vertex
    				if (b !=(*e).coor.first){
		    			caminoCostoMinimo(b,(*e).coor.first,cm_bi1,vertex,data,edges,solution);
		    			ccm.insert(cm_bi1);
		    		}
	    			if (b !=(*e).coor.second){
		    			caminoCostoMinimo(b,(*e).coor.second,cm_bi2,vertex,data,edges,solution);
		    			ccm.insert(cm_bi2);
		    		}
    			//endfor
			}
			pair<deque<edge>,bool> rcm = obtenerCamino(ccm,solution,data);
			if (rcm.second){
			 	break;
			}
			unirCaminoAlCiclo(solution , rcm.first);

			rpath_fromt(rcm.first, t); //eliminar toda arista de rcm de T'

			b = get_i(rcm.first);
			ccm.clear();
		}
	
	}
	if(solution.back().coor.second != 0){
		deque<pair<int,int>> cm_id;
		deque<edge> rcm;

		caminoCostoMinimo(solution.back().coor.second,0,cm_id,vertex,data,edges,solution);
		for (auto p= cm_id.begin(); p != cm_id.end(); ++p) {
	    	edge e = {*p,data[*p][1], data[*p][0]};
			rcm.push_back(e);
		}
		unirCaminoAlCiclo(solution , rcm);
	}
	print_benefit(solution);
}
