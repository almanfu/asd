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

class Graph
{
public:
  typedef int Node;
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
  vector<unordered_set<Node>> adj;
  vector<int> color;

public:
  Graph(int nodes, EdgeDirection eDir)
  {
    edgeDirection = eDir;
    n = nodes;
    V = vector<Node>(n);
    adj = vector<unordered_set<Node>>(n);
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
  void insertEdge(Node u, Node v)
  {
    if (has(u) && has(v) && u != v)
    {
      adj[u].insert(v);
      if (undirected())
        adj[v].insert(u);
    }
  }
  // Functions
  void print_adj()
  {
    for (Node u = 0; u < n; u++)
    {
      cout << u << ':';
        for (Node v : adj[u])
          cout << v << ' ';
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
      for (Node v : adj[u])
      {
        if (!visited[v])
        {
          q.push(v);
          visited[v] = true;
        }
      }
    }
  }

  void fun(int &res)
  {
    Node r = 0;
    struct sNode
    {
      Node node;
      unordered_set<Node>::iterator iter;
      bool haveLeaves;
      bool retval;
      sNode(Node u, unordered_set<Node>::iterator i)
      {
        node = u;
        iter = i;
        haveLeaves = false;
        retval = false;
      }
      ~sNode() {}
    };
    stack<sNode> s;
    s.push(sNode(r, adj[r].begin()));
    vector<int> dt(n, 0);
    vector<int> ft(n, 0);
    int time = 0;
    bool retval_popped = false;
    while (!s.empty())
    {
      sNode &snode = s.top();
      Node &u = snode.node;
      bool &haveLeaves = snode.haveLeaves;
      bool &retval = snode.retval;
      time++;
      if (ft[u] != 0)
      {
        // Post-order visit
        bool haveLeaves = false;
        for (Node v : adj[u]){
          if(haveLeaves |= adj[v].size() == 1)
            break;
        }
        if(haveLeaves){
          res += 1;
          for(Node v: adj[u]){
            adj[v].erase(u);
          }
          adj[u].clear();            
        }
        s.pop(); // Here I give a return value
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
          Node v = *iter;
          if (dt[v] == 0)
          { // Tree Edge
            iter++;
            s.push(sNode(v, adj[v].begin())); // here I expect a return value
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
};

int main(int argc, char *argv[])
{
  ifstream in("input.txt");
  ofstream out("output.txt");

  int n, m;
  in >> n;
  m = n - 1;

  Graph g(n, Graph::UNDIRECTED);

  for (int i = 0; i < m; i++)
  {
    Graph::Node u, v;
    in >> u >> v;
    g.insertEdge(u, v);
  }

  int res=0;

  g.fun(res);

  out << res;
  
  in.close();
  out.close();
  return 0;
}
