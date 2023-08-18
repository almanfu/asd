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
nassau_2 probability on possible end states
*/

// la probabilità di colpire vascello, vascellino o fregata NON è uniforme ma dipende da quanti ci sono

struct Key
{
  int V, v, F;

  bool operator==(const Key &other) const
  {
    return V == other.V && v == other.v && F == other.F;
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
      return ((hash<int>()(k.V) ^ (hash<int>()(k.v) << 1)) >> 1) ^ (hash<int>()(k.F) << 1);
    }
  };
}

unordered_map<Key, long double> DP;
int V, F, M;

// INV: V1, v1, F1, M1 >= 0
// M1:= remaining shots != used shots
// mDP := probability of getting to the defined end state
long double mDP (int V1, int v1, int F1, int M1){
  // Out of bounds
  if (V1 > V || (V1+v1) > V || F1 > F || M1 > M || v1 < 0 )
  {
    return 0;
  }
  // base case
  else if(V1==V && v1==0 && F1 == F && M1 == M){
    //cout << V1 << ' ' << v1 << ' ' << F1 << endl;
    return 1;
  }
  // memoized
  else if (DP.find({V1, v1, F1}) != DP.end())
  {
    return DP.at({V1, v1, F1});
  }
  // not memoized
  else //INV: V1,v1,F1 >= 0
  {

    long double res =
        ((long double)(V1 + 1) / (long double)(V1 + v1 + F1)) * mDP(V1 + 1, v1 - 1, F1, M1 + 1) +
        ((long double)(v1 + 1) / (long double)(V1 + v1 + 1 + F1)) * mDP(V1, v1 + 1, F1, M1 + 1) +
        ((long double)(F1 + 1) / (long double)(V1 + v1 + F1 + 1)) * mDP(V1, v1, F1 + 1, M1 + 1);
    DP.insert({{V1, v1, F1}, res});
    return res;
  }
}

int main(int argc, char *argv[]){
  ifstream in("input.txt");
  ofstream out("output.txt");

  in >> V >> F >> M;

  long double res = 0.;
  // O(5000^2)

  for (int d1 = 0; d1 <= V; d1++)
  {
    for (int v1 = max(M - F - 2 * d1, 0); v1 <= min(M - 2 * d1, V - d1); v1++)
    {
      int f1 = M - 2 * d1 - v1;
      //cout << V - v1 - d1 << ' ' << v1 << ' ' << F - f1 << ' ';
      if (((V - d1) * (F - f1)) != 0){
        //cout << mDP(V - v1 - d1, v1, F - f1, 0) << endl;
        res += mDP(V - v1 - d1, v1, F - f1, 0) * (long double)((V - d1) * (F - f1));
      }
    }
  }

  out << scientific << setprecision(10)
      << res << endl;

  in.close();
  out.close();
  return 0;
}
