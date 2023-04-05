#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

/*

*/

class MF
{
  vector<int> f;
  vector<int> c;

public:
  MF(int n):
  f(n),
  c(n, 1){
    for (int x = 0; x < n; x++)
      f[x] = x;
  }
  int find(int x)
  {
    if (f[x] == x)
      return x;
    else
      return f[x] = find(f[x]);
  }
  void merge(int x, int y)
  {
    int fx = find(x);
    int fy = find(y);
    if (fx == fy)
      return void();
    
    if (c[fx] > c[fy])
    {
      f[fy] = fx;
      c[fx] += c[fy];
    }
    else
    {
      f[fx] = fy;
      c[fy] += c[fx];
    }
  }
  void decrSize(int x){
    c[find(x)]--;
  }
  int getSize(int x){
    return c[find(x)];
  }
};

class Graph
{
public:
  typedef int Node;
  const Node NIL = -1;
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
    friend bool operator==(const Edge e1, const Edge e2)
    {
      return e1.node == e2.node;
    }
  };

private:
  int n;
  vector<list<Edge>> adj;

  vector<Node[2]> succ;
  vector<int[2]> maxLeaf;
  
  vector<Node> i2u;
  vector<Node> parent;

public:
  Graph(int nodes) :
  n(nodes),
  adj(n),
  succ(n),
  maxLeaf(n),
  i2u(n),
  parent(n) {}

  ~Graph() {}
  void insertEdge(Node u, Node v, int weight)
  {
    adj[u].push_front(Edge(v, weight));
    adj[v].push_front(Edge(u, weight));
  }
  // Functions

  void fun1(Node r){
    struct sNode
    {
      Node node;
      Node parent;
      sNode(Node u, Node p)
      {
        node = u;
        parent = p;
      }
      ~sNode() {}
    };
    vector<bool> visited(n, false);
    stack<sNode> s;

    s.push(sNode(r, NIL));

    while (!s.empty())
    {
      sNode &snode = s.top();
      Node u = snode.node;
      Node p = snode.parent;
      if(!visited[u])
      { //pre order
        succ[u][0] = succ[u][1] = NIL;
        maxLeaf[u][0] = maxLeaf[u][1] = -1;
        //
        for(Edge e: adj[u]){
          Node v = e.node;
          if (v != p)
            s.push(sNode(v, u));
        }
        visited[u] = true;
      }else{
        //postorder
        for (Edge e : adj[u])
        {
          Node v = e.node;
          if (v != p)
          {
            int path0 = e.weight + maxLeaf[v][0];
            if (path0 >= maxLeaf[u][0]) // Importante! Ne devo avere almeno 2 in r
            {
              maxLeaf[u][1] = maxLeaf[u][0];
              succ[u][1] = succ[u][0];

              maxLeaf[u][0] = path0;
              succ[u][0] = v;
            }
            else if (path0 > maxLeaf[u][1])
            {
              maxLeaf[u][1] = path0;
              succ[u][1] = v;
            }
          }
        }
        if (succ[u][0] == NIL)
        { // I am a Leaf!
          succ[u][0] = u;
          maxLeaf[u][0] = 0;
        }
        //
        s.pop();
      }
    }
  }

  void fun2(Node r)
  {
    struct sNode
    {
      Node node;
      Node parent;
      int pweight;
      sNode(Node u, Node p, int pw)
      {
        node = u;
        parent = p;
        pweight = pw;
      }
      ~sNode() {}
    };
    stack<sNode> s;
    s.push(sNode(r, NIL, 0));

    while (!s.empty())
    {
      sNode &snode = s.top();s.pop();
      Node u = snode.node;
      Node p = snode.parent;
      int pw = snode.pweight;
      // pre order
      if (p != NIL)
      {
        int path0 = pw + maxLeaf[p][0];
        int path1 = pw + maxLeaf[p][1];
        if (succ[p][0] != u && succ[p][1] != u)
        {
          if (path0 >= maxLeaf[u][0])
          {
            maxLeaf[u][1] = maxLeaf[u][0];
            succ[u][1] = succ[u][0];

            maxLeaf[u][0] = path0;
            succ[u][0] = p;
          }
          else if (path0 > maxLeaf[u][1])
          {
            maxLeaf[u][1] = path0;
            succ[u][1] = p;
          }
          else if (path1 > maxLeaf[u][1])
          {
            maxLeaf[u][1] = path1;
            succ[u][1] = p;
          }
        }
        else if (succ[p][0] != u && succ[p][1] == u)
        {
          if (path0 >= maxLeaf[u][0])
          {
            maxLeaf[u][1] = maxLeaf[u][0];
            succ[u][1] = succ[u][0];

            maxLeaf[u][0] = path0;
            succ[u][0] = p;
          }
          else if (path0 > maxLeaf[u][1])
          {
            maxLeaf[u][1] = path0;
            succ[u][1] = p;
          }
        }
        else if (succ[p][0] == u && succ[p][1] != u)
        {
          if (path1 >= maxLeaf[u][0])
          {
            maxLeaf[u][1] = maxLeaf[u][0];
            succ[u][1] = succ[u][0];

            maxLeaf[u][0] = path1;
            succ[u][0] = p;
          }
          else if (path1 > maxLeaf[u][1])
          {
            maxLeaf[u][1] = path1;
            succ[u][1] = p;
          }
        }
      }
      //
      for (Edge e : adj[u])
      {
        Node v = e.node;
        if (v != p)
          s.push(sNode(v, u, e.weight));
      }
    }
  }

  void calcMaxLeaf(){
    Node r = 0;
    while (r < n - 1 && adj[r].size() == 1)
      r++;
    if (adj[r].size() == 1)
      return void();

    fun1(r);
    fun2(r);

    vector<pair<int, Node>> maxleafs = vector<pair<int, Node>>(n);

    for (Node u = 0; u < n; u++)
      maxleafs[u] = {maxLeaf[u][0], u};

    sort(maxleafs.begin(), maxleafs.end());
    reverse(maxleafs.begin(), maxleafs.end());

    for (int i = 0; i < n; i++)
      i2u[i] = maxleafs[i].second;

    bfs(i2u[n - 1]);
  }

  void bfs(Node r){
    queue<pair<Node, Node>> q;
    q.push({r, NIL});
    while(!q.empty()){
      Node u = q.front().first;
      Node p = q.front().second;
      q.pop();

      parent[u] = p;

      for (const Edge &e : adj[u])
      {
        const Node v = e.node;
        if (v != p)
        {
          q.push({v, u});
        }
      }
    }
  }
  int lich(int l)
  {
    MF mf(n);

    int res = 1;
    int j = 0;
    for (int i = 0; i < n; i++)
    {
      Node u = i2u[i];

      // merge children
      for (Edge e : adj[u])
      {
        Node v = e.node;
        if (v != parent[u]){
          if(maxLeaf[v][0] - maxLeaf[u][0] <= l)
            mf.merge(u, v);
        }
      }

      // remove
      while ( j < n && maxLeaf[i2u[j]][0] - maxLeaf[u][0] > l)
      {
        mf.decrSize(i2u[j]);
        j++;
      }

      res = max(res, mf.getSize(u));
    }
    return res;
  }
};

  int main(int argc, char *argv[])
  {
    ifstream in("input.txt");
    ofstream out("output.txt");

    int n, m;
    in >> n;
    m = n - 1;

    Graph g(n);

    for (int i = 0; i < m; i++)
    {
      Graph::Node u, v;
      int w;
      in >> u >> v >> w;
      g.insertEdge(u, v, w);
    }

    // Pre-Calculate stuff
    g.calcMaxLeaf();


    // Execute linch
    int k;
    in >> k;

    for (int i = 0; i < k; i++)
    {
      int l;
      in >> l;
      int res = 0;
      if (n <= 2)
        res = n;
      else
        res = g.lich(l);
      out << res << endl;
    }

    in.close();
    out.close();
    return 0;
}
