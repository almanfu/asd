#include <vector>

using namespace std;

/*

*/

class MF
{
  vector<int> f;
  vector<int> c;

public:
  MF(int n) : f(n),
              c(n, 1)
  {
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
  void decrSize(int x)
  {
    c[find(x)]--;
  }
  int getSize(int x)
  {
    return c[find(x)];
  }
};