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
    int w;
    int f;
    int y;
    int n;
    int c;
    Edge(Node u, int weight, int first, int yes, int no){
      node = u;
      w = weight;
      f = first;
      y = yes;
      n = no;
      c = yes+no;
    }
    friend bool operator<(const Edge e1, const Edge e2){
      return e1.w < e2.w;
    }
  };
  enum EdgeDirection {DIRECTED, UNDIRECTED};
  enum EdgeWeight {WEIGHTED, UNWEIGHTED};
  EdgeDirection edgeDirection;
  EdgeWeight edgeWeight;
  static const int TMAX = 1000000;
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
  void insertEdge(Node u, Node v, int w=1, int f=0, int y=TMAX, int n=1){
    if(unweighted())
      w = 1;
    if(has(u) && has(v) && u != v && w >= 1){
      adj[u].push_front(Edge(v, w, f, y, n));
      if(undirected())
        adj[v].push_front(Edge(u, w, f, y, n));
    }
  }

  // isGreen
  bool isGreen(Edge e, int x){
    if(x < e.f)
      return false;
    else
      return 0 <= (x-e.f)%e.c && (x-e.f)%e.c < e.y;
  }

  // time-dependent Weight
  /// assuming y >= w for all edges and y >= 1
  int W(Edge e, int x){
    if(x <= e.f)
      return (e.f-x)+e.w;
    else if(!isGreen(e, x) || (isGreen(e, x) && e.w > (e.y-((x-e.f)%e.c)) ))
      return e.c-((x-e.f)%e.c)+e.w;
    else
      return e.w;
  }

  pair<int, list<Node>> laser(){
    Node r = 0;
    const int INF = INT32_MAX;
    typedef pair<int, Node> Li;
    priority_queue<Li, vector<Li>, greater<Li>> q;
    vector<int> time(n, INF);
    vector<Node> pred(n, -1);
    time[r] = 0;
    q.push(Li(0, r));
    //Dijkstra 
    while(!q.empty()){
      Li li = q.top(); q.pop();
      int x = li.first;
      Node u = li.second;
      for(Edge e: adj[u]){
        int altTime = W(e, x)+x;
        if(time[e.node] > altTime){
          //Rimuovo vecchio lato
          pred[e.node]=u;
          time[e.node] = altTime;
          q.push(Li(time[e.node], e.node));
        }
      }
    }
    
    //Creo cammino costo minimo da 0 a n-1
    list<Node> path = list<Node>();
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
    return pair<int, list<int>>(time[n-1], path);
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

  int museless = 0;
  for(int i=0; i < m; i++){
    Graph::Node u, v;
    int w, f, y, n;
    in >> u >> v >> w >> f >> y >> n;
    if(y < w)
      museless++;
    else// => y >= 1
      g.insertEdge(u, v, w, f, y, n);
  }
  //cout << "museful=" << (m-museless) << " museless=" << museless << " ratio=" << ((float)museless/(float)m) << endl;

  int time;
  list<Graph::Node> path;

  //test W() and isGreen
  /*            v  w  f  y  n
  Graph::Edge e(0, 1, 1, 1, 0);
  int x;
  for(x = 5; x < 10000; x++){
    if(g.W(e, x) != 1){
      cout << (g.isGreen(e, x) ? "yes" : "no")
           << " error; x=" << x << " -> " << g.W(e, x) << endl;
    }
  }*/

  auto res = g.laser();
  time = res.first;
  path = res.second;

  if(path.size()> 0 && time <= g.TMAX){
    out << time << endl;
    for(Graph::Node u: path)
      out << u << endl;
  }else{
    out << -1 << endl;
  }

  in.close();
  out.close();
  return 0;
}
