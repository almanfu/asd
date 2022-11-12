#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <stack>

using namespace std;

const bool LOCAL = false;

/*

*/

class Graph{
  public:
  typedef int Node;
  struct Edge{
    Node node;
    int weight;
    Edge(Node n, int w){
      node = n;
      weight = w;
    }
    friend bool operator<(const Edge e1, const Edge e2){
      return e1.weight < e2.weight;
    }
  };
  enum EdgeDirection {DIRECTED, UNDIRECTED};
  enum EdgeWeight {WEIGHTED, UNWEIGHTED};
  EdgeDirection edgeDirection;
  EdgeWeight edgeWeight;
  vector<int> color;
  private:
  vector<Node> V;
  vector<list<Edge>> adj;
  int n;
  public:
  Graph(int nodes, EdgeDirection eDir, EdgeWeight eWeight){
    edgeDirection = eDir;
    edgeWeight = eWeight;
    n = nodes;
    color = vector<int>(n, -1);
    adj = vector<list<Edge>>(n);
    V = vector<Node>(n);
    for(Node u=0; u < n; u++){
      V[u] = u;
      adj[u] = list<Edge>();
    }
  }
  ~Graph(){}
  bool has(Node u){
    return 0<=u && u < n;
  }
  bool undirected(){
    return edgeDirection == UNDIRECTED;
  }
  bool unweighted(){
    return edgeWeight == UNWEIGHTED;
  }
  void insertEdge(Node u, Node v, int weight=1){
    if(unweighted())
      weight  = 1;
    if(has(u) && has(v) && u != v && weight >= 1){
      adj[u].push_front(Edge(v, weight));
      if(undirected())
        adj[v].push_front(Edge(u, weight));
    }
  }
  // Utility Functions
  void print_adj(){
    for(Node u=0; u < n; u++){
      cout << u << ':';
      if(unweighted()){
        for(Edge e: adj[u])
          cout << e.node << ' ';
      }else{
        for(Edge e: adj[u])
          cout << '(' << e.node << ',' << e.weight << ')' << ' ';
      }
      cout << endl;
    }
  }
  void export_colors(){
    ofstream plt("plt.txt");
    for(Node u=0; u < n; u++)
      plt << u << ' ' << color[u] << '\n';
    plt.close();
  }
  int batman(Node s, Node d){
    //Find scc
    int s_scc=0, d_scc=-1;
    vector<list<Node>> scc(n);
    int n_scc=0;
    {
    struct sNode{
      Node node;
      list<Edge>::iterator iter; 
      sNode(Node u, list<Edge>::iterator i){
        node = u;
        iter = i;
      }
      ~sNode(){}
    };
    vector<bool> on_scc_stack(n, false);
    stack<Node> scc_stack;
    stack<sNode*> exs;
    scc_stack.push(s);
    on_scc_stack[s] = true;
    exs.push(new sNode(s, adj[s].begin()));
    int *dt = new int[n]{};
    int *ll = new int[n]{};
    int *ft = new int[n]{};
    int time=0;
    while(!exs.empty()){
      sNode* snode = exs.top(); 
      Node& u = snode->node;
      time++;
      if(ft[u] != 0){
        //Post-order visit
        for(Edge e: adj[u]){
          if(on_scc_stack[e.node])
            ll[u] = min(ll[u], ll[e.node]);          
        }
        if(ll[u] == dt[u]){
          //Base vertex, found a scc
          list<Node> found_scc;
          Node v;
          do{
            v = scc_stack.top(); scc_stack.pop();
            on_scc_stack[v] = false;
            if(v == s)
              s_scc = n_scc;
            if(v == d)
              d_scc = n_scc;
            color[v] = n_scc;
            found_scc.push_front(v);
          }while(v!=u);
          scc[n_scc]=found_scc;
          n_scc++;
        }
        exs.pop();
        delete snode;
      }else if(ft[u] == 0){
        if(dt[u] == 0){
          //Pre-order visit
          dt[u] = time;
          ll[u] = dt[u];
        }
        auto& iter = snode->iter;
        auto iterEnd = adj[u].end();
        while(iter!=iterEnd){
          Edge e = *iter;
          if(dt[e.node] == 0){//Tree Edge
            iter++;
            scc_stack.push(e.node);
            on_scc_stack[e.node] = true;
            exs.push(new sNode(e.node, adj[e.node].begin()));
            break;
          }else if(ft[e.node] != 0 && dt[e.node] > dt[u]){
            //Forward Edge
            iter++;
          }else if(ft[e.node] == 0 && dt[e.node] < dt[u]){
            //Back Edge
            if(on_scc_stack[e.node]){}
              //ll[u] = min(dt[e.node], ll[u]);
            iter++;
          }else{
            //Cross Edge
            if(on_scc_stack[u]){}
              //ll[u] = min(dt[e.node], ll[u]);
            iter++;
          }
        }
        if(iter == iterEnd){
          ft[u] = dt[u]==time?time+1:time;
        }
      }
    }
    delete[] dt;
    delete[] ll;
    delete[] ft;
    }
    //Build scc graph
    Graph G(n_scc, Graph::DIRECTED, Graph::WEIGHTED);
    for(int my_scc=0; my_scc < n_scc; my_scc++){
      if(my_scc == d_scc)
        continue;
      vector<int> weight(n, 0);
      for(Node u: scc[my_scc]){
        for(Edge e: adj[u]){
          if(color[e.node] != my_scc)
            weight[color[e.node]]++;
        }
      }
      for(int other_scc=0; other_scc< n_scc; other_scc++){
        if(weight[other_scc]!=0)
          G.insertEdge(my_scc, other_scc, weight[other_scc]);
      }
    }
    if(LOCAL){
      export_colors();
      for(int i=0; i < n_scc; i++){
        cout << '{';
        for(Node u: scc[i])
          cout << u << ' ';
        cout << '}' << endl;
      }
      //cout << s_scc << ' ' << d_scc << endl;
      //G.print_adj();
      cout << "=============" << endl;
    }
    if(d_scc==-1)
      return 0;
    vector<int> numPaths(n, 0);
    {
    //Find paths O(n+2*m)
    struct sNode{
      Node node;
      list<Graph::Edge>::iterator iter; 
      sNode(Node u, list<Graph::Edge>::iterator i){
        node = u;
        iter = i;
      }
    };
    stack<sNode*> exs;
    exs.push(new sNode(s_scc, G.adj[s_scc].begin()));
    int *dt = new int[n_scc]{};
    int *ft = new int[n_scc]{};
    int time=0;
    while(!exs.empty()){
      sNode* snode = exs.top(); 
      Node& u = snode->node;
      time++;
      if(ft[u] != 0){
        //Post-order visit
        for(Edge e: G.adj[u])
          numPaths[u] += numPaths[e.node]*e.weight;
        delete snode;
        exs.pop();
      }else if(ft[u] == 0){
        if(dt[u] == 0){
          //Pre-order visit
          dt[u] = time;
          numPaths[u] = u!=d_scc?0:1;
        }
        auto& iter = snode->iter;
        auto iterEnd = G.adj[u].end();
        while(iter!=iterEnd){
          Edge e = *iter;
          if(dt[e.node] == 0){//Tree Edge
            iter++;
            exs.push(new sNode(e.node, G.adj[e.node].begin()));
            break;
          }else if(ft[e.node] != 0 && dt[e.node] > dt[u]){
            iter++;
            //Forward Edge
          }else if(ft[e.node] == 0 && dt[e.node] < dt[u]){
            iter++;
            //Back Edge
          }else{
            iter++;
            //Cross Edge
          }
        }
        if(iter == iterEnd){
          ft[u] = dt[u]==time?time+1:time;
        }
      }
    }
    delete[] dt;
    delete[] ft;
    }
    return numPaths[s_scc];
  }
};

/*

*/

int main(int argc, char *argv[]){
  ifstream in("input.txt");
  ofstream out("output.txt");

  Graph::Node s, d;
  int n, m;
  in >> n >> m >> s >> d;
  
  Graph g(n, Graph::DIRECTED, Graph::UNWEIGHTED);

  for(int i=0; i < m; i++){
    Graph::Node u, v;
    in >> u >> v;
    g.insertEdge(u, v);
  }

  out << g.batman(s, d);

  in.close();
  out.close();
  return 0;
}
