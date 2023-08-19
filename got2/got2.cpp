#include <iostream>
#include <sstream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <cmath>
#include <algorithm>
#include <unordered_set>

#include "got2.h"

using namespace std;

bool INFO = false;

typedef vector<vector<bool>> Graph;
typedef int Node;
typedef int Cluster;

/*
  FORCE Heuristic
  O(Rn^2+kn^2)
  k ~ 300
*/

struct Point{
  double x, y;
};

double distance(Point a, Point b)
{
  double dx = a.x - b.x;
  double dy = a.y - b.y;
  return sqrt(dx * dx + dy * dy);
}

void info(vector<Point> pos, int r){
  if(!INFO)
    return void();
  std::ostringstream path;
  path << "layout/" << r << ".txt";
  ofstream f(path.str());
  for (Node u = 0; u < pos.size(); u++)
    f << pos[u].x << ' ' << pos[u].y << endl;
  f.close();
}

vector<Point> layout(Graph &G, int n, double rho, double fatt, double frep, double M0, double r, int R)
{
  vector<Point> pos(n);
  double M = M0;

  // Randomization
  vector<Node> v(n);
  for (Node u = 0; u < n; u++)
    v[u] = u;
  next_permutation(v.begin(), v.end());

  // Arrange nodes in a circle.
  // O(n)
  for (int i = 0; i < n; i++)
    pos[v[i]] = {rho * cos(2 * M_PI * i / n), rho * sin(2 * M_PI * i / n)};

  // O(R n^2)
  for (int r = 0; r < R; r++)
  {
    info(pos, r);
    
    // Compute displacements delta
    vector<Point> delta(n, {0, 0});

    // O(n^2)
    for (Node u = 0; u < n; u++)
    {
      for (Node v = 0; v < u; v++)
      {
        double d = distance(pos[u], pos[v]);
        double strength;

        // Force is attractive if there is an edge
        // Repulsive if there is no edge
        if (G[u][v])
          strength = (fatt * log(d + 1));
        else
          strength = (frep / (log(d + 1)));
        // Force-Vector
        Point f_v2u = {strength * (pos[v].x - pos[u].x) / d, strength * (pos[v].y - pos[u].y) / d};

        // Action
        delta[u].x += f_v2u.x;
        delta[u].y += f_v2u.y;
        
        // Reaction
        delta[v].x -= f_v2u.x;
        delta[v].y -= f_v2u.y;
      }
    }

    // Apply the displacements (capped to a maximum of max(M,1))
    // O(n)
    for (Node u = 0; u < n; u++)
    {
      double d = distance({0, 0}, delta[u]);
      if(d > max(M,1.)){
        delta[u].x = M*delta[u].x/d;
        delta[u].y = M*delta[u].y/d;
      }
      pos[u].x += delta[u].x;
      pos[u].y += delta[u].y;
    }
    M *= r;
  }

  return pos;
}

// Floyd-Warshall
Graph warshall(const Graph &G, int n)
{
  Graph Gc = G;  
  for (int u = 0; u < n; u++){
    for (int v = 0; v < n; v++){
      for (int k = 0; k < n; k++)
      Gc[u][v] = Gc[u][v] || (Gc[u][k] && Gc[k][v]);
    }
  }
  return Gc;
}

void ccsdfs(Node u, const Graph &G, int n, vector<int> &cc, vector<bool> &visited)
{
  visited[u] = true;
  cc.push_back(u);
  for (Node v = 0; v < n; v++)
  {
    if (G[u][v] && !visited[v])
    {
      ccsdfs(v, G, n, cc, visited);
    }
  }
}

void ccsdfs_iterative(Node r, vector<list<Node>> adj, int n, vector<int> &cc, vector<bool> &visited)
{
  stack<Node> dfsStack;
  dfsStack.push(r);

  while (!dfsStack.empty())
  {
    Node u = dfsStack.top();
    dfsStack.pop();

    if (!visited[u])
    {
      visited[u] = true;
      cc.push_back(u);

      for (Node v : adj[u])
      {
      if (!visited[v])
      {
          dfsStack.push(v);
      }
      }
    }
  }
}

// ccs[j][i] = i-th element of the j-th cc
vector<vector<int>> get_ccs(vector<list<Node>> adj, int n)
{
  vector<bool> visited(n, false);
  vector<vector<int>> ccs;

  for (Node u = 0; u < n; u++)
  {
    if (!visited[u])
    {
      vector<int> cc;
      ccsdfs_iterative(u, adj, n, cc, visited);
      ccs.push_back(cc);
    }
  }

  return ccs;
}

