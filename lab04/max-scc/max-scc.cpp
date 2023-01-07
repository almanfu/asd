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
  enum EdgeDirection {DIRECTED, UNDIRECTED};
  EdgeDirection edgeDirection;

  int n;
  vector<list<Node>> adj;

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

  int max_scc(Node s){
    if(undirected())
      return -1;
  
    struct sNode{
      Node node;
      list<Node>::iterator iter; 
      sNode(Node u, list<Node>::iterator i){
        node = u;
        iter = i;
      }
      ~sNode(){}
    };

    //Find sccs
    vector<bool> on_scc_stack(n, false);
    stack<Node> scc_stack;
    stack<sNode> exs;
    scc_stack.push(s);
    on_scc_stack[s] = true;
    exs.push(sNode(s, adj[s].begin()));
    vector<int> dt(n, 0);
    vector<int> ll(n, 0);
    vector<int> ft(n, 0);
    int maxSize = 0;
    int time=0;
    while(!exs.empty()){
      sNode& snode = exs.top(); 
      Node& u = snode.node;
      time++;
      if(ft[u] != 0){
        //Post-order visit
        for(Node v: adj[u]){
          if(on_scc_stack[v])
            ll[u] = min(ll[u], ll[v]);          
        }
        if(ll[u] == dt[u]){
          //Base vertex, found a scc
          list<Node> found_scc;
          Node v;
          int thisSize = 0;
          do{
            v = scc_stack.top(); scc_stack.pop();
            on_scc_stack[v] = false;
            thisSize++;
          }while(v!=u);
          maxSize = maxSize >= thisSize ? maxSize : thisSize;
        }
        exs.pop();
      }else if(ft[u] == 0){
        if(dt[u] == 0){
          //Pre-order visit
          dt[u] = time;
          ll[u] = dt[u];
        }
        auto& iter = snode.iter;
        auto iterEnd = adj[u].end();
        while(iter!=iterEnd){
          Node v = *iter;
          if(dt[v] == 0){//Tree Edge
            scc_stack.push(v);
            on_scc_stack[v] = true;
            exs.push(sNode(v, adj[v].begin()));
            break;
          }
          iter++;
        }
        if(iter == iterEnd)
          ft[u] = dt[u]==time?time+1:time;
      }
    }
    return maxSize;
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
  Graph g1(n, Graph::UNDIRECTED);

  for(int i=0; i < m; i++){
    Graph::Node u, v;
    in >> u >> v;
    g.insertEdge(u, v);
    g1.insertEdge(u, v);
  }

  int maxSize = 1;
  
  vector<int> cc = g1.ccs();

  unordered_set<int> ccs = unordered_set<int>();

  for(Graph::Node s = 0; s < n; s++){// O(n+m), poiché ogni nodo/arco è attraversato al più una volta
    if(ccs.find(cc[s]) == ccs.end() && g.adj[s].size() > 1){
      ccs.insert(cc[s]);
      int thisSize = g.max_scc(s);
      maxSize = maxSize >= thisSize ? maxSize : thisSize;
      if(maxSize > (n-s))
        break;
    }
  }
  out << maxSize << endl;

  in.close();
  out.close();
  return 0;
}