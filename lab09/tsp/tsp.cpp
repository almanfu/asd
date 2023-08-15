#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <stack>
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
                                                             minEdge(n, INF){}

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
      adj[u].push_front(Edge(v, weight));
      if(weight < minEdge[u]){
        transfer[u] = minEdge[u] + weight;
        minEdge[u] = weight;
      }
      if (undirected())
      {
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
  pair<stack<Node>, int> hamiltonrec(Node r, Node u, int i, int cost, stack<Node> &path, vector<bool> &in_path)
  {
    /*int lb = minEdge[u] + minEdge[r];
    for (Graph::Node h = 0; h < n; h++){
      if(!inPath[h])
        lb += transfer[h] / 2.0;
    }*/
    if(i==n){
      return {path, cost};
    }
    for (Graph::Edge &e : adj[u])
    {
      Graph::Node v = e.node;
      if (!in_path[v] || (i == n - 1 && v == r))
      {
        path.push(v);
        in_path[v] = true;
        auto res = hamiltonrec(r, v, i + 1, cost + e.weight, path, in_path);
        if(res.second != INF)
          return res;
        path.pop();
        in_path[v] = false;
      }
    }
    return {path, INF};
  }
  pair<stack<Node>, int> hamilton(Node r)
  {
    stack<Node> path;
    path.push(r);
    vector<bool> in_path(n, false);
    in_path[r] = true;
    return hamiltonrec(r, r, 0, 0, path, in_path);
  }
};

/*

*/

int main()
{
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

  pair<stack<Graph::Node>, int> res = g.hamilton(0);
  while(!res.first.empty()){
    Graph::Node u = res.first.top();
    res.first.pop();
    out << u << ' ';
  }
  out << '#';

  in.close();
  out.close();
  return 0;
}
