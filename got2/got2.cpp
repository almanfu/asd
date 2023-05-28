#include "got2.h"
#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <cmath>

using namespace std;

bool INFO = true;

typedef matrix matrix;

/*

*/

/// Layouting
// Define a 2D Point structure.
struct Point
{
  double x, y;
};

double distance(Point a, Point b)
{
  double dx = a.x - b.x;
  double dy = a.y - b.y;
  return sqrt(dx * dx + dy * dy);
}

vector<Point> graph_layouting(matrix &M, double rho, double fatt, double frep, int R)
{
  int n = M.size();
  vector<Point> pos(n);

  // Arrange nodes in a circle.
  for (int i = 0; i < n; ++i){
    pos[i] = {rho * cos(2 * M_PI * i / n), rho * sin(2 * M_PI * i / n)};
  }

  for (int r = 0; r < R; ++r)
  {
    vector<Point> delta(n, {0, 0});

    for (int i = 0; i < n; ++i)
    {
      for (int j = 0; j < i; ++j)
      {
        if (M[i][j] > 0)
        { // If nodes are connected.
          double d = distance(pos[i], pos[j]);
          Point f = {(log(d + 1) * fatt) * (pos[j].x - pos[i].x) / d, (log(d + 1) * fatt) * (pos[j].y - pos[i].y) / d};

          delta[i].x += f.x;
          delta[i].y += f.y;
          delta[j].x -= f.x;
          delta[j].y -= f.y;
        }
      }
    }

    // Apply the displacements (capped to a maximum of 1).
    for (int i = 0; i < n; ++i)
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

///Partitioning

matrix_cost_pair partitionGraph(const matrix &M, const vector<Point> &layout,
                                double delta_init, double delta_max, double sigma_init, double sigma_factor)
{
  int n = M.size();
  double delta = delta_init;
  double sigma = sigma_init;
  double c_star = INFINITY;
  matrix E_star(M.size(), vector<int>(M.size(), 0));

  while (delta <= delta_max)
  {
    matrix E_delta(M.size(), vector<int>(M.size(), 0));

    // Construct auxiliary graph G_delta
    for (int i = 0; i < layout.size(); ++i)
    {
      for (int j = i + 1; j < layout.size(); ++j)
      {
        if (distance(layout[i], layout[j]) <= delta)
        {
          E_delta[i][j] = E_delta[j][i] = 1;
        }
      }
    }

    // Compute transitively closed adjacency matrix E_prime from E_delta
    // Note: In the real-world implementation, we should use a proper algorithm
    // for transitive closure such as Floyd-Warshall algorithm or DFS for each node.
    matrix E_prime = E_delta;

    double curr_cost = cost(E_prime);
    if (curr_cost < c_star)
    {
      E_star = E_prime;
      c_star = curr_cost;
    }

    sigma = sigma * sigma_factor;
    delta = delta + sigma;
  }

  return make_pair(E_star, c_star);
}


/// Main 
int main()
{
  ifstream in("input.txt");
  ofstream out("output.txt");



  int n, m;
  in >> n >> m;
  matrix M(n, vector<short int>(n, 0));

  for (int i = 0; i < m; i++)
  {
    int u, v;
    in >> u >> v;
    M[u][v] = 1;
  }

  const int R = 100;
  const int rho = n/10;
  const int fatt = 1;
  const int frep = 1;

  // Graph Layouting
  vector<Point> layout = graph_layouting(M, rho, fatt, frep, R);

  // Partitioning

  in.close();
  out.close();
  return 0;
}