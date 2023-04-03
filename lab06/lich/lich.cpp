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
  vector<list<Edge>> adj;

  vector<Node[2]> succ;
  vector<int[2]> maxLeaf;

  vector<pair<int, Node>> maxleafs;
  vector<Node> i2u;
  vector<int> u2i;

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
  u2i(vector<int>(n)){}

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
    while (r < n - 1 && adj[r].size() == 1)
      r++;
    if (adj[r].size() == 1)
      return void();
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
    s.push(sNode(r, NIL, adj[r].begin()));
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
            int path0 = e.weight+maxLeaf[v][0];
            if (path0 >= maxLeaf[u][0]) // Importante! Ne devo avere almeno 2 in r
            {
              maxLeaf[u][1] = maxLeaf[u][0];
              succ[u][1] = succ[u][0];

              maxLeaf[u][0] = path0;
              succ[u][0] = v;
            }
            else if (path0 > maxLeaf[u][1]){
              maxLeaf[u][1] = path0;
              succ[u][1] = v;
            }
          }
        }
        if (succ[u][0] == NIL){ // I am a Leaf!
          succ[u][0] = succ[u][1] = u;
          maxLeaf[u][0] = maxLeaf[u][1] = 0;
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
          succ[u][0] = succ[u][1] = NIL;
          maxLeaf[u][0] = maxLeaf[u][1] = -1;
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
    while (r<n-1 && adj[r].size() == 1)
      r++;
    if (adj[r].size() == 1)
      return void();
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
    s.push(sNode(r, NIL, 0, adj[r].begin()));
    vector<int> dt(n, 0);
    vector<int> ft(n, 0);
    int time = 0;
    while (!s.empty())
    {
      sNode &snode = s.top();
      Node &u = snode.node;
      Node &p = snode.parent;
      int &pw = snode.pweight;
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
          if (p != NIL)
          {
            int path0 = pw + maxLeaf[p][0];
            int path1 = pw + maxLeaf[p][1];
            if (succ[p][0] != u && succ[p][1] != u){
              if(path0 >= maxLeaf[u][0]){
                maxLeaf[u][1] = maxLeaf[u][0];
                succ[u][1] = succ[u][0];

                maxLeaf[u][0] = path0;
                succ[u][0] = p;
              }else if(path0 > maxLeaf[u][1]){
                maxLeaf[u][1] = path0;
                succ[u][1] = p;
              }else if(path1 > maxLeaf[u][1]){
                maxLeaf[u][1] = path1;
                succ[u][1] = p;
              }
            }
            else if (succ[p][0] != u && succ[p][1] == u){
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
            else if(succ[p][0] == u && succ[p][1] != u){
              if(path1 >= maxLeaf[u][0]){
                maxLeaf[u][1] = maxLeaf[u][0];
                succ[u][1] = succ[u][0];

                maxLeaf[u][0] = path1;
                succ[u][0] = p;
              }else if(path1 > maxLeaf[u][1]){
                maxLeaf[u][1] = path1;
                succ[u][1] = p;
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
      maxleafs[u] = {maxLeaf[u][0], u};

    sort(maxleafs.begin(), maxleafs.end());

    for (int i = 0; i < n; i++)
    {
      auto item = maxleafs[i];
      i2u[i]=item.second;
      u2i[item.second]=i;
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
    int i = 0;
    int j = 0;
    //<unordered_set<Node> U = unordered_set<Node>(n);
    //U.insert(i2u[j]);
    while (j < n - 1)
    {
      while(j < n-1 && (maxleafs[j].first-maxleafs[i].first) <= l){
        j++;
        //U.insert(i2u[j]);
      }
      if((maxleafs[j].first - maxleafs[i].first) > l){
        //U.erase(i2u[j]);
        j--;
      }
      
      //Ora devo creare un mfset con j-i+1 elementi
      int k = j-i+1;
      /* Esploro il grafo con una multi-source bfs
      DisjointSet ds = DisjointSet(k);

      for (int h1 = i; h1 <= j; h1++){
        Node u = i2u[h1];
        for (int h2 = i; h2 <= j; h2++)
        {
          Node v = i2u[h2];
          if (adj[u].find({v, 0}) != adj[u].end())
            ds.merge(h1-i, h2-i);
        }
      }*/
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

      //U.erase(i2u[i]);
      i++;
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
