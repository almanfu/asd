#include <bits/stdc++.h>
using namespace std;
#define MAXN 50001
#define MAXM 500001
#define MAXQ 50001
#define LOG 21 // ~log2(MAXN)

int N, M, Q;

vector<vector<int>> graph; // I suppose vertices are 0...N-1
vector<vector<int>> tree;

// for Tarjan's algorithm
vector<int> d, low; 
stack<int> s; 
int counttime, newnode;

// for LCA algorithm
vector<vector<int>> lca; // lca[i][j] = 2^j th ancestor of node i 
vector<int> h; // h[i] = height of i in the tree

///////// TARJAN /////////

// This function uses Tarjan to find cycles.
// Whenever a cycle is found, a vertex (with id = newnode) in the tree is added 
// and connected to each vertex in the cycle.
void constructTree(int u, int parent){ 
	s.push(u);
	low[u] = counttime;
	d[u] = counttime;
	counttime++;

	for(int v : graph[u]){
		if(d[v] == -1){ // not visited
			constructTree(v, u);
			low[u] = min(low[u], low[v]);
			
			if(d[u] <= low[v]){ // u articulation point, I have "closed" a cycle
				int w;
				do{
					w = s.top();
				    s.pop();
				    tree[w].push_back(newnode);
				    tree[newnode].push_back(w);
				}while (w != v);
				tree[u].push_back(newnode);
				tree[newnode].push_back(u);
				newnode++;
			}
		} 
		else if(v != parent)
			low[u] = min(low[u], d[v]);
	}
}

///////// LCA /////////

void preprocess(int u, int parent){
	lca[u][0] = parent;
	if(parent == -1)
		h[u] = 0;
	else 
		h[u] = h[parent] + 1;
	
	// compute all ancestors of u
	for (int i = 1; i<LOG; i++)
		if(lca[u][i-1] != -1)
			lca[u][i] = lca[lca[u][i-1]][i-1]; // 2^j th ancestor of node i = 
											   // 2^(j-1) th ancestor of 2^(j-1) th ancestor of node i
	
	// compute all ancestors of children
	for(int v : tree[u])
		if(v != parent)
			preprocess(v, u);
}

int getLca(int u, int v){
	if(h[u] < h[v]) 
		swap(u, v);

	// u is the node at lower level, we raise u to be at the same level as v
    int dist = h[u]-h[v]; 
	while(dist > 0){
        int raiseby = log2(dist);
        u = lca[u][raiseby];
        dist -= pow(2,raiseby);
    }

	if(u == v) return u;

    for(int j = LOG-1; j>=0; j--){
    	/*
    	We don't need the check lca[u][j] != -1. Indeed, if lca[u][j] = -1, then u is the root 
    	and since in the for loop u and v are always at the same height, this means that u = v = root. 
    	But this is impossibile since we check lca[u][j] != lca[v][j], before the assignement.
    	*/
        if(lca[u][j] != lca[v][j]){ 
            u = lca[u][j]; 
            v = lca[v][j]; 
        }
    }

	return lca[u][0]; // parent of u 
}

///////// SOLUTION /////////

int solve(int u, int v){
	return (h[u]+h[v]-2*h[getLca(u,v)])/2;
}

int main (){

    ifstream in("input.txt");
    ofstream out("output.txt");

    in >> N >> M >> Q;

    graph.resize(N+1); 
    tree.resize(N+M+1);
    d.resize(N+1,-1);
    low.resize(N+1,INT_MAX);
    lca.resize(N+M+1,vector<int>(LOG));
    h.resize(N+M+1);

    int u, v;
    for(int i = 0; i<M; i++){
        in >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    newnode = N;
    counttime = 0;
    constructTree(0, 0);
    preprocess(0, -1);

    for(int i = 0; i<Q; i++){
        in >> u >> v;
        out << solve(u,v) << endl;
    }

    return 0;
}