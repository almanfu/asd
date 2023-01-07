#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <unordered_set>

using namespace std;

/*

*/

class Graph{
  public:
  typedef int Node;
  enum GraphType {DIRECTED=false, UNDIRECTED=true};
  private:
  vector<unordered_set<Node>> adj_out;
  vector<unordered_set<Node>> adj_in;
  int n;
  GraphType _graphType;
  public:
  Graph(int nodes, GraphType gType){
    _graphType = gType;
    n = nodes;
    adj_out = vector<unordered_set<Node>>(n);
    adj_in = vector<unordered_set<Node>>(n);
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
      adj_out[u].insert(v);
      adj_in[v].insert(u);
      if(undirected())
        adj_out[v].insert(u);
    }
  }
  // Utility Functions
  void print_adj_out(Node u){
    if(has(u)){
      cout << u << ':';
      for(Node v: adj_out[u])
        cout << v << ' ';
      cout << endl;
    }
  }
  void print_adj_out(){
    for(Node u=0; u < n; u++){
      cout << u << ':';
      for(Node v: adj_out[u])
        cout << v << ' ';
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
      for(Node v: adj_out[u]){
        if(!visited[v]){
          q.push(v);
          visited[v] = true;
        }
      }
    }
    cout << endl;
    delete[] visited;
  }

  void dfs(Node r){
    stack<Node> s;
    bool *visited = new bool[n]{};
    if(has(r)){
      s.push(r);
      visited[r] = true;
    }
    while(!s.empty()){
      Node u = s.top(); s.pop();
      cout << u << ' ';
      for(Node v: adj_out[u]){
        if(!visited[v]){
          s.push(v);
          visited[v] = true;
        }
      }
    }
    cout << endl;
    delete[] visited;
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
      for(Node v: adj_out[u]){
        if(_distance[v] == -1){
          _distance[v] = 1+_distance[u];
          q.push(v);
        }
      }
    }
    return _distance;  
  }

  list<int> topsort(){// O (n*n+m)
    list<int> _topsort = list<int>();
    vector<bool> added = vector<bool>(n, false);
    while(_topsort.size() < n){
      for(Node u=0; u < n; u++){
        if(!added[u] && adj_in[u].size() == 0){
          added[u] = true;
          _topsort.push_front(u);
          for(Node v: adj_out[u])
            adj_in[v].erase(u);
          break;
        }
      }
    }
    _topsort.reverse();
    return _topsort;
  }

};

/*

*/

int main(int argc, char *argv[]){
  ifstream in("input.txt");
  ofstream out("output.txt");

  int n, m;
  in >> n >> m;
  
  Graph g(n, Graph::DIRECTED);

  for(int i=0; i < m; i++){
    Graph::Node u, v;
    in >> u >> v;
    g.insertEdge(u, v);
  }

  unordered_set<int> s = unordered_set<int>();

  for(Graph::Node u: g.topsort())
    out << u << ' ';

  in.close();
  out.close();
  return 0;
}
