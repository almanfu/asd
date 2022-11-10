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
  private:
  vector<Node> V;
  vector<list<Node>> adj;
  int n;
  GraphType _graphType;
  public:
  Graph(int nodes, GraphType gType){
    _graphType = gType;
    n = nodes;
    adj = vector<list<Node>>(n);
    V = vector<Node>(n);
    for(Node u=0; u < n; u++)
      V[u] = u;
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
    bool *visited = new bool[n]{};
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
      for(Node v: adj[u]){
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
      for(Node v: adj[u]){
        if(_distance[v] == -1){
          _distance[v] = 1+_distance[u];
          q.push(v);
        }
      }
    }
    return _distance;  
  }

  bool hasCycle(){
    if(undirected()){
      bool _hasCycle = false;
      stack<Node> s;
      bool *visited = new bool[n]{};
      for(Node r: V){
        for(Node u: V)
          visited[u] = false;
        s = stack<Node>();
        s.push(r);
        visited[r] = true;
        while(!_hasCycle && !s.empty()){
          Node u = s.top(); s.pop();
          for(Node v: adj[u]){
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
      int *dt = new int[n]{};
      int *ft = new int[n]{};
      int time;
      for(Node r: V){
        time = 0;
        for(Node u: V)
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
            for(Node v: adj[u]){
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

  Graph transposed(){
    Graph thisTransposed(n, _graphType);
    for(Node u: V){
      for(Node v: adj[u])
        thisTransposed.insertEdge(v, u);
    }
    return thisTransposed;
  }
  list<Node> cc(Node r){
    queue<Node> q;
    list<Node> _cc;
    bool *visited = new bool[n]{};
    if(has(r)){
      q.push(r);
      visited[r] = true;
      _cc.push_front(r);
    }
    while(!q.empty()){
      Node u = q.front(); q.pop();
      for(Node v: adj[u]){
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
  list<list<Node>*> ccs(){
    list<list<Node>*> _ccs;
    bool* visited = new bool[n]{};
    for(Node r: V){
      if(!visited[r]){
        list<Node>* mycc = new list<Node>();
        //BFS
        queue<Node> q;
        q.push(r);
        visited[r] = true;
        mycc->push_front(r);
        while(!q.empty()){
          Node u = q.front(); q.pop();
          for(Node v: adj[u]){
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
  void print_cc(Node r){
    cout << r << ':';
    for(Node u: cc(r))
      cout << u << ' ';
    cout << endl;
  }
  void print_ccs(){
    for(list<Node>* _cc: ccs()){
      cout << "CC:";
      for(Node u: *_cc)
        cout << u << ' ';
      cout << endl;
      delete _cc;    
    }
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

  in.close();
  out.close();
  return 0;
}
