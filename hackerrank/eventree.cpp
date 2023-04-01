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

  int n;
  vector<list<Node>> adj;

  public:
  Graph(int nodes, EdgeDirection eDir){
    edgeDirection = eDir;
    n = nodes;
    adj = vector<list<Node>>(n);
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
    vector<bool> visited = vector<bool>(n, false);
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
    vector<bool> visited = vector<bool>(n, false);
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
};

pair<bool, int> fun(Graph G, int r, int p=-1){
  int allDetached = 0;
  int childrenDetached = 0;
  for(int v: G.adj[r]){
    if(v != p){
      auto res = fun(G, v, r);
      allDetached += res.second;
      childrenDetached += res.first ? 1 : 0;
    }
  }
  if((G.adj[r].size()-(p != -1 ? 1 : 0)-childrenDetached+1) % 2 == 0){
    return pair<bool, int>(true, allDetached+childrenDetached);
  }else{
    return pair<bool, int>(false, allDetached+childrenDetached);
  }
}

// Complete the evenForest function below.
int evenForest(int t_nodes, int t_edges, vector<int> t_from, vector<int> t_to) {
  Graph G = Graph(t_nodes, Graph::UNDIRECTED);
  for(int i=0; i < t_edges; i++){
    G.insertEdge(t_from[i]-1, t_to[i]-1);
  }
  auto res = fun(G, 0);
  return res.second;
}

int main(int argc, char *argv[]){
  ifstream in("input.txt");
  ofstream out("output.txt");


  in.close();
  out.close();
  return 0;
}