Graph transitive_closure(const Graph &G, vector<list<Node>> adj, int n)
{
  Graph Gc(n, vector<bool>(n, false));

  // Get connected components
  auto ccs = get_ccs(adj, n);

  for (const auto &cc : ccs)
  {
    for (int i = 0; i < cc.size(); i++)
    {
      for (int j = 0; j < cc.size(); j++)
        Gc[cc[i]][cc[j]] = true;
    }
  }

  return Gc;
}

struct Cost{
  int adds;
  int rems;
  Cost() : adds(0), rems(0) {}
  Cost(int adds, int rems) : adds(adds), rems(rems) {}
  int tot() { return adds + rems; }
};

bool operator<(Cost& a, Cost& b){
  return (a.adds + a.rems) < (b.adds + b.rems);
}

const Cost INF(INFINITY, 0);

// Cost of a partition
Cost cost(const Graph &G, const Graph &G1)
{
  Cost c;
  for (Node u = 0; u < G.size(); u++)
  {
    for (Node v = 0; v < u; v++)
    {
      if (!G[u][v] && G1[u][v])
        c.adds++;
      if (G[u][v] && !G1[u][v])
        c.rems++;
    }
  }
  return c;
}

// Clustering
void clustering(const Graph &G, int n, const vector<Point> &pos,
               double delta_init, double delta_max, double sigma_init, double alpha, ofstream& out)
{
  double delta = delta_init;
  double sigma = sigma_init;
  Cost costMin = INF;
  Graph G_min (n, vector<bool>(n, false));

  // O(k n^2)
  while (delta <= delta_max)
  {
    Graph G_delta(n, vector<bool>(n, false));
    vector<list<Node>> adj_delta(n);

    // Construct auxiliary graph G_delta
    // O(n^2)
    for (Node u = 0; u < n; u++)
    {
      for (Node v=u+1; v<n; v++)
      {
        if (distance(pos[u], pos[v]) <= delta){
          G_delta[u][v] = G_delta[v][u] = true;
          adj_delta[u].push_front(v);
          adj_delta[v].push_front(u);
        }
      }
    }
    
    // O(n+m*)
    Graph Gc_delta = transitive_closure(G_delta, adj_delta, n);
    
    // O(n^2)
    Cost costHere = cost(G, Gc_delta);
    if (costHere < costMin)
    {
      G_min = Gc_delta;
      costMin = costHere;
    }

    sigma *= alpha;
    delta += sigma;
  }

  // O(n^2)
  out << costMin.adds << ' ' << costMin.rems << endl;
  // Print Additions/Removals
  for (Node u = 0; u < n; u++){
    for (Node v = 0; v < u; v++){
      if (!G[u][v] && G_min[u][v])
        out << "+ " << u << ' ' << v << endl;
      if(G[u][v] && !G_min[u][v])
        out << "- " << u << ' ' << v << endl;
    }
  }
  out << "***" << endl;
  // cout << costMin;
}

struct nInfo
{
  Node u;
  int c;
  nInfo() : u(-1), c(0) {}
  nInfo(Node u, int c) : u(u), c(c) {}
};

struct cInfo{
  int s; // size -- number of nodes
  Cost c;    // cost
  double sc; // specific cost
  cInfo() : s(0), c(), sc(0.) {}
  cInfo(int s, Cost c) : s(s), c(c){
    updateSC();
  }
  void updateSC() { sc = (s > 0 ? (double)c.tot() / s : 0); }
};

