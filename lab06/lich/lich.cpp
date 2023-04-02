#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <stack>

using namespace std;

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

private:
  int n;
  vector<Node> V;
  vector<list<Edge>> adj;
  vector<Node> succ;
  vector<int> maxLeaf;
  vector<int> color;

public:
  Graph(int nodes, EdgeDirection eDir, EdgeWeight eWeight)
  {
    edgeDirection = eDir;
    edgeWeight = eWeight;
    n = nodes;
    V = vector<Node>(n);
    adj = vector<list<Edge>>(n);

    succ = vector<Node>(n);
    maxLeaf = vector<int>(n);
    
    color = vector<int>(n, -1);
    for (Node u = 0; u < n; u++)
      V[u] = u;
  }
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
    if (has(u) && has(v) && u != v && weight >= 1)
    {
      adj[u].push_front(Edge(v, weight));
      if (undirected())
        adj[v].push_front(Edge(u, weight));
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
        for (Edge e : adj[u])
          cout << e.node << ' ';
      }
      else
      {
        for (Edge e : adj[u])
          cout << '(' << e.node << ',' << e.weight << ')' << ' ';
      }
      cout << endl;
    }
  }

  void print_maxLeaf(){
    cout << "maxLeaf" << endl;
    for (Node u = 0; u < n; u++)
      cout << u << ':' << maxLeaf[u] << endl;
  }
  void print_succ()
  {
    cout << "succ" << endl;
    for (Node u = 0; u < n; u++)
      cout << u << ':' << succ[u] << endl;
  }

  void bfs(Node r){
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
      for (Edge e : adj[u])
      {
        if (!visited[e.node])
        {
          q.push(e.node);
          visited[e.node] = true;
        }
      }
    }
  }

  void fun1()
  {
    Node r=0;
    while (adj[r].size() == 1)
      r++;
    if (adj[r].size() == 1)
      return;
    struct sNode
    {
      Node node;
      Node parent;
      list<Edge>::iterator iter;
      sNode(Node u, Node p, list<Edge>::iterator i)
      {
        node = u;
        parent = p;
        iter = i;
      }
      ~sNode() {}
      };
    stack<sNode> s;
    s.push(sNode(r, -1, adj[r].begin()));
    vector<int> dt(n, 0);
    vector<int> ft(n, 0);
    int time = 0;
    while (!s.empty())
    {
      sNode &snode = s.top();
      Node &u = snode.node;
      Node &p = snode.parent;
      time++;
      if (ft[u] != 0)
      { // Post-order visit
        for(Edge e: adj[u]){
          Node v = e.node;
          if(v != p){
            if (maxLeaf[v] + e.weight > maxLeaf[u])
            {
              maxLeaf[u] = maxLeaf[v] + e.weight;
              succ[u] = v;
            }
          }
        }
        if(succ[u] == -1){// I am a Leaf!
          succ[u] = u;
          maxLeaf[u] = 0;
        }

        s.pop();
        // Here I give the return value
      }
      else if (ft[u] == 0)
      {
        if (dt[u] == 0)
        {
          dt[u] = time;
          // Pre-order visit
          succ[u] = -1;
          maxLeaf[u] = -1;
        }
        auto &iter = snode.iter;
        auto iterEnd = adj[u].end();
        while (iter != iterEnd)
        {
          Node v = iter->node;
          if (dt[v] == 0)
          { // Tree Edge
            iter++;
            s.push(sNode(v, u, adj[v].begin())); // here I expect a return value
            break;
          }
          else if (ft[v] != 0 && dt[v] > dt[u])
          {
            iter++;
            // Forward Edge
          }
          else if (ft[v] == 0 && dt[v] < dt[u])
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

  void fun2()
  {
    Node r = 0;
    while (adj[r].size() == 1)
      r++;
    if (adj[r].size() == 1)
      return;
    struct sNode
    {
      Node node;
      Node parent;
      int pweight;
      list<Edge>::iterator iter;
      sNode(Node u, Node p, int pw, list<Edge>::iterator i)
      {
        node = u;
        parent = p;
        pweight = pw;
        iter = i;
      }
      ~sNode() {}
    };
    stack<sNode> s;
    s.push(sNode(r, -1, 0, adj[r].begin()));
    vector<int> dt(n, 0);
    vector<int> ft(n, 0);
    int time = 0;
    while (!s.empty())
    {
      sNode &snode = s.top();
      Node &u = snode.node;
      Node &p = snode.parent;
      Node &pw = snode.pweight;
      time++;
      if (ft[u] != 0)
      { // Post-order visit
        s.pop();
        // Here I give the return value
      }
      else if (ft[u] == 0)
      {
        if (dt[u] == 0)
        {
          dt[u] = time;
          // Pre-order visit
          if(p != -1 && succ[p] != u && (pw+maxLeaf[p] < maxLeaf[u])){
            succ[u] = p;
            maxLeaf[u] = pw + maxLeaf[p];
          }
        }
        auto &iter = snode.iter;
        auto iterEnd = adj[u].end();
        while (iter != iterEnd)
        {
          Node v = iter->node;
          if (dt[v] == 0)
          { // Tree Edge
            iter++;
            s.push(sNode(v, u, pw, adj[v].begin())); // here I expect a return value
            break;
          }
          else if (ft[v] != 0 && dt[v] > dt[u])
          {
            iter++;
            // Forward Edge
          }
          else if (ft[v] == 0 && dt[v] < dt[u])
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

  void calcMaxLeaf(){
    fun1();
    //fun2();
  }

  int lich(int l){
    return 0;
  }
};

int main(int argc, char *argv[])
{
  ifstream in("input.txt");
  ofstream out("output.txt");

  int n, m;
  in >> n;
  m = n - 1;

  Graph g(n, Graph::UNDIRECTED, Graph::WEIGHTED);

  for (int i = 0; i < m; i++){
    Graph::Node u, v;
    int w;
    in >> u >> v >> w;
    g.insertEdge(u, v, w);
  }

  //Calculate
  g.calcMaxLeaf();

  g.print_maxLeaf();
  g.print_succ();

  int k;
  in >> k;

  for (int i = 0; i < k; i++){
    int l;
    in >> l;
    int res=0;
    if (n <= 2){
      res = n;
    }
    else{
      res = g.lich(l);
    }
    out << res << endl;
  }

  in.close();
  out.close();
  return 0;
}
