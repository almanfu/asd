#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <cmath>
#include "tsp.h"
using namespace std;

bool INFO = false;
const int INF = INT32_MAX;

/*

*/

class Graph
{

public:
  typedef int Node;
  struct Edge
  {
    Node node;
    int weight;
    Edge(Node n, int w)
    {
      node = n;
      weight = w;
    }
    friend bool operator<(const Edge e1, const Edge e2)
    {
      return e1.weight < e2.weight;
    }
  };
  enum EdgeDirection
  {
    DIRECTED,
    UNDIRECTED
  };
  enum EdgeWeight
  {
    WEIGHTED,
    UNWEIGHTED
  };
  EdgeDirection edgeDirection;
  EdgeWeight edgeWeight;

  int n;
  vector<list<Edge>> adj;
  vector<vector<int>> W;
  vector<Node> color;
  vector<int> transfer;
  vector<int> minEdge;

public:
  Graph(int nodes, EdgeDirection eDir, EdgeWeight eWeight) : n(nodes),
                                                             edgeDirection(eDir),
                                                             edgeWeight(eWeight),
                                                             adj(n),
                                                             color(n),
                                                             transfer(n, INF),
                                                             minEdge(n, INF),
                                                             W(n, vector<int>(n,INF)){}

  ~Graph() {}

  bool has(Node u)
  {
    return 0 <= u && u < n;
  }

  bool undirected()
  {
    return edgeDirection == UNDIRECTED;
  }

  bool unweighted()
  {
    return edgeWeight == UNWEIGHTED;
  }

  void insertEdge(Node u, Node v, int weight = 1)
  {
    if (unweighted())
      weight = 1;
    if (has(u) && has(v) && u != v)
    {
      W[u][v] = weight;
      adj[u].push_front(Edge(v, weight));
      if(weight < minEdge[u]){
        transfer[u] = minEdge[u] + weight;
        minEdge[u] = weight;
      }
      if (undirected())
      {
        W[v][u] = weight;
        adj[v].push_front(Edge(u, weight));
        if (weight < minEdge[v]){
          transfer[v] = minEdge[v] + weight;
          minEdge[v] = weight;
        }
      }
    
    }
  }

  // Functions
  void print_adj()
  {
    for (Node u = 0; u < n; u++)
    {
      cout << u << ':';
      if (unweighted())
      {
        for (Edge &e : adj[u])
          cout << e.node << ' ';
      }
      else
      {
        for (Edge &e : adj[u])
          cout << '(' << e.node << ',' << e.weight << ')' << ' ';
      }
      cout << endl;
    }
  }

  void bfs(Node r)
  {
    queue<Node> q;
    vector<bool> visited(n, false);
    if (has(r))
    {
      q.push(r);
      visited[r] = true;
    }
    while (!q.empty())
    {
      Node u = q.front();
      q.pop();
      // Visit
      for (Edge &e : adj[u])
      {
        if (!visited[e.node])
        {
          q.push(e.node);
          visited[e.node] = true;
        }
      }
    }
  }

  void dfs(Node r)
  {
    struct sNode
    {
      Node node;
      list<Graph::Edge>::iterator iter;
      sNode(Node u, list<Graph::Edge>::iterator i)
      {
        node = u;
        iter = i;
      }
      ~sNode() {}
    };
    stack<sNode> s;
    s.push(sNode(r, adj[r].begin()));
    vector<int> dt(n, 0);
    vector<int> ft(n, 0);
    int time = 0;
    while (!s.empty())
    {
      sNode &snode = s.top();
      Node &u = snode.node;
      time++;
      if (ft[u] != 0)
      {
        // Post-order visit
        s.pop();
        // Give return value
      }
      else if (ft[u] == 0)
      {
        if (dt[u] == 0)
        {
          // Pre-order visit
          dt[u] = time;
        }
        auto &iter = snode.iter;
        auto iterEnd = adj[u].end();
        while (iter != iterEnd)
        {
          Edge e = *iter;
          // Expect return value from previous call
          if (dt[e.node] == 0)
          {
            iter++;
            // Tree Edge
            s.push(sNode(e.node, adj[e.node].begin()));
            break;
          }
          else if (ft[e.node] != 0 && dt[e.node] > dt[u])
          {
            iter++;
            // Forward Edge
          }
          else if (ft[e.node] == 0 && dt[e.node] < dt[u])
          {
            iter++;
            // Back Edge
          }
          else
          {
            iter++;
            // Cross Edge
          }
        }
        if (iter == iterEnd)
        {
          ft[u] = dt[u] == time ? time + 1 : time;
        }
      }
    }
  }

