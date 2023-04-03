#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <unordered_set>
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

  int getMaxSize()
  {
    int res = -1;
    for (int s : size)
    {
      res = max(res, s);
    }
    return res;
  }

  void decrSize(int x){
    int px = find(x);
    size[px]--;
  }

private:
  std::vector<int> parent, rank, size;
};

class Graph
{
public:
  typedef int Node;
  const Node NIL = -1;
  const int MINF = -1;
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
  vector<list<Edge>> adj;

  vector<Node[2]> succ;
  vector<int[2]> maxLeaf;

  vector<pair<int, Node>> maxleafs;
  vector<Node> i2u;
  vector<int> u2i;

  vector<Node> parent;

public:
  Graph(int nodes, EdgeDirection eDir, EdgeWeight eWeight) :
  edgeDirection(eDir),
  edgeWeight(eWeight),
  n(nodes),
  adj(vector<list<Edge>>(n)),
  succ(vector<Node[2]>(n)),
  maxLeaf(vector<int[2]>(n)),
  maxleafs(vector<pair<int, Node>>(n)),
  i2u(vector<Node>(n)),
  u2i(vector<int>(n)),
  parent(n){}

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
      cout << u << ':' << maxLeaf[u][0] << ',' << maxLeaf[u][1] << endl;
  }
  void print_succ()
  {
    cout << "succ" << endl;
    for (Node u = 0; u < n; u++)
      cout << u << ':' << succ[u][0] << ',' << succ[u][1] << endl;
  }
  void plt(){
    ofstream plt_file("plt.txt");
    bool root = true;
    for (Node u = 0; u < n; u++){
      plt_file << (adj[u].size() == 1 ? "lf" : (root ? "root" : "nlf" )) << ' ' << u2i[u] << ' ' << maxleafs[u2i[u]].first << endl;
      if (adj[u].size() != 1){
        root = false;
      }
    }
      
    plt_file.close();
  }

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

    for (Node u = 0; u < n; u++)
      maxleafs[u] = {maxLeaf[u][0], u};

    sort(maxleafs.begin(), maxleafs.end());
    reverse(maxleafs.begin(), maxleafs.end());

    for (int i = 0; i < n; i++)
    {
      auto item = maxleafs[i];
      i2u[i]=item.second;
      u2i[item.second]=i;
    }
  }

  /*
  int lich(int l){
    //
    int res = 0;
    int i = 0;
    int j = 0;
    int left = 0;
    int right = 0;

    while (j < n && i < n)
    { 
      // con ricerca dicotomica, porto avanti j
      left = j;
      right = n-1;
      while(left != right){
        int m = (left + right + 1) / 2;
        if ((maxleafs[m].first - maxleafs[i].first) <= l)
          left = m;
        else
          right = m - 1;
      }
      j = left;

      //Ora devo creare un mfset con j-i+1 elementi
      int k = j-i+1;
      vector<int> cc = vector<int>(k, -1);
      vector<int> cc_size = vector<int>(k, 0);
      int id = 0;
      for (int ir = i; ir <= j; ir++)
      {
        if (cc[ir-i] == -1)
        {
          // BFS
          queue<int> q;
          q.push(ir);
          cc[ir-i] = id;
          cc_size[id] = 1;
          while (!q.empty())
          {
            int iu = q.front();
            q.pop();
            for (Edge e : adj[i2u[iu]])
            {
              Node &v = e.node;
              int iv = u2i[v];
              if ( i <= iv && iv <= j && cc[iv-i] == -1)
              {
                q.push(iv);
                cc[iv-i] = id;
                cc_size[id]++;
                
              }
            }
          }
          id++;
        }
      }
      
      for (int p = 0; p < k; p++){
        res = max(res, cc_size[p]);
      }
      // con ricerca dicotomica, porto avanti i
      j++;
      left = i;
      right = j;
      while (j < n && left != right)
      {
        int m = (left + right) / 2;
        if ((maxleafs[j].first - maxleafs[m].first) > l)
          left = m+1;
        else
          right = m;
      }
      i = left;
    }
    return res;
  }
  
  int lich(int &l){
    int res = 1;
    lich1(i2u[0], -1, l, res);
    return res;
  };


  void lich1(Node u, Node p, int &l, int&res)
  {
    
    // pre-order

    for(Edge e: adj[u]){
      Node &v = e.node;
      if(v != p){
        // in-order
        lich1(v, u, l, res);
      }
    }

    // post-order

    int size = 1;

    for (Edge e : adj[u])
    {
      Node &v = e.node;
      if(v != p){
        if(maxLeaf[v][0] - maxLeaf[u][0] <= l){
          size++;
          lich2(v, u, u, l, size);
        }
      }
    }

    res = max(res, size);
  }

  void lich2(Node u, Node p, Node r, int &l, int &size){
    // pre-order

    for(Edge e: adj[u]){
      Node &v = e.node;
      if(v != p){
        // in-order
        if (maxLeaf[v][0] - maxLeaf[r][0] <= l)
        {
          size++;
          lich2(v, u, r, l, size);
        }
      }
    }
    // post-order
  }*/
  void dfs(Node r)
  {
    std::stack<std::pair<int, int>> s;
    s.push({r, NIL});

    while (!s.empty())
    {
      Node u = s.top().first;
      Node p = s.top().second;
      s.pop();

      parent[u] = p;

      for (const Edge &e : adj[u])
      {
        const Node v = e.node;
        if (v != p){
          s.push({v, u});
        }
      }
    }
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
    bfs(i2u[n-1]);
    DisjointSet ds(n);

    int res = 1;
    int j = 0;
    for (auto it : maxleafs)
    {
      Node &u = it.second;
      int &maxpath = it.first;
      
      // merge children
      for (Edge e : adj[u])
      {
        Node &v = e.node;
        if (v != parent[u]){
          if(maxLeaf[v][0] - maxLeaf[u][0] <= l)
            ds.merge(u, v);
        }
      }

      // remove
      while ( j < n && maxLeaf[i2u[j]][0] - maxLeaf[u][0] > l)
      {
        ds.decrSize(i2u[j]);
        j++;
      }

      res = max(res, ds.getSize(u));
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

    for (int i = 0; i < m; i++)
    {
      Graph::Node u, v;
      int w;
      in >> u >> v >> w;
      g.insertEdge(u, v, w);
    }

    // Calculate
    g.calcMaxLeaf();

    // g.plt();

    // g.print_maxLeaf();
    // g.print_succ();

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
