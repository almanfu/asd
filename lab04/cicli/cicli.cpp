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
  vector<Node> cricca;
  vector<Node> parentd;
  int n;
  GraphType _graphType;
  bool _hasCycle;

  Graph(int nodes, GraphType gType, Node r){
    n = nodes;
    _graphType = gType;
    root = r;

    adj = vector<list<Node>>(n);
    parent = vector<Node>(n, -1);
    level = vector<Node>(n, -1);
    cricca = vector<Node>(n, -1);
    parentd = vector<Node>(n, -1);
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
  void print_adj(){
    for(Node u=0; u < n; u++){
      cout << u << ':';
      for(Node v: adj[u])
        cout << v << ' ';
      cout << endl;
    }
  }

  void bfs(){
    //setup level[], parent[]
    Node r = root;
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
        if(visited[v] && v != parent[u] && cricca[v] == -1){
          _hasCycle = true;
          if(cricca[parent[u]] != parent[u] && cricca[parent[u]] != -1){}
            //cout << "conflict " << parent[u] << " " << cricca[parent[u]] << endl;
          cricca[parent[u]] = parent[u];
          cricca[u] = parent[u];
          cricca[v] = parent[u];
        }

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

  void printCricche(){
    cout << "cricche[]:" << endl << endl;
    for(Node u=0; u < n; u++)
      cout << "cricca[" << u << "]=" << cricca[u] << endl;
    cout << "======" << endl;
  }

  void printTree(){
    cout << "level[]:" << endl << endl;
    for(Node u=0; u < n; u++){
      cout << "level[" << u <<"]=" << level[u] << endl;
    }
    cout << "======" << endl;

    cout << "parent[]:" << endl << endl;
    for(Node u=0; u < n; u++){
      cout << "parent[" << u <<"]=" << parent[u] << endl;
    }
    cout << "======" << endl;
  }

  int cicli(Node s, Node d){
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

    while(sanc != danc && !(cricca[sanc] == cricca[danc] && cricca[sanc] != -1)){
      sanc = parent[sanc];
      danc = parent[danc];
    }
    if(s == 79 && d == 527){
      //cout << sanc << ' ' << danc << endl;
    }
    if(sanc == danc)
      return level[s]+level[d]-2*level[sanc];
    else
      return level[s]+level[d]-2*level[sanc]+1;
  }
};

/*

*/

int main(int argc, char *argv[]){
  ifstream in("input.txt");
  ofstream out("output.txt");

  

  int n, m, q;
  in >> n >> m >> q;
  
  Graph g(n, Graph::UNDIRECTED, (n-1)/2);

  for(int i=0; i < m; i++){
    Graph::Node u, v;
    in >> u >> v;
    g.insertEdge(u, v);
  }

  // setup
  g.bfs();
  //g.dfs();

  //g.printCricche();
  //g.printTree();
  //cout << (g.hasCycle() ? "si ciclo" : "no ciclo") << endl;


  for(int i=0; i<q; i++){
    Graph::Node r, d;
    in >> r >> d;
    out << g.cicli(r, d) << endl;
  }

  in.close();
  out.close();
  return 0;
}