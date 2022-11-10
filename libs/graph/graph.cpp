#include <iostream>
#include <fstream>

#include <bitset>
#include <vector>
#include <list>
#include <queue>
#include <deque>
#include <map>
#include <stack>
#include <set>

#include <string>
#include <cctype>

#include <utility>
#include <functional>
#include <algorithm>

using namespace std;

/*
Beware!
Some functions operate on sets, and are O(n*log(n))
*/

class Graph{
  public:
  typedef int Node;
  enum GraphType {DIRECTED=false, UNDIRECTED=true};
  private:
  vector<set<Node>*> _adj;
  set<Node> _V;
  int _n;
  GraphType _graphType;
  public:
  Graph(GraphType gType=UNDIRECTED){
    _graphType = gType;
    _n = 0;
    _adj.push_back(nullptr);
  }
  ~Graph(){
    for(Node u=1; u <= _n; u++){
      if(_adj[u] != nullptr)
        delete _adj[u];
    }
  }
  const GraphType graphType(){
    return _graphType;
  }
  const bool undirected(){
    return _graphType == UNDIRECTED;
  }
  const int n(){
    return _n;
  }
  const set<Node> V(){
    return _V;
  }
  bool has(Node u){
    return _V.find(u) != _V.end();
  }
  bool insertNode(Node u){
    if(u <= 0){
      return false;      
    }else if(_n <= u){
      for(Node v=_n; v < u; v++)
        _adj.push_back(nullptr);
      _n = u;
      _V.insert(u);
      _adj[u] = new set<Node>;
      return true;
    }else{
      _V.insert(u);
      _adj[u] = new set<Node>;
      return true;
    }
  }
  void insertEdge(Node u, Node v){
    if(has(u) && has(v) && u != v){
      _adj[u]->insert(v);
      if(undirected())
        _adj[v]->insert(u);
    }
  }
  void deleteNode(Node u){
    delete _adj[u];
    _adj[u] = nullptr;
    for(Node v: _V)
      deleteEdge(v, u);
    _V.erase(u);
  }
  void deleteEdge(Node u, Node v){
    if(has(u) && has(v) && u != v){
      _adj[u]->erase(v);
      if(undirected())
        _adj[v]->erase(u);
    }
  }
  const set<Node> adj(Node u){
    if(has(u))
      return *_adj[u];
    else
      return set<Node>();
  }
  void print_V();
  void print_adj();
  void print_adj(Node u);
  void bfs(Node r);
  vector<int> distance(Node u);
  void dfs(Node r);
  void dfs();
  bool hasCycle();
  Graph transposed();
  list<set<Node>*> ccs();
  set<Node> cc(Node r);
  void print_cc(Node r);
  void print_ccs();
};

void Graph::print_V(){
  cout << "V:";
  for(Node u: V())
    cout << u << ' ';
  cout << endl;
}

void Graph::print_adj(Node u){
  if(has(u)){
    cout << u << ':';
    for(Node v: adj(u))
      cout << v << ' ';
    cout << endl;
  }
}

void Graph::print_adj(){
  for(Node u: V()){
    cout << u << ':';
    for(Node v: adj(u))
      cout << v << ' ';
    cout << endl;
  }
}

void Graph::bfs(Node r){
  queue<Node> q;
  bool *visited = new bool[n()+1]{};
  if(has(r)){
    q.push(r);
    visited[r] = true;
  }
  while(!q.empty()){
    Node u = q.front(); q.pop();
    cout << u << ' ';
    for(Node v: adj(u)){
      if(!visited[v]){
        q.push(v);
        visited[v] = true;
      }
    }
  }
  cout << endl;
  delete[] visited;
}

void Graph::dfs(Node r){
  stack<Node> s;
  bool *visited = new bool[n()+1]{};
  if(has(r)){
    s.push(r);
    visited[r] = true;
  }
  while(!s.empty()){
    Node u = s.top(); s.pop();
    cout << u << ' ';
    for(Node v: adj(u)){
      if(!visited[v]){
        s.push(v);
        visited[v] = true;
      }
    }
  }
  cout << endl;
  delete[] visited;
}

