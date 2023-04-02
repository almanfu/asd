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
  vector<list<Node>> adj;
  vector<int> color;
  vector<int> weight;

public:
  Graph(int nodes, EdgeDirection eDir)
  {
    edgeDirection = eDir;
    n = nodes;
    V = vector<Node>(n);
    weight = vector<int>(n);
    adj = vector<list<Node>>(n);
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
  void assignWeight(Node u, int w){
    weight[u] = w;
  }
  void insertEdge(Node u, Node v)
  {
    if (has(u) && has(v) && u != v)
    {
      adj[u].push_front(v);
      if (undirected())
        adj[v].push_front(u);
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

  int fun()
  {
    vector<vector<int>> dp = vector<vector<int>>(n, vector<int>(2, 0));
    Node r = 0;
    struct sNode
    {
      Node node;
      Node parent;
      list<Node>::iterator iter;
      int dp0, dp1;
      sNode(Node u, Node p, int w, list<Node>::iterator i)
      {
        node = u;
        parent = p;
        iter = i;
        dp0 = 0;
        dp1 = w;
      }
      ~sNode() {}
    };
    stack<sNode> s;
    s.push(sNode(r, -1, weight[r], adj[r].begin()));
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
        if(u != r && adj[u].size() == 1){
          dp[u][0] = 0;
          dp[u][1] = weight[u];
        }else{

          dp[u][0] = 0;
          for(Node v: adj[u]){
            if(v != p)
              dp[u][0] += dp[v][1];
          }

          dp[u][1] = weight[u];
          for(Node v: adj[u]){
            if(v != p)
              dp[u][1] += min(dp[v][0], dp[v][1]);
          }

        }
        s.pop(); // Here I give the return value
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
            s.push(sNode(v, u, weight[v], adj[v].begin())); // here I expect a return value
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
    return min(dp[0][0], dp[0][1]);
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

  for (int u = 0; u < n; u++){
    int w;
    in >> w;
    g.assignWeight(u, w);
  }

  for (int i = 0; i < m; i++){
    Graph::Node u, v;
    in >> u >> v;
    g.insertEdge(u, v);
  }

  out << g.fun();

  in.close();
  out.close();
  return 0;
}
