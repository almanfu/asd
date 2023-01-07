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

  private:
    int n;
  vector<Node> V;
  vector<list<Edge>> adj;
  vector<int> color;

  public:
  Graph(int nodes, EdgeDirection eDir, EdgeWeight eWeight){
    edgeDirection = eDir;
    edgeWeight = eWeight;
    n = nodes;
    V = vector<Node>(n);
    adj = vector<list<Edge>>(n);
    color = vector<int>(n, -1);    
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
  // Functions
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
      for(Edge e: adj[u]){
        if(!visited[e.node]){
          q.push(e.node);
          visited[e.node] = true;
        }
      }
    }
  }

  void dfs(Node r){
    struct sNode{
      Node node;
      list<Graph::Edge>::iterator iter; 
      sNode(Node u, list<Graph::Edge>::iterator i){
        node = u;
        iter = i;
      }
      ~sNode(){}
    };
    stack<sNode> s;
    s.push(sNode(r, adj[r].begin()));
    vector<int> dt(n, 0);
    vector<int> ft(n, 0);
    int time=0;
    while(!s.empty()){
      sNode& snode = s.top(); 
      Node& u = snode.node;
      time++;
      if(ft[u] != 0){
        //Post-order visit
        s.pop();
      }else if(ft[u] == 0){
        if(dt[u] == 0){
          //Pre-order visit
          dt[u] = time;
        }
        auto& iter = snode.iter;
        auto iterEnd = adj[u].end();
        while(iter!=iterEnd){
          Edge e = *iter;
          if(dt[e.node] == 0){//Tree Edge
            iter++;
            s.push(sNode(e.node, adj[e.node].begin()));
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
  }

  Graph dijkstra(Node r){
    typedef pair<int, Node> Li;
    priority_queue<Li, vector<Li>, greater<Li>> q;
    vector<int> distance(n);
    vector<list<Node>> preds(n);
    const int INF = INT32_MAX;
    for(Node u: V)
      distance[u] = INF;
    distance[r] = 0;
    q.push(Li(0, r));
    //Dijkstra 
    while(!q.empty()){
      Li li = q.top(); q.pop();
      Node u = li.second;
      for(Edge e: adj[u]){
        int altDistance = e.weight+distance[u];
        if(distance[e.node] > altDistance){
          //Rimuovo vecchi lati
          preds[e.node].clear();
          preds[e.node].push_front(u);
          distance[e.node] = altDistance;
          q.push(Li(distance[e.node], e.node));
        }else if (distance[e.node] == altDistance){
          //Aggiungo nuovo lato
          preds[e.node].push_front(u);
        }
      }
    }
    //Creo grafo cammini costo minimi
    Graph G(n, Graph::DIRECTED, Graph::UNWEIGHTED);
    for(Node v: G.V){
      for(Node u: preds[v])
        G.insertEdge(u, v);
    }
    return G;
  }

  void sccs(Node s){
    struct sNode{
      Node node;
      list<Edge>::iterator iter; 
      sNode(Node u, list<Edge>::iterator i){
        node = u;
        iter = i;
      }
      ~sNode(){}
    };
    //Find sccs and color 
    int s_scc=0;
    vector<list<Node>> _sccs(n);
    int n_sccs=0;
    vector<bool> on_scc_stack(n, false);
    stack<Node> scc_stack;
    stack<sNode> exs;
    scc_stack.push(s);
    on_scc_stack[s] = true;
    exs.push(sNode(s, adj[s].begin()));
    vector<int> dt(n, 0);
    vector<int> ll(n, 0);
    vector<int> ft(n, 0);
    int time=0;
    while(!exs.empty()){
      sNode& snode = exs.top(); 
      Node& u = snode.node;
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
              s_scc = n_sccs;
            color[v] = n_sccs;
            found_scc.push_front(v);
          }while(v!=u);
          _sccs[n_sccs]=found_scc;
          n_sccs++;
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
          Edge e = *iter;
          if(dt[e.node] == 0){//Tree Edge
            iter++;
            scc_stack.push(e.node);
            on_scc_stack[e.node] = true;
            exs.push(sNode(e.node, adj[e.node].begin()));
            break;
          }else if(ft[e.node] != 0 && dt[e.node] > dt[u]){
            //Forward Edge
            iter++;
          }else if(ft[e.node] == 0 && dt[e.node] < dt[u]){
            //Back Edge
            iter++;
          }else{
            //Cross Edge
            iter++;
          }
        }
        if(iter == iterEnd){
          ft[u] = dt[u]==time?time+1:time;
        }
      }
    }
    //Build sccs graph
    Graph G(n_sccs, Graph::DIRECTED, Graph::WEIGHTED);
    for(int scc=0; scc < n_sccs; scc++){
      vector<int> weight(n, 0);
      //Calculate weights
      for(Node u: _sccs[scc]){
        for(Edge e: adj[u]){
          if(color[e.node] != scc)
            weight[color[e.node]]++;
        }
      }
      for(int other_scc=0; other_scc<scc; other_scc++){
        if(weight[other_scc]!=0)
          G.insertEdge(scc, other_scc, weight[other_scc]);
      }
    }
  }
};