int
main()
{
  ifstream in("input.txt");
  ofstream out("output.txt");

  int n, m;
  in >> n >> m;

  if(n >= 0){// O(I(n+m))
    LOCALSEARCH:

    vector<unordered_set<Node>> adj(n);
    for (int i = 0; i < m; i++)
    {
      int u, v;
      in >> u >> v;
      adj[u].insert(v);
      adj[v].insert(u);
    }
    //
    const int ITER = 1000;
    //
    out << "1 1" << endl;
    out << "***" << endl;

    vector<Cluster> ncid(n); // Node -> Cluster
    vector<nInfo> ni(n);    // Node -> NodeInfo
    vector<cInfo> ci(n); // Cluster -> ClusterInfo
    vector<list<Node>> c(n); // Cluster -> List of Nodes

    unordered_set<Node> tabu; // tabu list

    std::sort(ni.begin(), ni.end(), [](const nInfo &a, const nInfo &b)
              { return a
              .c > b.c; });
    
    // starting point
    for (Node u = 0; u < n; u++){
      ncid[u] = u;
      ni[u] = {u, adj[u].size()};
      ci[u] = {1, {0, adj[u].size()}};
      c[u].push_front(u);
    }

    // hill climbing
    Cost minCost(0, m);
    for (int i = 0; i < ITER; i++)
    {
      int minDelta = 0;

      // pick maximum cost node not in tabu
      Node u = find_if(ni.begin(), ni.end(), [&](nInfo& x)
                          {return tabu.find(x.u) == tabu.end(); })->u;
      Cluster old_cid = ncid[u];
      Cluster new_cid = old_cid;
      int so = ci[old_cid].s; // size old, before removing node

      // explore neighborhood O(n*m)
      // (moving selected node to another cluster)
      for (Cluster cid = 0; cid < n; cid++)
      {
        int delta = 0;
        // compute delta
        int sn = ci[cid].s; // size new
        int oldrems = 0;
        int newrems = 0;
        for (Node v : adj[u]) // uv exists
        {
          if(ncid[v] == cid){
            // add back edge
            oldrems++; // that was removed before
          }
          if (ncid[v] == old_cid)
          {
            // remove edge
            newrems++; // that was not removed before
          }
          // OBS:  cid == old_cid => delta == 0
        }
        int newadds = sn - oldrems;
        int oldadds = so - newrems;
        delta = +newrems-oldadds+newadds-oldrems;
        if (delta < minDelta)
        {
          new_cid = cid;
          minDelta = delta;
        }
      }
      if(minDelta >= 0){
        tabu.insert(u);
        continue;
      }
      // update data structures
      // - ni
      // - ci
      // - c
      // - ncid

      // - c
      c[old_cid].remove(u);
      // - ni without moved node in either component
      for (Node v : c[new_cid])
        ni[v].c += 1;
      for (Node v : c[old_cid])
        ni[v].c -= 1;
      // - c - ncid
      c[new_cid].push_front(u);
      ncid[u] = new_cid;

      // - ci
      auto &new_ci = ci[new_cid];
      auto &old_ci = ci[old_cid];
      int sn = new_ci.s; // before adding node
      new_ci.s++;
      old_ci.s--;

      // recalculate oldrems, newrems (consider doing this before later)
      int oldrems = 0;
      int newrems = 0;
      for (Node v : adj[u]) // uv exists
      {
        if (ncid[v] == new_cid)
        {
          // add back edge
          oldrems++; // that was removed before
          ni[v].c-=2; // will add 1 to all in c[new_cid] after
        }
        if (ncid[v] == old_cid)
        {
          // remove edge
          newrems++; // that was not removed before
          ni[v].c += 2; // will add -1 to all in c[old_cid] after
        }
        // OBS:  cid == old_cid => delta == 0
      }
      int newadds = sn - oldrems;
      int oldadds = so - 1 - newrems;

      // - ci
      new_ci.c.adds += newadds;
      new_ci.c.rems += newrems - oldrems;
      old_ci.c.adds -= oldadds;
      old_ci.c.rems -= oldrems;

      new_ci.updateSC();
      old_ci.updateSC();

      // - ni (moved node)
      // OBS delta == delta cost of node
      ni[u].c += newadds - oldrems + newrems - oldrems;

      std::sort(ni.begin(), ni.end(), [](const nInfo &a, const nInfo &b)
                { return a.c > b.c; });

      // sum of sum of nodes cost != sum of cluster costs != cost
      // minCost
      minCost.adds += newadds - oldadds;
      minCost.rems += newrems - oldrems;
    }

    // print solution
    out << minCost.adds << ' ' << minCost.rems << endl;
    for (Node u = 0; u < n; u++){
      for (Node v = 0; v < u; v++){
        if( ncid[u] == ncid[v] && adj[u].find(v) == adj[u].end())
          out << "+ " << u << ' ' << v << endl;
        if (ncid[u] != ncid[v] && adj[u].find(v) != adj[u].end())
          out << "- " << u << ' ' << v << endl;
      }
    }
    out << "***" << endl;
  }else{// O(Rn^2+kn^2)
    FORCE:
    Graph G(n, vector<bool>(n, false));
    for (int i = 0; i < m; i++)
    {
      int u, v;
      in >> u >> v;
      G[u][v] = true;
      G[v][u] = true;
    }

    //
    const int R=50;
    const double rho = n*10/(6.28);
    const double fatt=1.245;
    const double frep=1.687;
    const double M0 = 633;
    const double r = 0.99;
    //
    const double delta_init=0;
    const double delta_max=200;
    const double sigma_init=0.01;
    const double alpha=1.1+0.0000001*n;

    out << "0 0" << endl;
    out << "***" << endl;

    // Layout
    vector<Point> pos = layout(G, n, rho, fatt, frep, M0, r, R);

    // Clustering
    clustering(G, n, pos, delta_init, delta_max, sigma_init, alpha, out);
  }

  in.close();
  out.close();

  return 0;
}