#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <set>

#include <algorithm>

using namespace std;

/*

*/

class Graph{
  public:
  typedef int Node;
  struct Edge{
    Node node;
    int weight;
    Edge(Node n, int w){
      node = n;
      weight = w;
    }
    friend bool operator<(const Edge e1, const Edge e2){
      return e1.weight < e2.weight;
    }
  };
  enum EdgeDirection {DIRECTED, UNDIRECTED};
  enum EdgeWeight {WEIGHTED, UNWEIGHTED};
  EdgeDirection edgeDirection;
  EdgeWeight edgeWeight;
  private:
  vector<Node> V;
  vector<list<Edge>> adj;
  int n;
  public:
  Graph(int nodes, EdgeDirection eDir, EdgeWeight eWeight){
    edgeDirection = eDir;
    edgeWeight = eWeight;
    n = nodes;
    adj = vector<list<Edge>>(n);
    V = vector<Node>(n);
    for(Node u=0; u < n; u++)
      V[u] = u;
  }
  ~Graph(){}
  bool has(Node u){
    return 0<=u && u < n;
  }
  bool undirected(){
    return edgeDirection == UNDIRECTED;
  }
  bool unweighted(){
    return edgeWeight == UNWEIGHTED;
  }
  void insertEdge(Node u, Node v, int weight=1){
    if(unweighted())
      weight  = 1;
    if(has(u) && has(v) && u != v && weight >= 1){
      adj[u].push_front(Edge(v, weight));
      if(undirected())
        adj[v].push_front(Edge(u, weight));
    }
  }
  // Utility Functions
  void print_adj(){
    for(Node u=0; u < n; u++){
      cout << u << ':';
      if(unweighted()){
        for(Edge e: adj[u])
          cout << e.node << ' ';
      }else{
        for(Edge e: adj[u])
          cout << '(' << e.node << ',' << e.weight << ')' << ' ';
      }
      cout << endl;
    }
  }

  void bfs(Node r){
    queue<Node> q;
    bool *visited = new bool[n]{};
    if(has(r)){
      q.push(r);
      visited[r] = true;
    }
    while(!q.empty()){
      Node u = q.front(); q.pop();
      cout << u << ' ';
      for(Edge e: adj[u]){
        if(!visited[e.node]){
          q.push(e.node);
          visited[e.node] = true;
        }
      }
    }
    cout << endl;
    delete[] visited;
  }

  Graph dijkstra(Node r){
    typedef pair<int, Node> Li;
    priority_queue<Li, vector<Li>, greater<Li>> q;
    vector<int> distance(n);
    vector<list<Node>> preds(n);
    const int INF = INT32_MAX;
    for(Node u: V)
      distance[u] = INF;
    distance[r] = 0;
    q.push(Li(0, r));
    //Dijkstra 
    while(!q.empty()){
      Li li = q.top(); q.pop();
      Node u = li.second;
      for(Edge e: adj[u]){
        int altDistance = e.weight+distance[u];
        if(distance[e.node] > altDistance){
          //Rimuovo vecchi lati
          preds[e.node].clear();
          preds[e.node].push_front(u);
          q.push(Li(distance[e.node], e.node));
        }else if (distance[e.node] == altDistance){
          //Aggiungo nuovo lato
          preds[e.node].push_front(u);
        }
      }
    }
    //Creo grafo cammini costo minimi
    Graph G(n, Graph::DIRECTED, Graph::UNWEIGHTED);
    for(Node v: G.V){
      for(Node u: preds[v])
        G.insertEdge(u, v);
    }
    return G;
  }
  void dfs(Node r){
    struct sNode{
      Node node;
      list<Graph::Edge>::iterator iter; 
      sNode(Node u, list<Graph::Edge>::iterator i){
        node = u;
        iter = i;
      }
    };
    stack<sNode*> s;
    s.push(new sNode(r, adj[r].begin()));
    int *dt = new int[n]{};
    int *ft = new int[n]{};
    int time=0;
    while(!s.empty()){
      sNode* snode = s.top(); 
      Node& u = snode->node;
      time++;
      if(ft[u] != 0){
        //Post-order visit
        delete snode;
        s.pop();
      }else if(ft[u] == 0){
        if(dt[u] == 0){
          //Pre-order visit
          dt[u] = time;
        }
        auto& iter = snode->iter;
        auto iterEnd = adj[u].end();
        while(iter!=iterEnd){
          Edge e = *iter;
          if(dt[e.node] == 0){//Tree Edge
            iter++;
            s.push(new sNode(e.node, adj[e.node].begin()));
            break;
          }else if(ft[e.node] != 0 && dt[e.node] > dt[u]){
            //Forward Edge
          }else if(ft[e.node] == 0 && dt[e.node] < dt[u]){
            //Back Edge
          }else{
            //Cross Edge
          }
        }
        if(iter == iterEnd){
          ft[u] = dt[u]==time?time+1:time;
        }
      }
    }
    delete[] dt;
    delete[] ft;
  }
};