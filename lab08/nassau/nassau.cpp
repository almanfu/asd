#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <unordered_map>
#include <iomanip>


using namespace std;

struct Key
{
  int V, v, F, M;

  bool operator==(const Key &other) const
  {
    return V == other.V && v == other.v && F == other.F && M == other.M;
  }
};

// Hash function
namespace std
{
  template <>
  struct hash<Key>
  {
    std::size_t operator()(const Key &k) const
    {
      return ((hash<int>()(k.V) ^ (hash<int>()(k.v) << 1)) >> 1) ^ (hash<int>()(k.F) << 1) ^ (hash<int>()(k.M) << 2);
    }
  };
}

unordered_map<Key, long double> DP;

long double mDP (int V, int v, int F, int M, long double f){
  // out of bounds
  if (min(V, 0) + min(v, 0) + min(F, 0) < 0)
  {
    return 0;
  }
  // Base cases
  if (M == 0)
  {
    return f*(long double)((V + v) * F);
  }
  else if ((V + v) == 0 || F == 0)
  {
    return 0;
  }
  // memoized
  if (DP.find({V, v, F, M}) != DP.end())
  {
    return DP.at({V, v, F, M});
  }
  // not memoized
  else//INV: V,v,F >= 0
  {
    long double res=0;
    // negligible (will get here after 27 rounds at most)
    if (f*(long double)((V + v) * F) == 0.)
    {
      res = f*(long double)((V + v) * F);
    }
    else{
      long double d = V + v + F;
      res = mDP(V - 1, v + 1, F, M - 1, f * (V / d)) + mDP(V, v - 1, F, M - 1, f * (v / d)) + mDP(V, v, F - 1, M - 1, f * (F / d));
    }
    DP.insert({{V, v, F, M}, res});
    return res;
  }
}

int main(int argc, char *argv[]){
  ifstream in("input.txt");
  ofstream out("output.txt");

  int V, F, M;

  in >> V >> F >> M;

  long double res = mDP(V, 0, F, M, 1);

  out << scientific << setprecision(10)
      << res << endl;

  in.close();
  out.close();
  return 0;
}
