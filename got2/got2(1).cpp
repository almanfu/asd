#include <iostream>
#include <sstream>
#include <fstream>

#include <vector>
#include <list>
#include <stack>
#include <cmath>
#include <algorithm>
#include <unordered_map>
#include <random>
#include <chrono>

#include "got2.h"

using namespace std;

bool INFO = false;

typedef vector<vector<bool>> Graph;
typedef int Node;
typedef int Cluster;

/*
  FORCE Heuristic
  O(Rn^2+kn^2)
  k ~ 60
  LOCAL SEARCH
  O(I2*I2*(n*maxdeg+nlg n))
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
    // info(pos, r);
    
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

void ccsdfs(Node u, const Graph &G, int n, vector<Node> &cc, vector<bool> &visited)
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

void ccsdfs_iterative(Node r, vector<list<Node>> adj, int n, vector<Node> &cc, vector<bool> &visited)
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
vector<vector<Node>> get_ccs(vector<list<Node>> adj, int n)
{
  vector<bool> visited(n, false);
  vector<vector<Node>> ccs;

  for (Node u = 0; u < n; u++)
  {
    if (!visited[u])
    {
      vector<Node> cc;
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

// Clustering
vector<vector<Node>> clustering(const Graph &G, int n, const vector<Point> &pos,
               double delta_init, double delta_max, double sigma_init, double alpha)
{
  double delta = delta_init;
  double sigma = sigma_init;
  Cost costMin = INF;
  Graph G_min (n, vector<bool>(n, false));

  // O(k n^2)
  while (delta <= delta_max)
  {
    vector<list<Node>> adj_delta(n);

    // Construct auxiliary graph G_delta
    // O(n^2)
    for (Node u = 0; u < n; u++)
    {
      for (Node v = 0; v < u; v++)
      {
        if (distance(pos[u], pos[v]) <= delta){
          adj_delta[u].push_front(v);
          adj_delta[v].push_front(u);
        }
      }
    }
    
    // O(n+m*)
    return get_ccs(adj_delta, n);
  }
}

struct nInfo
{
  Node u;
  int c;
  nInfo() : u(-1), c(0) {}
  nInfo(Node u, int c) : u(u), c(c) {}
};

struct cInfo{
  Cluster cid;
  int s;     // size -- number of nodes
  Cost c;    // cost
  double sc; // specific cost
  cInfo() : cid(-1), s(0), c(), sc(0.) {}
  cInfo(Cluster cid, int s, Cost c) : cid(cid), s(s), c(c){
    updateSC();
  }
  void updateSC() { sc = (s > 0 ? (double)c.tot() / s : 0); }
  void update(int _s, int incrCostAdds, int incrCostRems){
    s = _s;
    c.adds += incrCostAdds;
    c.rems += incrCostRems;
    updateSC();
  }
};

int main()
{
  ifstream in("input.txt");
  ofstream out("output.txt");


  int n, m;
  in >> n >> m;
  Graph G(n, vector<bool>(n, false));

  if(n > 1001)
  { // O (CLIMBS*I*(n*maxdeg+nlg n))
  LOCALSEARCH:
    // auto end_time = chrono::high_resolution_clock::now() + std::chrono::milliseconds(5000);
    // std::chrono::high_resolution_clock::duration rem_time = std::chrono::milliseconds(5000);
    // std::chrono::high_resolution_clock::duration safe_print_time;
    // bool firstPrint = true; // chrono first print
    // int merges = 0;
    // int moves = 0;
    // LS
    int prints = 0;
    const int CLIMBS = 20;
    const int I = 500;
    int tabuPauseNodes = 50;
    const int tabuPauseClusters = 50;
    const int tabuPauseNodes_delta = 3;
    double e_kin = n * 2;
    const double t_alpha = 0.9;
    const int maxEmptyTries = 5;
    
    // FORCE Layout
    const int n0 = 250;
    const int R = 50;
    const double rho = n0 * 10 / (6.28);
    const double fatt = 1.245;
    const double frep = 1.687;
    const double M0 = 633;
    const double r = 0.99;
    // FORCE Clustering
    const double delta_init = 10;
    const double delta_max = 200;
    const double sigma_init = 0.01;
    const double alpha = 1.1 + 0.0000001 * n0;

    const double e = std::exp(1.0);
    vector<vector<Node>> adj(n);
    for (int i = 0; i < m; i++)
    {
      Node u, v;
      in >> u >> v;
      G[u][v] = G[v][u] = true;
      adj[u].push_back(v);
      adj[v].push_back(u);
    }
    std::random_device rd;                          
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0); // Uniform distribution between 0 and 1

    //
    out << "1 1" << endl; // output malformato
    out << "***" << endl;

    vector<Cluster> ncid(n); // Node -> Cluster
    vector<nInfo> ni(n);    // int -> NodeInfo
    vector<cInfo> ci(n); // Cluster -> ClusterInfo
    vector<vector<Node>> c(n); // Cluster -> List of Nodes

    unordered_map<Node, int> tabuNodes; // tabu list for nodes
    unordered_map<Cluster, int> tabuClusters; // tabu list for clusters

    auto decrCost = [](const nInfo &a, const nInfo &b) {return a.c > b.c; };
    auto indexIsNode = [](const nInfo &a, const nInfo &b) { return a.u < b.u; };
    auto decrSpecificCost = [](const cInfo &a, const cInfo &b){ return a.sc > b.sc; };
    auto indexIsCluster = [](const cInfo &a, const cInfo &b) { return a.cid < b.cid; };
    Cost minCost;

    // starting point
    if(m < 15000){
      // all isolated
      for (Node u = 0; u < n; u++){
        ncid[u] = u;
        ni[u] = {u, adj[u].size()};
        ci[u] = {u, 1, {0, adj[u].size()}};
        c[u].push_back(u);
      }
    }else{
      // starting point from FORCE D&C on n0xn0 diagonal
      for (Cluster i = 0; i < n; i++)
        ci[i] = {i, 0, {0, 0}};
      for (Node u = 0; u < n; u++)
        ni[u] = {u, 0};
      int h = 0;
      Graph G1(n0, vector<bool>(n0, false));
      for (Node u0 = 0; u0 < n; u0 += n0)
      {
        int n1 = min(n0, n - u0);
        for (Node u = u0; u < u0 + n1; u++)
        {
          for (Node v = u0; v < u; v++){
            G1[u - u0][v - u0] = G[u][v]; // loading sub-problem
            G1[v - u0][u - u0] = G[v][u];
          }
        }
        // sub-problem solution
        // Layout
        vector<Point> pos = layout(G1, n1, rho, fatt, frep, M0, r, R);

        // Clustering
        vector<vector<Node>> ccs = clustering(G1, n1, pos, delta_init, delta_max, sigma_init, alpha);
        // Assign cid
        for (auto&cc : ccs)
        { // INV: ccs[i].size() > 0
          ci[h].s = cc.size();
          for (Node u1 : cc)
          {
            ncid[u0+u1] = h;
            c[h].push_back(u0 + u1); // need to add u0 to all elements
          }
          h++;
        }
      }
      cout << h << endl;
      // Assign costs
      for (Node u = 0; u < n; u++)
      {
        for (Node v = 0; v < u; v++)
        { // only lower half for costs!
          if (ncid[u] == ncid[v] && !G[u][v])
          {
            // add
            ni[u].c++;
            ni[v].c++;
            minCost.adds++;
            ci[ncid[u]].c.adds++;
          }
          if (ncid[u] != ncid[v] && G[u][v])
          {
            // rem
            ni[u].c++;
            ni[v].c++;
            minCost.rems++;
            ci[ncid[u]].c.rems++;
            ci[ncid[v]].c.rems++;
          }
        }
      }
      cout << minCost.adds << ' ' << minCost.rems << endl;
      // compute SC after all subproblems
      for (Cluster i = 0; i < h; i++)
        ci[i].updateSC();
    }

    // hill climbing
    int emptyTries = 0;
    bool nowMerge = true;
    for (int j = 0; j < CLIMBS; j++) // CLIMB
    {
      // O(n*maxdeg+nlg n)
      for (int i = 0; i < 100; i++)
      {
        
        // Toggle between moving nodes and merging clusters
        if (emptyTries > maxEmptyTries)
        {
          //cout << "toggling" << endl;
          emptyTries = 0;
          nowMerge = !nowMerge;
          if (!nowMerge) // INV: ci is "ordered" when we move nodes
            sort(ci.begin(), ci.end(), indexIsCluster);
        }
        // update Clusters tabu lists
        // O(n) Clusters
        for (auto &pair : tabuClusters)
          pair.second--;
        for (auto it = tabuClusters.begin(); it != tabuClusters.end();)
        {
          if (it->second < 0)
            it = tabuClusters.erase(it);
          else
            it++;
        }

        /// Merging a cluster
        if(nowMerge){
          //merges++;
          // cout << i << "-nowMerge" << endl;
          //  O(nlg n)
          //  INV ni is "ordered", ci is "sorted"
          // apply stronger heuristic only after a while
          if(j*I >= n/5)
            std::sort(ci.begin(), ci.end(), decrSpecificCost);

          // pick maximum specific cost cluster not in tabu nor empty
          auto a_ci = find_if(ci.begin(), ci.end(), [&](cInfo& x)
                              { return x.s != 0 && tabuClusters.find(x.cid) == tabuClusters.end(); });
          if(a_ci == ci.end()){
            emptyTries = maxEmptyTries + 1;
            continue;
          }
          Cluster a = a_ci->cid;
          int sa = a_ci->s;
          Cluster b = a;
          // explore neighborhood O(n*s)
          // (merging selected cluster to another cluster)
          int minDelta = INFINITY;
          for (Cluster x = 0; x < n; x++)
          {
            int delta = 0;
            // compute delta
            int oldrems = 0;
            // find x (ci is currently "unordered")
            // such x exists
            auto x_ci = find_if(ci.begin(), ci.end(), [=](cInfo &y)
                 { return y.cid == x; });
            int sx = x_ci->s;
            if (x == a || sx == 0)
              continue;

            bool swapped = false;
            if (sa < sx){
              swapped = true;
              swap(a, x);
              swap(sa, sx);
              swap(a_ci, x_ci);
            }
            // INV:  A is larger than X
            for (Node u : c[x]) // X is smaller, we explore from X
            {
              for (Node v : adj[u]) // uv exists
              {
                if (ncid[v] == a)
                {
                  // add back edge
                  oldrems++;    // that was removed before
                  ni[u].c -= 2; // will add sb to all in c[a] after
                  ni[v].c -= 2; // will add sa to all in c[b] after
                }
              }
            }
            int newadds = sa * sx - oldrems;
            delta = newadds - oldrems;
            if (swapped)
            {
              swap(a, x);
              swap(sa, sx);
              swap(a_ci, x_ci);
            }
            if (delta < minDelta)
            {
              b = x;
              minDelta = delta;
            }
          }
          if (minDelta >= 0 && !(dis(gen) < exp(-minDelta / e_kin)))
          {
            tabuClusters.insert({a, tabuPauseClusters});
            emptyTries++;
            continue; // skips and increments
          }
          // INV: a != b and sa != 0 != sb
          // smaller is merged into larger;
          // find b (ci is currently "unordered")
          auto b_ci = find_if(ci.begin(), ci.end(), [=](cInfo &y)
                             { return y.cid == b; });
          int sb = b_ci->s;
          if(sa < sb){
            swap(a, b);
            swap(sa, sb);
            swap(a_ci, b_ci);
          }
          // INV: sa >= sb
          tabuNodes.insert({a, max((double) tabuPauseClusters, (double) tabuPauseClusters / log(e - minDelta))});
          // update data structures
          // - ni
          // - ci
          // - c
          // - ncid
          // - ni
          for (Node v : c[a]) // new edges
            ni[v].c += sb;
          for (Node v : c[b])
            ni[v].c += sa;
          // O(n+m)
          int oldrems = 0;
          for (Node u : c[b]) // B is smaller, we explore B
          {
            for (Node v : adj[u]) // uv exists
            {
              if (ncid[v] == a)
              {
                // add back edge
                oldrems++;    // that was removed before
                ni[u].c -= 2; // added sb to all in c[a] before
                ni[v].c -= 2; // added sa to all in c[b] before
              }
            }
          }
          int newadds = sa * sb - oldrems;
          // - ci
          a_ci->s += sb;
          b_ci->s = 0;

          a_ci->c.adds += newadds;
          a_ci->c.rems -= oldrems;
          a_ci->updateSC();
          b_ci->c.adds = 0;
          b_ci->c.rems = 0;
          b_ci->updateSC();
          // - c - ncid
          for (Node v : c[b]){ // B is smaller; it is eaten by A
            c[a].push_back(v);
            ncid[v] = a;
          }
          c[b].clear();

          /*if (newadds < 0)
          {
            cout << "a=" << a << " b=" << b << endl;
            cout << "sa=" << sa << " sb=" << sb << " newadds=" << newadds << " oldrems=" << oldrems << endl;
          }*/

          // sum of sum of nodes cost != sum of cluster costs != cost
          // minCost
          minCost.adds += newadds;
          minCost.rems -= oldrems;

          e_kin *= t_alpha;
        }
        else
        { /// Moving a node
          //moves++;
          // cout << i << "-movingNode" << endl;
          int minDelta = INFINITY;
          // O(nlg n)
          // INV: ni is "sorted", ci is "ordered"
          std::sort(ni.begin(), ni.end(), decrCost);
          
          // pick maximum cost node not in tabu
          auto u_ni = find_if(ni.begin(), ni.end(), [&](nInfo &x)
                           { return tabuNodes.find(x.u) == tabuNodes.end(); });
          if (u_ni == ni.end())
          {
            emptyTries = maxEmptyTries + 1;
            continue;
          }
          Node u = u_ni->u;
          Cluster old_cid = ncid[u];
          Cluster new_cid = old_cid;
          int so = ci[old_cid].s; // size old, before removing node
          /*if(so == 0){
            cout << "the plot thickens..." << endl;
            if(ci[old_cid].cid != old_cid){
              cout << "up to the grand finale...where " << old_cid << " fights against " << ci[old_cid].cid << endl;
            }
          }*/

          // explore neighborhood O(n*maxdeg)
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
              if (ncid[v] == cid)
              {
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
            delta = +newrems - oldadds + newadds - oldrems;
            if (delta < minDelta)
            {
              new_cid = cid;
              minDelta = delta;
            }
          }
          if (minDelta >= 0)
          {
            tabuNodes.insert({u, tabuPauseNodes});
            emptyTries++;
            tabuPauseNodes += tabuPauseNodes_delta;
            continue; // skips and increments
          }else{
            tabuNodes.insert({u, tabuPauseNodes / log(e - minDelta)});
          }
          // update data structures
          // - ni
          // - ci
          // - c
          // - ncid

          // - c
          c[old_cid].erase(find(c[old_cid].begin(), c[old_cid].end(), u));
          // - ni (fixed nodes)
          // INV: ni is "ordered" ci is "ordered"
          std::sort(ni.begin(), ni.end(), indexIsNode);
          for (Node v : c[new_cid])
            ni[v].c += 1;
          for (Node v : c[old_cid])
            ni[v].c -= 1;
          // - c - ncid
          c[new_cid].push_back(u);
          ncid[u] = new_cid;

          // - ci
          auto &new_ci = ci[new_cid];
          auto &old_ci = ci[old_cid];
          int sn = new_ci.s; // before adding node
          new_ci.s++;
          old_ci.s--;
          /*if(old_ci.s == -1)
            cout << "someone did something..." << endl;*/

          // recalculate oldrems, newrems (might simply save them)
          int oldrems = 0;
          int newrems = 0;
          // O(n)
          for (Node v : adj[u]) // uv exists
          {
            if (ncid[v] == new_cid)
            {
              // add back edge
              oldrems++;    // that was removed before
              ni[v].c -= 2; // will add 1 to all in c[new_cid] after
            }
            if (ncid[v] == old_cid)
            {
              // remove edge
              newrems++;    // that was not removed before
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

          // sum of sum of nodes cost != sum of cluster costs != cost
          // minCost
          minCost.adds += newadds - oldadds;
          minCost.rems += newrems - oldrems;

          // update tabu list nodes
          // O(n) Nodes
          for (auto &pair : tabuNodes)
            pair.second--;
          for (auto it = tabuNodes.begin(); it != tabuNodes.end();)
          {
            if (it->second < 0)
              it = tabuNodes.erase(it);
            else
              it++;
          }
          // consider using a different data structure for ci
          // here you should remove old_ci if it is empty
        }
        //rem_time = end_time - chrono::high_resolution_clock::now();
        //if (rem_time <= safe_print_time){
        //  // cout << safe_print_time.count() << endl;
        //  // cout << "print now!" << endl;
        //  break;
        //}
      }
      //if(firstPrint || rem_time <= safe_print_time){
      //  prints++;
      //  firstPrint = false;
      //  auto before = std::chrono::high_resolution_clock::now();
        // print solution
        out << minCost.adds << ' ' << minCost.rems << endl;
        for (Node u = 0; u < n; u++)
        {
          for (Node v = 0; v < u; v++)
          {
            if (ncid[u] == ncid[v] && !G[u][v])
              out << "+ " << u << ' ' << v << endl;
            else if (ncid[u] != ncid[v] && G[u][v])
              out << "- " << u << ' ' << v << endl;
          }
        }
        out << "***" << endl;
        //cout << prints << endl;
        //auto after = std::chrono::high_resolution_clock::now();
        //safe_print_time = std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(5./4. * (after - before));
        //if(!firstPrint)
        //  end_time += std::chrono::milliseconds(10);
      //}
      //cout << "merges=" << merges << " moves=" << moves << endl;
      // force a toggle for every climb
      emptyTries = maxEmptyTries + 1;
    }
  }
  else
  { // O(Rn^2+kn^2)
  FORCE:
    // FORCE Layout
    const int R = 50;
    const double rho = n * 10 / (6.28);
    const double fatt = 1.245;
    const double frep = 1.687;
    const double M0 = 633;
    const double r = 0.99;
    // FORCE Clustering
    const double delta_init = 0;
    const double delta_max = 200;
    const double sigma_init = 0.01;
    const double alpha = 1.1 + 0.0000001 * n;

    for (int i = 0; i < m; i++)
    {
      int u, v;
      in >> u >> v;
      G[u][v] = true;
      G[v][u] = true;
    }

    out << "0 0" << endl; // componente non cricca
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