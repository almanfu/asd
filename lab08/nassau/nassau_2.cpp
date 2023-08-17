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
nassau_2 combinatorial analysis
*/

// la probabilità di colpire vascello, vascellino o fregata NON è uniforme ma dipende da quanti ci sono

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

unordered_map<Key, int> DP;
int V, F, M;

// INV: V1, v1, F1, M1 >= 0
// M1:= remaining shots != used shots
int mDP (int V1, int v1, int F1, int M1){
  // Out of bounds
  if (V1 > V || v1 > V || F1 > F || M1 > M)
  {
    return 0;
  }
  // base case
  else if(V1==V && v1==0 && F1 == F && M1 == M){
    return 1;
  }
  // memoized
  else if (DP.find({V1, v1, F1, M1}) != DP.end())
  {
    return DP.at({V1, v1, F1, M1});
  }
  // not memoized
  else//INV: V1,v1,F1 >= 0
  {
    int res = (V1 + 1) * mDP(V1 + 1, v1 - 1, F1, M1 + 1) + (v1 + 1) * mDP(V1, v1 + 1, F1, M1 + 1) + (F1 + 1) * mDP(V1, v1, F1 + 1, M1 + 1);
    DP.insert({{V1, v1, F1, M1}, res});
    return res;
  }
}

int main(int argc, char *argv[]){
  ifstream in("input.txt");
  ofstream out("output.txt");

  in >> V >> F >> M;

  int tot = 0.;
  for (int f = 0; f <= F; f++){
    for (int d = 0; d <= V; d++){
      for (int v = 0; v <= V - d; v++){
        if(mDP(V - d - v, v, F - f, 0) != 0){
          cout << (V - d - v) << ' ' << v << ' ' << (F - f) << '=' << mDP(V - d - v, v, F - f, 0) << endl;
          tot += mDP(V - d - v, v, F - f, 0);
        }
      }
    }
  }

  long double res = 0.;
  for (int f = 0; f <= F; f++){
    for (int d = 0; d <= V; d++){
      for (int v = 0; v <= V - d; v++){
        if ((V - d) * (F - f) != 0)
          res += (long double) mDP(V - d - v, v, F - f, 0) * (long double)((V - d) * (F - f));
      }
    }
  }
  cout << res << ' ' << tot << endl;

  out << scientific << setprecision(10)
      << (res/tot) << endl;

  in.close();
  out.close();
  return 0;
}
