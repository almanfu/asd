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
  enum EdgeDirection {DIRECTED, UNDIRECTED};
  EdgeDirection edgeDirection;

  private:
    int n;
  vector<Node> V;
  vector<list<Node>> adj;

  public:
  Graph(int nodes, EdgeDirection eDir){
    edgeDirection = eDir;
    n = nodes;
    V = vector<Node>(n);
    adj = vector<list<Node>>(n);
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
  void insertEdge(Node u, Node v){
    if(has(u) && has(v) && u != v){
      adj[u].push_front(v);
      if(undirected())
        adj[v].push_front(u);
    }
  }
  // Functions
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
    vector<bool> visited(n, false);
    if(has(r)){
      q.push(r);
      visited[r] = true;
    }
    while(!q.empty()){
      Node u = q.front(); q.pop();
      //Visit
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
    vector<bool> visited(n, false);
    if(has(r)){
      s.push(r);
      visited[r] = true;
    }
    while(!s.empty()){
      Node u = s.top(); s.pop();
      //Pre-order visit
      for(Node v: adj[u]){
        if(!visited[v]){
          s.push(v);
          visited[v] = true;
        }
      }
    }
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
      vector<bool> visited(n);
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
      return _hasCycle;
    }else{
      bool _hasCycle = false;
      stack<Node> s;
      vector<int> dt(n,0);
      vector<int> ft(n,0);
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
      return _hasCycle;    
    }
  }

  Graph transposed(){
    Graph _transposed(n, edgeDirection);
    for(Node u: V){
      for(Node v: adj[u])
        _transposed.insertEdge(v, u);
    }
    return _transposed;
  }
  list<Node> cc(Node r){
    queue<Node> q;
    list<Node> _cc;
    vector<bool> visited(n, false);
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
    return _cc;
  }
  list<list<Node>> ccs(){
    list<list<Node>> _ccs;
    vector<bool> visited(n, false);
    for(Node r: V){
      if(!visited[r]){
        list<Node> cc;
        //BFS
        queue<Node> q;
        q.push(r);
        visited[r] = true;
        cc.push_front(r);
        while(!q.empty()){
          Node u = q.front(); q.pop();
          for(Node v: adj[u]){
            if(!visited[v]){
              q.push(v);
              visited[v] = true;
              cc.push_front(v);
            }
          }
        }      
        _ccs.push_front(cc);
      }
    }
    return _ccs;
  }
};