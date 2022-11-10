#include <iostream>

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
  private:
  vector<list<Node>*> _adj;
  vector<Node> _V;
  int _n;
  GraphType _graphType;
  public:
  Graph(int nodes, GraphType gType){
    _graphType = gType;
    _n = nodes;
    _adj = vector<list<Node>*>(_n);
    _V = vector<Node>(_n);
    for(Node v=0; v < _n; v++){
      _V[v] = v;
      _adj[v] = new list<Node>;
    }
  }
  ~Graph(){
    for(Node u=0; u < _n; u++)
        delete _adj[u];
  }
  bool has(Node u){
    return 0<=u && u < _n;
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
  const vector<Node> V(){
    return _V;
  }
  void insertEdge(Node u, Node v){
    if(has(u) && has(v) && u != v){
      _adj[u]->push_front(v);
      if(undirected())
        _adj[v]->push_front(u);
    }
  }
  const list<Node> adj(Node u){
    if(has(u))
      return *_adj[u];
    else
      return list<Node>();
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
  list<list<Node>*> ccs();
  list<Node> cc(Node r);
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
  bool *visited = new bool[n()]{};
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
  bool *visited = new bool[n()]{};
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
  vector<int> _distance(n());
  for(Node u=0; u <n(); u++)
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
    bool *visited = new bool[n()]{};
    for(Node r: V()){
      for(Node u=0; u <n(); u++)
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
    int *dt = new int[n()]{};
    int *ft = new int[n()]{};
    int time;
    for(Node r: V()){
      time = 0;
      for(Node u=0; u <n(); u++)
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
  Graph thisTransposed(_n, _graphType);
  for(Node u: V()){
    for(Node v: adj(u)){
      thisTransposed.insertEdge(v, u);
    }
  }
  return thisTransposed;
}
list<Graph::Node> Graph::cc(Node r){
  queue<Node> q;
  list<Node> _cc;
  bool *visited = new bool[n()]{};
  if(has(r)){
    q.push(r);
    visited[r] = true;
    _cc.push_front(r);
  }
  while(!q.empty()){
    Node u = q.front(); q.pop();
    for(Node v: adj(u)){
      if(!visited[v]){
        q.push(v);
        visited[v] = true;
        _cc.push_front(v);
      }
    }
  }
  delete[] visited;
  return _cc;
}
list<list<Graph::Node>*> Graph::ccs(){
  list<list<Node>*> _ccs;
  bool* visited = new bool[n()]{};
  for(Node r: V()){
    if(!visited[r]){
      list<Node>* mycc = new list<Node>();
      //BFS
      queue<Node> q;
      q.push(r);
      visited[r] = true;
      mycc->push_front(r);
      while(!q.empty()){
        Node u = q.front(); q.pop();
        for(Node v: adj(u)){
          if(!visited[v]){
            q.push(v);
            visited[v] = true;
            mycc->push_front(v);
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
  for(list<Node>* _cc: ccs()){
    cout << "CC:";
    for(Node u: *_cc)
      cout << u << ' ';
    cout << endl;
    delete _cc;    
  }
}

int main(int argc, char *argv[]){
  Graph g(10, Graph::DIRECTED);
  // Building a graph
  g.insertEdge(0,1);
  g.insertEdge(1,2);
  g.insertEdge(2,3);
  g.insertEdge(3,4);
  g.insertEdge(4,0);

  g.print_adj();
  g.print_ccs();
  return 0;
}

