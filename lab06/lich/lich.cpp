#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

using namespace std;

/*

*/
class DisjointSet
{
public:
  DisjointSet(int n) : parent(n), rank(n), size(n, 1)
  {
    for (int i = 0; i < n; i++)
    {
      parent[i] = i;
    }
  }

  int find(int x)
  {
    if (parent[x] != x)
    {
      parent[x] = find(parent[x]);
    }
    return parent[x];
  }

  bool merge(int x, int y)
  {
    int px = find(x), py = find(y);
    if (px == py)
    {
      return false;
    }
    if (rank[px] < rank[py])
    {
      std::swap(px, py);
    }
    parent[py] = px;
    size[px] += size[py];
    if (rank[px] == rank[py])
    {
      rank[px]++;
    }
    return true;
  }

  int getSize(int x)
  {
    return size[find(x)];
  }

  int getMaxSize(){
    int res = -1;
    for(int s: size){
      res = max(res, s);
    }
    return res;
  }

private:
  std::vector<int> parent, rank, size;
};

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
    friend bool operator==(const Edge e1, const Edge e2)
    {
      return e1.node == e2.node;
    }
  };
  struct EdgeHash
  {
    std::size_t operator()(const Edge &edge) const
    {
      return std::hash<int>()(edge.node);
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
  vector<unordered_set<Edge, EdgeHash>> adj;
  vector<Node> succ0;
  vector<Node> succ1;
  
  vector<int> maxLeaf0;
  vector<int> maxLeaf1;

  vector<pair<int, Node>> maxleafs;
  unordered_map<int, Node> i2u;
  unordered_map<Node, int> u2i;

  vector<int> color;

public:
  Graph(int nodes, EdgeDirection eDir, EdgeWeight eWeight) :
  edgeDirection(eDir),
  edgeWeight(eWeight),
  n(nodes),
  V(vector<Node>(n)),
  adj(vector<unordered_set<Edge, EdgeHash>>(n)),
  succ0(vector<Node>(n)),
  succ1(vector<Node>(n)),
  maxLeaf0(vector<int>(n)),
  maxLeaf1(vector<int>(n)),
  maxleafs(vector<pair<int, Node>>(n)),
  i2u(unordered_map<int, Node>(n)),
  u2i(unordered_map<Node, int>(n)),
  color(vector<int>(n, -1))
  {
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
      adj[u].insert(Edge(v, weight));
      if (undirected())
        adj[v].insert(Edge(u, weight));
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
      cout << u << ':' << maxLeaf0[u] << ',' << maxLeaf1[u] << endl;
  }
  void print_succ()
  {
    cout << "succ" << endl;
    for (Node u = 0; u < n; u++)
      cout << u << ':' << succ0[u] << ',' << succ1[u] << endl;
  }
  void plt(){
    ofstream plt_file("plt.txt");
    for (Node u = 0; u < n; u++)
      plt_file << maxLeaf0[u] << ' ' << u2i[u] << endl;
    plt_file.close();
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
      return void();
    struct sNode
    {
      Node node;
      Node parent;
      unordered_set<Edge>::iterator iter;
      sNode(Node u, Node p, unordered_set<Edge>::iterator i)
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
            if (maxLeaf0[v] + e.weight > maxLeaf0[u])
            {
              maxLeaf1[u] = maxLeaf0[u];
              succ1[u] = succ0[u];

              maxLeaf0[u] = maxLeaf0[v] + e.weight;
              succ0[u] = v;
            }
          }
        }
        if(succ0[u] == -1){// I am a Leaf!
          succ0[u] = succ1[u] = u;
          maxLeaf0[u] = maxLeaf1[u] = 0;
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
          succ0[u] = succ1[u] = - 1;
          maxLeaf0[u] = maxLeaf1[u] = -1;
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
      return void();
    struct sNode
    {
      Node node;
      Node parent;
      int pweight;
      unordered_set<Edge>::iterator iter;
      sNode(Node u, Node p, int pw, unordered_set<Edge>::iterator i)
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
          if(p != -1){
            if(succ0[p] != u && (pw+maxLeaf0[p] >= maxLeaf0[u])){
              maxLeaf1[u] = maxLeaf0[u];
              succ1[u] = succ0[u];
              
              maxLeaf0[u] = pw + maxLeaf0[p];
              succ0[u] = p;
            }
            if(succ1[p] != u){ // parent is not a leaf, so it has 2 paths
              if ((pw + maxLeaf1[p] >= maxLeaf0[u])){
                // => could have given maxLeaf0[u] := pw+maxLeaf0[p]
                // and they had the same weight
                if(succ0[u] != p){
                  maxLeaf1[u] = maxLeaf0[u];
                  succ1[u] = succ0[u];

                  maxLeaf0[u] = pw + maxLeaf1[p];
                  succ0[u] = p;                  
                }
              }
              if(pw+maxLeaf1[p] >= maxLeaf1[u]){
                if(succ0[u] != p){
                  maxLeaf1[u] = pw + maxLeaf1[p];
                  succ1[u] = p;
                }
              }
            }
          }
        }
        auto &iter = snode.iter;
        auto iterEnd = adj[u].end();
        while (iter != iterEnd)
        {
          Edge e = *iter;
          Node &v = e.node;
          if (dt[v] == 0)
          { // Tree Edge
            iter++;
            s.push(sNode(v, u, e.weight, adj[v].begin())); // here I expect a return value
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
    fun2();
    for (Node u = 0; u < n; u++)
      maxleafs[u] = {maxLeaf0[u], u};

    sort(maxleafs.begin(), maxleafs.end());

    for (int i = 0; i < n; i++)
    {
      auto item = maxleafs[i];
      i2u.insert({i, item.second});
      u2i.insert({item.second, i});
    }
  }

  struct dpnode
  {
    int min;
    int max;
    int enne;
    dpnode(int _min, int _max, int _enne)
    {
      min = _min;
      max = _max;
      enne = _enne;
    }
    ~dpnode() {}
  };

  int lich(int l){
    //
    int res = 0;
    for (int i = 0; i < n; i++)
    {
      int j = i;
      while(j < n-1 && (maxleafs[j].first-maxleafs[i].first) <= l)
        j++;
      if((maxleafs[j].first - maxleafs[i].first) > l)
        j--;
      //Ora devo creare un mfset con j-i+1 elementi
      int k = j-i+1;
      // Esploro il grafo con una multi-source bfs
      DisjointSet ds = DisjointSet(k);

      for (int h1 = i; h1 <= j; h1++){
        Node u = i2u[h1];
        for (int h2 = i; h2 <= j; h2++)
        {
          Node v = i2u[h2];
          if (adj[u].find({v, 0}) != adj[u].end())
            ds.merge(h1-i, h2-i);
        }
      }
      res = max(res, ds.getMaxSize());
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

  Graph g(n, Graph::UNDIRECTED, Graph::WEIGHTED);

  for (int i = 0; i < m; i++){
    Graph::Node u, v;
    int w;
    in >> u >> v >> w;
    g.insertEdge(u, v, w);
  }

  //Calculate
  g.calcMaxLeaf();

  //g.plt();

  //g.print_maxLeaf();
  //g.print_succ();

  int k;
  in >> k;

  for (int i = 0; i < k; i++){
    int l;
    in >> l;
    int res=0;
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
