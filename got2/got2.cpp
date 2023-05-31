#include "got2.h"
#include <iostream>
#include <sstream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <cmath>

using namespace std;

bool INFO = true;

typedef vector<vector<short int>> matrix;
typedef int Node;

/*

*/

/// Layout
// Define a 2D Point structure.
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
}

vector<Point> layout(matrix &M, double rho, double fatt, double frep, int R)
{
  int n = M.size();
  vector<Point> pos(n);

  // Arrange nodes in a circle.
  for (int i = 0; i < n; i++)
    pos[i] = {rho * cos(2 * M_PI * i / n), rho * sin(2 * M_PI * i / n)};

  for (int r = 0; r < R; r++)
  {
    info(pos, r);
    // Compute displacements Delta for iteration r
    vector<Point> delta(n, {0, 0});
    for (Node u = 0; u < n; u++)
    {
      for (Node v = 0; v < u; v++)
      {
        double d = distance(pos[u], pos[v]);
        short strength = (log(d + 1) * fatt);
        if (M[u][v] == 0) // Repulsive Force
          strength = -strength;
        Point f_v2u = {strength*(pos[v].x-pos[u].x)/d, strength*(pos[v].y-pos[u].y)/d};

        // Action
        delta[u].x += f_v2u.x;
        delta[u].y += f_v2u.y;
        
        // Reaction
        delta[v].x -= f_v2u.x;
        delta[v].y -= f_v2u.y;
      }
    }

    // Apply the displacements (capped to a maximum of 1).
    for (int i = 0; i < n; i++)
    {
      double d = distance({0, 0}, delta[i]);
      if (d > 1)
      {
        delta[i].x /= d;
        delta[i].y /= d;
      }
      pos[i].x += delta[i].x;
      pos[i].y += delta[i].y;
    }
  }

  return pos;
}

// A helper function for the depth-first search
void dfs(int v, const std::vector<std::vector<short int>> &E_delta, std::vector<int> &component, std::vector<bool> &visited)
{
  visited[v] = true;
  component.push_back(v);

  for (int u = 0; u < E_delta.size(); u++)
  {
    if (E_delta[v][u] && !visited[u])
    {
      dfs(u, E_delta, component, visited);
    }
  }
}

// Detect connected components of G_delta
vector<vector<int>> connected_components(const vector<vector<short int>> &E_delta)
{
  vector<bool> visited(E_delta.size(), false);
  vector<vector<int>> components;

  for (int v = 0; v < E_delta.size(); v++)
  {
    if (!visited[v])
    {
      vector<int> component;
      dfs(v, E_delta, component, visited);
      components.push_back(component);
    }
  }

  return components;
}

// Compute transitively closed adjacency matrix E_prime from E_delta using the Floyd-Warshall algorithm
matrix transitive_closure(const matrix &E_delta)
{
  matrix E_prime = E_delta;

  int n = E_delta.size();
  for (int k = 0; k < n; k++){
    for (int i = 0; i < n; i++){
      for (int j = 0; j < n; j++)
        E_prime[i][j] = E_prime[i][j] || (E_prime[i][k] && E_prime[k][j]);
    }
  }

  return E_prime;
}

double cost(const matrix &M, const matrix &E_prime)
{
  double total_cost = 0.0;
  for (int i = 0; i < M.size(); i++)
  {
    for (int j = 0; j < M[i].size(); j++)
    {
      if (M[i][j] != E_prime[i][j])
      {
        total_cost += 1.0; // The cost of adding or removing an edge is considered as 1.
      }
    }
  }
  return total_cost;
}

///Partition
void partition(const matrix &M, const vector<Point> &pos,
                                double delta_init, double delta_max, double sigma_init, double sigma_factor)
{
  ofstream out("output.txt");
  int n = M.size();
  double delta = delta_init;
  double sigma = sigma_init;
  double c_star = INFINITY;
  matrix E_star(n, vector<short int>(n, 0));

  while (delta <= delta_max)
  {
    matrix E_delta(n, vector<short int>(n, 0));

    // Construct auxiliary graph G_delta
    for (Node u=0; u<n; u++)
    {
      for (Node v=u+1; v<n; v++)
      {
        if (distance(pos[u], pos[v]) <= delta)
          E_delta[u][v] = E_delta[v][u] = 1;
      }
    }

    matrix E_prime = transitive_closure(E_delta);

    double c = cost(M, E_prime);
    if (c < c_star)
    {
      E_star = E_prime;
      c_star = c;
    }

    sigma *= sigma_factor;
    delta += sigma;
  }
  out << c_star;
  out.close();
}


/// Main 
int main()
{
  ifstream in("input.txt");

  int n, m;
  in >> n >> m;
  matrix M(n, vector<short int>(n, 0));

  for (int i = 0; i < m; i++)
  {
    int u, v;
    in >> u >> v;
    M[u][v] = 1;
  }

  //
  const int R=200;
  const double rho=n*10/(6.28);
  const double fatt=10*n;
  const double frep=7*n;
  //
  const double delta_init=1;
  const double delta_max=100;
  const double sigma_init=delta_init/10;
  const double sigma_factor=1.01+0.0001*n;

  // Layout
  vector<Point>
      pos = layout(M, rho, fatt, frep, R);

  // Partition
  partition(M, pos, delta_init, delta_max, sigma_init, sigma_factor);

  in.close();

  return 0;
}