  Graph dijkstra(Node r)
  {
    const int INF = INT32_MAX;
    typedef pair<int, Node> Li;
    priority_queue<Li, vector<Li>, greater<Li>> q;
    vector<int> distance(n, INF);
    vector<list<Node>> preds(n);
    distance[r] = 0;
    q.push(Li(0, r));
    // Dijkstra
    while (!q.empty())
    {
      Li li = q.top();
      q.pop();
      Node u = li.second;
      for (Edge &e : adj[u])
      {
        int altDistance = e.weight + distance[u];
        if (distance[e.node] > altDistance)
        {
          // Rimuovo vecchi lati
          preds[e.node].clear();
          preds[e.node].push_front(u);
          distance[e.node] = altDistance;
          q.push(Li(distance[e.node], e.node));
        }
        else if (distance[e.node] == altDistance)
        {
          // Aggiungo nuovo lato
          preds[e.node].push_front(u);
        }
      }
    }
    // Creo grafo cammini costo minimi
    Graph G(n, Graph::DIRECTED, Graph::UNWEIGHTED);
    for (Node v = 0; v < n; v++)
    {
      for (Node u : preds[v])
        G.insertEdge(u, v);
    }
    return G;
  }
  /*int lb = minEdge[u] + minEdge[r];
for (Graph::Node h = 0; h < n; h++){
  if(!inPath[h])
    lb += transfer[h] / 2.0;
}*/
  pair<vector<Node>, int> init(Node r, float t)
  {//complete graph => GREEDY choice always yields a candidate solution
    
    int i=0;
    int cost=0;
    vector<Node> path;
    vector<bool> in_path(n, false);
    path.push_back(r);
    in_path[r] = true;

    Node u = r;
    while(i != n-1){
      Graph::Node v; // chosen node
      int minChoice = INF;
      for (Graph::Edge &e : adj[u])
      {
        if (!in_path[e.node])
        {
          int delta = e.weight - minChoice;
          if (e.weight < minChoice || (float)rand() / (float)RAND_MAX < exp(-delta/t))
          {
            minChoice = e.weight;
            v = e.node;
          }
        }
      }
      path.push_back(v);
      in_path[v] = true;
      u = v;
      cost += minChoice;
      i++;
    }
    cost += W[u][r];
    return {path, cost};
  }
};

/*

*/

int main()
{
  srand(time(NULL));
  ifstream in("input.txt");
  ofstream out("output.txt");

  int n;
  in >> n;

  Graph g(n, Graph::UNDIRECTED, Graph::WEIGHTED);

  for (Graph::Node u = 1; u < g.n; u++)
  {
    for (Graph::Node v = 0; v < u; v++){
      int w;
      in >> w;
      g.insertEdge(u, v, w);
    }
  }
  int minCost = INF;

  // multi-level SA
  // SA increasing geometric T
  float T = 100;
  const float beta = 1.1;

  // iterated local search (multi-start)
  for (Graph::Node r = 0; r < n; (++r)%=n){

    // starting point (greedy+randomized with T)
    auto res = g.init(r, T); // O(n)
    vector<Graph::Node> path = res.first;
    int cost = res.second;

    // local search with 2-opt
    // SA decreasing geometric t
    float t = 2000;
    const int ITER = 10000;
    const float alpha = 0.99;

    for (int _ = 0; _ < ITER; _++)
    {
      int minDelta = INF;
      int mia, mib, mic, mid;

      // 2-opt neighbors, O(n^2)
      for (int x = 0; x < n; x++)
      {
        for (int y = x + 2; y <= n - 2; y++)
        { // can avoid all y<=x+n-2
          // 2-opt with (x,x+1) <-> (y,y+1)
          // (a,b),(c,d) to (a,c),(b,d)
          int ia = x, ib = (x + 1) % n, ic = y % n, id = (y + 1) % n;
          Graph::Node a = path[ia];
          Graph::Node b = path[ib];
          Graph::Node c = path[ic];
          Graph::Node d = path[id];
          int delta = - g.W[a][b] - g.W[c][d] + g.W[a][c] + g.W[b][d];
          if (delta < minDelta)
          {
            minDelta = delta;
            mia = ia;
            mib = ib;
            mic = ic;
            mid = id;
          }
        }
      }
      // go to neighbour with SA
      // O(n)
      if(minDelta != INF && (minDelta < 0 || (float)rand()/(float)RAND_MAX < exp(-minDelta/t))){
        cost += minDelta;
        vector<int> newPath;
        for (int i = 0; i <= mia; i++)
          newPath.push_back(path[i]);
        for (int i = mic; i >= mib; i--)
          newPath.push_back(path[i]);
        for (int i = mid; i < n; i++)
          newPath.push_back(path[i]);
        path = newPath;
      }
      t *= alpha;
    }
    if(cost < minCost){
      minCost = cost;
      // output
      for (Graph::Node u : path)
          out << u << ' ';
      out << path[0] << '#' << endl;
    }
    T *= beta;
  }

  in.close();
  out.close();
  return 0;
}
