#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <stack>

using namespace std;

/*

*/

class Graph{
  public:
  typedef int Node;
  enum GraphType {DIRECTED=false, UNDIRECTED=true};

  vector<list<Node>> adj;
  vector<vector<int>> shortest;
  int n;
  GraphType _graphType;

  Graph(int nodes, GraphType gType){
    _graphType = gType;
    n = nodes;
    adj = vector<list<Node>>(n);
  }
  ~Graph(){}
  bool has(Node u){
    return 0<=u && u < n;
  }
  const GraphType graphType(){
    return _graphType;
  }
  const bool undirected(){
    return _graphType == UNDIRECTED;
  }
  void insertEdge(Node u, Node v){
    if(has(u) && has(v) && u != v){
      adj[u].push_front(v);
      if(undirected())
        adj[v].push_front(u);
    }
  }
  // Utility Functions
  void print_adj(Node u){
    if(has(u)){
      cout << u << ':';
      for(Node v: adj[u])
        cout << v << ' ';
      cout << endl;
    }
  }
  void print_adj(){
    for(Node u=0; u < n; u++){
      cout << u << ':';
      for(Node v: adj[u])
        cout << v << ' ';
      cout << endl;
    }
  }

  void bfs(Node r){
    queue<Node> q;
    vector<bool> visited = vector<bool>(n, false);
    if(has(r)){
      q.push(r);
      visited[r] = true;
    }
    while(!q.empty()){
      Node u = q.front(); q.pop();
      cout << u << ' ';
      for(Node v: adj[u]){
        if(!visited[v]){
          q.push(v);
          visited[v] = true;
        }
      }
    }
    cout << endl;
  }

  void dfs(Node r){
    stack<Node> s;
    vector<bool> visited = vector<bool>(n, false);
    if(has(r)){
      s.push(r);
      visited[r] = true;
    }
    while(!s.empty()){
      Node u = s.top(); s.pop();
      cout << u << ' ';
      for(Node v: adj[u]){
        if(!visited[v]){
          s.push(v);
          visited[v] = true;
        }
      }
    }
    cout << endl;
  }

  vector<int> distance(Node r){
    queue<Node> q;
    vector<int> _distance(n);
    for(Node u=0; u < n; u++)
      _distance[u] = -1;
    if(has(r)){
      q.push(r);
      _distance[r] = 0;
    }
    while(!q.empty()){
      Node u = q.front(); q.pop();
      for(Node v: adj[u]){
        if(_distance[v] == -1){
          _distance[v] = 1+_distance[u];
          q.push(v);
        }
      }
    }
    return _distance;  
  }

  vector<int> ccs(){
    if(!undirected())
      return vector<int>();

    vector<int> cc = vector<int>(n, -1);
    int i = 0;
    for(Node r=0; r < n; r++){
      if(cc[r] == -1){
        //BFS
        queue<Node> q;
        q.push(r);
        cc[r] = i;
        while(!q.empty()){
          Node u = q.front(); q.pop();
          for(Node v: adj[u]){
            if(cc[v] == -1){
              q.push(v);
              cc[v] = i;
            }
          }
        }
        i++;
      }
    }
    return cc;
  }

  void setup(){// calculate all pairs shortest-path
    shortest = vector<vector<int>>(n);
    for(Node r=0; r < n; r++)
      shortest[r] = distance(r);
  }

  int cicli(Node u, Node v){
    return shortest[u][v];
  }

};

/*

*/

int main(int argc, char *argv[]){
  ifstream in("input.txt");
  ofstream out("output.txt");

  

  int n, m, q;
  in >> n >> m >> q;
  
  Graph g(n, Graph::UNDIRECTED);

  for(int i=0; i < m; i++){
    Graph::Node u, v;
    in >> u >> v;
    g.insertEdge(u, v);
  }

  g.setup();

  for(int i=0; i<q; i++){
    Graph::Node u, v;
    in >> u >> v;
    out << g.cicli(u, v) << endl;
  }

  in.close();
  out.close();
  return 0;
}