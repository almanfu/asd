#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <unordered_map>
#include <iomanip>


using namespace std;
/*
nassau_1 dynamic programming + branch & bound
*/

// la probabilità di colpire vascello, vascellino o fregata NON è uniforme ma dipende da quanti ci sono

struct Key
{
  int V1, v1, F1;

  bool operator==(const Key &other) const
  {
    return V1 == other.V1 && v1 == other.v1 && F1 == other.F1;
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
      return ((hash<int>()(k.V1) ^ (hash<int>()(k.v1) << 1)) >> 1) ^ (hash<int>()(k.F1) << 1);
    }
  };
}

unordered_map<Key, long double> DP;

// int tot = 0;
// int memo = 0;
// int usedmemo = 0;
//  mDP := expected power for problem with parameters
long double mDP(int V1, int v1, int F1, int M1, long double f)
{
  // tot++;
  // int M1 = M - 2*(V - V1) +v1 - (F - F1);
  //  out of bounds
  if (V1 < 0 || v1 < 0 || F1 < 0)
  {
    return 0;
  }
  // Base cases
  else if (M1 == 0)
  {
    return (long double)((V1 + v1) * F1);
  }
  else if ((V1 + v1) == 0 || F1 == 0)
  {
    return 0;
  }
  // INV: V1,v1,F1,M1 >= 0
  // memoized
  else if (DP.find({V1, v1, F1}) != DP.end())
  {
    // usedmemo++;
    return DP.at({V1, v1, F1});
  }
  // not memoized
  else
  {
    // memo++;
    long double res = 0;
    // bounding
    if (f <= 3e-57)
      res = 0;
    else
    {
      long double d = V1 + v1 + F1;
      res = (V1 / d) * mDP(V1 - 1, v1 + 1, F1, M1 - 1, f * (V1 / d)) + (v1 / d) * mDP(V1, v1 - 1, F1, M1 - 1, f * (v1 / d)) + (F1 / d) * mDP(V1, v1, F1 - 1, M1 - 1, f * (F1 / d));
    }
    DP.insert({{V1, v1, F1}, res});
    return res;
  }
}

int main(int argc, char *argv[])
{
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