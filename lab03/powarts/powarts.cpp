#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>

#include <algorithm>

using namespace std;

const bool LOCAL = true;
ofstream plt;
ofstream out("output.txt");

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

  void powarts(Node r){
    if(LOCAL)
      plt = ofstream("plt.txt");
    queue<Node> q;
    vector<int> distance(n);
    vector<list<Node>> predecessor(n);
    for(Node u=0; u < n; u++){
      distance[u] = -1;
    }
    if(has(r)){
      distance[r] = 0;
      q.push(r);
    }

    while(!q.empty()){
      Node u = q.front(); q.pop();
      for(Edge e: adj[u]){
        if(distance[e.node] == -1){
          predecessor[e.node].push_front(u);
          distance[e.node] = e.weight+distance[u];
          q.push(e.node);
        }else if(distance[e.node] > e.weight+distance[u]){
          //Rimuovo vecchi lati
          predecessor[e.node].clear();
          predecessor[e.node].push_front(u);
          distance[e.node] = e.weight+distance[u];
        }else if (distance[e.node] == e.weight+distance[u]){
          //Aggiungo nuovo lato
          predecessor[e.node].push_front(u);
        }
      }
    }

    //Creo grafo cammini costo minimi
    Graph G(n, Graph::DIRECTED, Graph::UNWEIGHTED);
    for(Node v: G.V){
      for(Node u: predecessor[v]){
        if(LOCAL)
          plt << u << ' ' << v << endl;
        G.insertEdge(u, v);
      }
    }
    if(LOCAL)
      plt << "0 0";

    //Rimuovo ogni nodo e vedo la grandezza della cc di r in G 
    int minPowarts = n;

    list<Node> attacked;

    list<Node> disabled;
    
    bool *visited = new bool[n]{};
    for(Node removed: G.V){
      if(removed != r && !G.adj[removed].empty()){
        // Modified BFS
        for(int i=0; i < n; i++)
          visited[i] = false;
        queue<Node> q;
        q.push(r);
        visited[r] = true;
        int numPowarts=1;
        while(!q.empty()){
          Node u = q.front(); q.pop();
          for(Edge e: G.adj[u]){
            if(e.node != removed && !visited[e.node]){
              q.push(e.node);
              visited[e.node] = true;
              numPowarts++;
            }
          }
        }
        if(numPowarts<minPowarts){
          minPowarts = numPowarts;
          disabled.clear();
          for(Node u: G.V){
            if(visited[u] == false)
              disabled.push_front(u);
          }
          if(LOCAL){
            attacked.clear();
            attacked.push_front(removed);
          }
        }else if(numPowarts == minPowarts){
          if(LOCAL)
            attacked.push_front(removed);
        }
      }
    }
    if(!LOCAL){
      out << (n-minPowarts) << '\n';
      for(Node u: disabled)
        out << u << '\n';
    }else{
      cout << (n-minPowarts) << '\n';
      for(Node u: attacked)
        out << u << ' ';
      plt.close();
    }

    delete[] visited;
  }
};

/*

*/

int main(int argc, char *argv[]){
  ifstream in("input.txt");

  
  Graph::Node r;
  int n, m;
  in >> n >> m >> r;
  
  Graph g(n, Graph::UNDIRECTED, Graph::WEIGHTED);

  for(int i=0; i < m; i++){
    Graph::Node u, v;
    int weight;
    in >> u >> v >> weight;
    g.insertEdge(u, v, weight);
  }

  g.powarts(r);

  if(LOCAL)
    cout << "===================" << '\n';
  in.close();
  out.close();
  return 0;
}
