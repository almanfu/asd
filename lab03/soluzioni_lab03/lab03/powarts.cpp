#include <bits/stdc++.h>
using namespace std;

#ifdef EVAL
    FILE * in= fopen("input.txt","r");
    FILE * out= fopen("output.txt","w");
#else
    FILE * in = stdin;
    FILE * out = stdout;
#endif

typedef pair<int,int> ii;
typedef long long ll;
typedef pair<int, ll> il;
typedef vector<ll> vll;
#define NONE -1
#define CURRENT 0
#define COST 1
#define PARENT 2

struct cmp{
    bool operator()(vll a, vll b){
        return a[COST] > b[COST];
    }
};

//ritorno il dag dei cammini minimi
vector<vector<int>> dijkstra(vector<vector<il>> &graph, int source){
    priority_queue<vll, vector<vll>, cmp> q;
    vector<bool> visited(graph.size(), false);
    vector<vector<int>> dag(graph.size());
    vll distance(graph.size(), LLONG_MAX);

    distance[source] = 0;
    vll s = {source, 0, NONE}; 
    // 0 -> nodo corrente,
    // 1 -> costo del cammino fino a questo nodo
    // 2 -> padre nel cammino corrente
    q.push(s);

    while(!q.empty()){
        vll node = q.top(); q.pop();

        //costruzione grafo: importante notare che se un nodo è raggiunto da più cammini minimi si forma un DAG, altrimenti un albero
        if(distance[node[CURRENT]] == node[COST] && node[PARENT] != NONE) 
            dag[node[PARENT]].push_back(node[CURRENT]);

        if(!visited[node[CURRENT]]){
            visited[node[CURRENT]] = true;
            for(il &adj : graph[node[CURRENT]])
                if(!visited[adj.first]){
                    ll new_dist = (ll)distance[node[CURRENT]] + (ll)adj.second;
                    if(new_dist <= distance[adj.first]){ //importante il <= : con solo < si perdono gli archi che rendono l'albero dei cammini minimi un DAG
                        distance[adj.first] = new_dist;
                        vll to_push = {adj.first, distance[adj.first], node[CURRENT]};
                        q.push(to_push);
                    }
                }
        }
    }

    return dag;
}

void top_sort_dfs(vector<vector<int>> &dag, stack<int> &top_sort_stack, vector<bool> &visited, int source){
    visited[source] = true;
    for(int n : dag[source])
        if(!visited[n])
            top_sort_dfs(dag, top_sort_stack, visited, n);
    top_sort_stack.push(source);
}

stack<int> top_sort(vector<vector<int>> &dag, int source){
    vector<bool> visited(dag.size(), false);
    stack<int> top_sort_stack;
    top_sort_dfs(dag, top_sort_stack, visited, source);
    return top_sort_stack;
}

vector<int> get_list(vector<int> &dominated_nodes, vector<int> &dominator, int solution){
    int node = NONE;
    vector<int> list;
    for(int i=0; i<dominated_nodes.size() && node == NONE; i++)
        if(dominated_nodes[i] == solution)
            node = i;
    
    for(int i=0; i<dominator.size(); i++)
        if(dominator[i] == node)
            list.push_back(i);

    return list;
}

pair<int, vector<int>> solve(vector<vector<int>> &dag, stack<int> &top_sort_stack){
    vector<int> dominated_nodes(dag.size(), 0); // quanti nodi sono dominati dal nodo i-esimo
    vector<int> dominator(dag.size(), NONE);    // chi è il dominator del nodo i-esimo

    for(int adj : dag[top_sort_stack.top()]){ //per i vicini della sorgente
        dominator[adj] = adj;
        dominated_nodes[adj]++;
    }
    top_sort_stack.pop(); //tolgo la sorgente

    while(!top_sort_stack.empty()){
        int node = top_sort_stack.top(); top_sort_stack.pop();
        for(int adj : dag[node])
            if(dominator[adj] == NONE){ //è la prima volta che vedo adj
                dominator[adj] = dominator[node];
                dominated_nodes[dominator[node]]++;
            }else if(dominator[adj] != dominator[node] && dominator[adj] != adj){ //adj ha almeno due padri con dominator diversi, quindi diventa dominator a sua volta
                dominated_nodes[dominator[adj]]--;
                dominator[adj] = adj;
                dominated_nodes[adj]++;
            }
    }

    int result = *max_element(dominated_nodes.begin(), dominated_nodes.end());
    vector<int> list = get_list(dominated_nodes, dominator, result);

    return make_pair(result, list);
}

int main(){
    int n, m, s, u, v;
    ll w;
    fscanf(in, "%d %d %d", &n, &m, &s);

    vector<vector<il>> graph(n);
    vector<vector<int>> dag;

    for(int i=0; i<m; i++){
        fscanf(in, "%d %d %lld", &u, &v, &w);
        graph[u].push_back(make_pair(v, w));
        graph[v].push_back(make_pair(u, w));
    }

    if(graph[s].size() == 0){
        fprintf(out, "0\n");
        return 0;
    }

    dag = dijkstra(graph, s);
    stack<int> top_sort_stack = top_sort(dag, s);

    pair<int, vector<int>> sol = solve(dag, top_sort_stack);

    int result = sol.first;
    vector<int> list = sol.second;

    fprintf(out, "%d\n", result);
    for(int el : list)
        fprintf(out, "%d\n", el);
    
    return 0;
}
