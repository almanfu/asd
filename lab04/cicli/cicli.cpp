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
  Node root;
  vector<Node> parent;
  vector<Node> level;
  int n;
  GraphType _graphType;
  bool _hasCycle;

  Graph(int nodes, GraphType gType){
    _graphType = gType;
    n = nodes;
    adj = vector<list<Node>>(n);
    parent = vector<Node>(n);
    level = vector<Node>(n);
    _hasCycle = false;
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
  const bool hasCycle(){
    return _hasCycle;
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
      root = r;
      visited[r] = true;
      parent[r] = -1;
      level[r] = 0;
    }
    while(!q.empty()){
      Node u = q.front(); q.pop();
      //cout << u << ' ';
      for(Node v: adj[u]){
        if(visited[v] && v != parent[u])
          _hasCycle = true;

        if(!visited[v]){
          q.push(v);
          visited[v] = true;
          parent[v] = u;
          level[v] = level[u]+1;
        }
      }
    }
    //cout << endl;
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

  int distance(Node r, Node d){
    queue<Node> q = queue<Node>();
    vector<int> _distance(n, -1);
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
          if(v == d){
            q = queue<Node>();
            break;
          }
        }
      }
    }
    return _distance[d];  
  }

  int distance_tree(Node s, Node d){
    queue<Node> q = queue<Node>();
    int i=0;
    Node sanc = s;
    Node danc = d;
    while(level[s]-i > level[d]){
      sanc = parent[sanc];
      i++;
    }
    i=0;
    while(level[d]-i > level[s]){
      danc = parent[danc];
      i++;
    }

    while(sanc != danc){
      sanc = parent[sanc];
      danc = parent[danc];
    }

    return level[s]+level[d]-2*level[sanc];      
  }

  int cicli(Node s, Node d){
    if(hasCycle()){
      return distance(s, d);
    }else{
      return distance_tree(s, d);
    }
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

  // setup
  g.bfs((n-1)/2);

  //cout << g.hasCycle() ;

  for(int i=0; i<q; i++){
    Graph::Node r, d;
    in >> r >> d;
    out << g.cicli(r, d) << endl;
  }

  in.close();
  out.close();
  return 0;
}