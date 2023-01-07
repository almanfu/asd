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

  int n;
  vector<list<Edge>> adj;
  vector<int> color;

  public:
  Graph(int nodes, EdgeDirection eDir, EdgeWeight eWeight){
    edgeDirection = eDir;
    edgeWeight = eWeight;
    n = nodes;
    adj = vector<list<Edge>>(n);
    color = vector<int>(n, -1);
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
    const int INF = INT32_MAX;
    priority_queue<Li, vector<Li>, greater<Li>> q;
    vector<int> distance(n, INF);
    vector<Node> pred(n, -1);
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
          pred[e.node]=u;
          distance[e.node] = altDistance;
          q.push(Li(distance[e.node], e.node));
        }
      }
    }
    //Creo grafo cammino costo minimi
    Graph G(n, Graph::DIRECTED, Graph::UNWEIGHTED);
    for(Node v=0; v < n; v++){
      if(pred[v] != -1)
        G.insertEdge(pred[v], v);
    }
    return G;
  }

  pair<int, list<Node>> laser(){
    Node r = 0;
    const int INF = INT32_MAX;
    typedef pair<int, Node> Li;
    priority_queue<Li, vector<Li>, greater<Li>> q;
    vector<int> distance(n, INF);
    vector<Node> pred(n, -1);
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
          pred[e.node]=u;
          distance[e.node] = altDistance;
          q.push(Li(distance[e.node], e.node));
        }
      }
    }
    
    //Creo cammino costo minimo da 0 a n-1
    list<Node> path = list<Node>();

    Graph G(n, Graph::DIRECTED, Graph::UNWEIGHTED);
    Node d = n-1;

    do{
      if(pred[d] == -1)
        break;
      else{
        path.push_front(d);
        d = pred[d];
      }
    }while(d != r);
    if(d == r)
      path.push_front(r);
    return pair<int, list<int>>(distance[n-1], path);
  }
};

/*

*/

int main(int argc, char *argv[]){
  ifstream in("input.txt");
  ofstream out("output.txt");

  

  int n, m;
  in >> n >> m;
  
  Graph g(n, Graph::UNDIRECTED, Graph::WEIGHTED);

  for(int i=0; i < m; i++){
    Graph::Node u, v;
    int w, blackoutTime, onDelta, offDelta;
    in >> u >> v >> w >> blackoutTime >> onDelta >> offDelta;
    g.insertEdge(u, v, w);
  }


  int cost;
  list<Graph::Node> path;

  auto res = g.laser();
  cost = res.first;
  path = res.second;

  if(path.size()> 0){
    out << cost << endl;
    for(Graph::Node u: path)
      out << u << endl;
  }else{
    out << -1;
  }

  in.close();
  out.close();
  return 0;
}