vector<int> Graph::distance(Node r){
  queue<Node> q;
  vector<int> _distance(n()+1);
  for(Node u=1; u <=n(); u++)
    _distance[u] = -1;
  if(has(r)){
    q.push(r);
    _distance[r] = 0;
  }
  while(!q.empty()){
    Node u = q.front(); q.pop();
    for(Node v: adj(u)){
      if(_distance[v] == -1){
        _distance[v] = 1+_distance[u];
        q.push(v);
      }
    }
  }
  return _distance;  
}

bool Graph::hasCycle(){
  if(undirected()){
    bool _hasCycle = false;
    stack<Node> s;
    bool *visited = new bool[n()+1]{};
    for(Node r: V()){
      for(Node u=1; u <=n(); u++)
        visited[u] = false;
      s = stack<Node>();
      s.push(r);
      visited[r] = true;
      while(!_hasCycle && !s.empty()){
        Node u = s.top(); s.pop();
        for(Node v: adj(u)){
          if(!visited[v]){
            s.push(v);
            visited[v] = true;
          }else{
            _hasCycle = true;
            break;
          }
        }
      }
      if(_hasCycle)
        break;
    }
    delete[] visited;
    return _hasCycle;
  }else{
    bool _hasCycle = false;
    stack<Node> s;
    int *dt = new int[n()+1]{};
    int *ft = new int[n()+1]{};
    int time;
    for(Node r: V()){
      time = 0;
      for(Node u=1; u <=n(); u++)
        dt[u] = ft[u] = 0;        
      s = stack<Node>();
      s.push(r);
      while(!_hasCycle && !s.empty()){
        Node u = s.top();
        time++;
        if(dt[u] != 0){
          ft[u] = time;
          s.pop();
        }
        else{
          dt[u] = time;
          for(Node v: adj(u)){
            if(dt[v] == 0){
              s.push(v);
            }else if(ft[v] == 0 && dt[v]<dt[u]){
              _hasCycle = true;
              break;
            }
          }
        }
      }
      if(_hasCycle)
        break;
    }
    delete[] dt;
    delete[] ft;
    return _hasCycle;    
  }
}

Graph Graph::transposed(){
  Graph thisTransposed(_graphType);
  for(Node u: V()){
    for(Node v: adj(u)){
      thisTransposed.deleteEdge(u, v);
      thisTransposed.insertEdge(v, u);
    }
  }
  return thisTransposed;
}
set<Graph::Node> Graph::cc(Node r){
  queue<Node> q;
  set<Node> _cc;
  bool *visited = new bool[n()+1]{};
  if(has(r)){
    q.push(r);
    visited[r] = true;
  }
  while(!q.empty()){
    Node u = q.front(); q.pop();
    for(Node v: adj(u)){
      if(!visited[v]){
        q.push(v);
        visited[v] = true;
        _cc.insert(u);
      }
    }
  }
  delete[] visited;
  return _cc;
}
list<set<Graph::Node>*> Graph::ccs(){
  list<set<Node>*> _ccs;
  bool* visited = new bool[n()+1]{};
  for(Node r: V()){
    if(!visited[r]){
      set<Node>* mycc = new set<Node>();
      //BFS
      queue<Node> q;
      q.push(r);
      visited[r] = true;
      mycc->insert(r);
      while(!q.empty()){
        Node u = q.front(); q.pop();
        for(Node v: adj(u)){
          if(!visited[v]){
            q.push(v);
            visited[v] = true;
            mycc->insert(v);
          }
        }
      }      
      _ccs.push_front(mycc);
    }
  }
  delete[] visited;
  return _ccs;
}
void Graph::print_cc(Node r){
  cout << r << ':';
  for(Node u: cc(r))
    cout << u << ' ';
  cout << endl;
}
void Graph::print_ccs(){
  for(set<Node>* _cc: ccs()){
    cout << "CC:";
    for(Node u: *_cc)
      cout << u << ' ';
    cout << endl;
    delete _cc;    
  }
}

int main(int argc, char *argv[]){
  Graph g(Graph::UNDIRECTED);
  // Building a graph
  for(Graph::Node u=1; u <= 10; u++){
    g.insertNode(u);
  }
  g.insertEdge(1,2);
  g.insertEdge(2,3);
  g.insertEdge(1,3);
  g.insertEdge(1,6);
  g.insertEdge(5,8);

  g.print_adj();
  g.print_cc(1);
  return 0;
}

