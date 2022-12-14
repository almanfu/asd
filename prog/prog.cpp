#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <set>

#include <algorithm>

using namespace std;

const bool LOCAL = false;
ofstream out("output.txt");


/*

*/

class Graph{
  public:
  typedef int Node;
  bool hasVentola;
  const Node NULLNODE = -1;
  Node vs, ve;
  Node i, s, f;
  int wmin, wmax;

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

  void dijkstra(Node r){
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
          distance[e.node] = altDistance;
          q.push(Li(distance[e.node], e.node));
        }else if (distance[e.node] == altDistance){
          //Aggiungo nuovo lato
          preds[e.node].push_front(u);
        }
      }
    }
    /*Creo grafo cammini costo minimi
    Graph G(n, Graph::DIRECTED, Graph::UNWEIGHTED);
    for(Node v: G.V){
      for(Node u: preds[v])
        G.insertEdge(u, v);
    }*/

    // First output line
    if(distance[i] == distance[s]){
      out << '0' << endl;
    }else if(distance[i] < distance[s]){
      out << '1' << endl;
    }else if(distance[i] > distance[s]){
      out << '2' << endl;
    }

    //Second output line
    out << distance[i] << ' ' << distance[s] << endl;


    //Third output line
    out << endl;

    int R=1;
    Node cursor = i;
    while(cursor != f){
      R++;
      cursor = *preds[cursor].begin();
    }

    //Fourth output line
    out << R << endl;

    //Fifth output line
    cursor = i;
    while(cursor != f){
      out << cursor << ' ';
      cursor = *preds[cursor].begin();
    }
    out << f;

    return;
  }

};

int main(int argc, char *argv[]){
  ifstream in("input.txt");

  Graph::Node i, s, f;
  int n, m, k;
  in >> n >> m >> k;
  in >> i >> s >> f;
  
  Graph g(n, Graph::DIRECTED, Graph::WEIGHTED);

  g.i = i;
  g.s = s;
  g.f = f;

  //Inserisco direttamente il grafo trasposto
  for(int i=0; i < m; i++){
    Graph::Node u, v;
    int weight;
    in >> u >> v >> weight;
    g.insertEdge(v, u, weight);
  }

  if(k==0){
    g.hasVentola = false;
    g.vs=0;
    g.ve=0;
    //Mi serve dijkstra per f->i ed f->s
    g.dijkstra(f);
  }
  else{
    g.hasVentola = true;
    //e' tutto trasposto
    in >> g.ve >> g.vs >> g.wmin >> g.wmax;
  }



  in.close();
  out.close();
  return 0;
}
