#include <iostream>
#include <sstream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <cmath>

#include "got2.h"

using namespace std;

bool INFO = false;

typedef vector<vector<bool>> Graph;
typedef int Node;

/*
FORCE Heuristic
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

vector<Point> layout(Graph &G, int n, double rho, double fatt, double frep, int R)
{
  vector<Point> pos(n);

  // Arrange nodes in a circle.
  // O(n)
  for (int i = 0; i < n; i++)
    pos[i] = {rho * cos(2 * M_PI * i / n), rho * sin(2 * M_PI * i / n)};

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
        short strength = (log(d + 1) * fatt);

        // Force is attractive if there is an edge
        // Repulsive if there is no edge
        if (!G[u][v])
          strength = -strength;
        // Force-Vector
        Point f_v2u = {strength*(pos[v].x-pos[u].x)/d, strength*(pos[v].y-pos[u].y)/d};

        // Action
        delta[u].x += f_v2u.x;
        delta[u].y += f_v2u.y;
        
        // Reaction
        delta[v].x -= f_v2u.x;
        delta[v].y -= f_v2u.y;
      }
    }

    // Apply the displacements (capped to a maximum of 1)
    // O(n)
    for (Node u = 0; u < n; u++)
    {
      double d = distance({0, 0}, delta[u]);
      if (d > 1)
      {
        delta[u].x /= d;
        delta[u].y /= d;
      }
      pos[u].x += delta[u].x;
      pos[u].y += delta[u].y;
    }
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

// ccs[j][i] = i-th element of the j-th cc
vector<vector<int>> get_ccs(const Graph &G, int n)
{
  vector<bool> visited(n, false);
  vector<vector<int>> ccs;

  for (Node u = 0; u < n; u++)
  {
    if (!visited[u])
    {
      vector<int> cc;
      ccsdfs(u, G, n, cc, visited);
      ccs.push_back(cc);
    }
  }

  return ccs;
}

Graph transitive_closure(const Graph &G, int n)
{
  Graph Gc(n, vector<bool>(n, false));

  // Get connected components
  auto ccs = get_ccs(G, n);

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

// Cost of a partition
int cost(const Graph &G, const Graph &Gc)
{
  int tot = 0;
  for (Node u = 0; u < G.size(); u++)
  {
    for (Node v = 0; v < u; v++)
    {
      if (G[u][v] != Gc[u][v])
        tot++;
    }
  }
  return tot;
}

// Clustering
void clustering(const Graph &G, int n, const vector<Point> &pos,
               double delta_init, double delta_max, double sigma_init, double alpha, ofstream& out)
{
  double delta = delta_init;
  double sigma = sigma_init;
  int costMin = INFINITY;
  Graph G_min (n, vector<bool>(n, false));

  // O(k n^3)
  while (delta <= delta_max)
  {
    Graph G_delta(n, vector<bool>(n, false));

    // Construct auxiliary graph G_delta
    // O(n^2)
    for (Node u=0; u<n; u++)
    {
      for (Node v=u+1; v<n; v++)
      {
        if (distance(pos[u], pos[v]) <= delta)
          G_delta[u][v] = G_delta[v][u] = true;
      }
    }
    
    //O(n^3)
    Graph Gc_delta = transitive_closure(G_delta, n);
    
    //O(n^2)
    double costHere = cost(G, Gc_delta);
    if (costHere < costMin)
    {
      G_min = Gc_delta;
      costMin = costHere;
    }

    sigma *= alpha;
    delta += sigma;
  }

  // O(n^2)
  // Count Additions/Removals
  int adds = 0, rems = 0;
  for (Node u = 0; u < n; u++)
  {
    for (Node v = 0; v < u; v++)
    {
      if (!G[u][v] && G_min[u][v])
        adds++;
      if (G[u][v] && !G_min[u][v])
        rems++;
    }
  }
  out << adds << ' ' << rems << endl;
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


int main()
{
  ifstream in("input.txt");
  ofstream out("output.txt");

  int n, m;
  in >> n >> m;
  Graph G(n, vector<bool>(n, false));

  for (int i = 0; i < m; i++)
  {
    int u, v;
    in >> u >> v;
    G[u][v] = true;
    G[v][u] = true;
  }

  //
  const int R=20;
  const double rho=n*10/(6.28);
  const double fatt=10*n;
  const double frep=7*n;
  //
  const double delta_init=1;
  const double delta_max=100;
  const double sigma_init=delta_init/10;
  const double alpha=1.01+0.0001*n;

  out << "0 0" << endl;
  out << "***" << endl;

  // Layout
  vector<Point> pos = layout(G, n, rho, fatt, frep, R);

  // Clustering
  clustering(G, n, pos, delta_init, delta_max, sigma_init, alpha, out);

  in.close();
  out.close();

  return 0;
}