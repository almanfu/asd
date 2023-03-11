#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <unordered_map>

using namespace std;

/*

*/

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2>& p) const {
        return std::hash<T1>()(p.first) ^ std::hash<T2>()(p.second);
    }
};

int fun(vector<int>& A, unordered_map<pair<int, int>, int, pair_hash>& DP, int i, int l){
  if(i==1){
    if(l==1)
      return A[l];
    else
      return 0;
  }else if(DP.find({i, l}) != DP.end()){
    return DP[{i, l}];
  }else{
    int res = A[l];
    for(int pl=1; pl < l; pl++){
      if(A[pl] <= A[l])
        res = max(res, fun(A, DP, i-1, pl)+A[l]);
    }
    DP[{i, l}] = res;
    return DP[{i, l}];
  }
}

int sottocres(vector<int> A, int n){
  unordered_map<pair<int, int>, int, pair_hash> DP;
  int res = INT32_MIN;
  for(int l=1; l <= n; l++)
    res = max(res, fun(A, DP, n, l));
  return res;
}

int main(int argc, char *argv[]){
  ifstream in("input.txt");
  ofstream out("output.txt");
  
  int n;
  in >> n;

  vector<int> A = vector<int>(n+1);

  for(int i=1; i <= n; i++)
    in >> A[i];

  out << sottocres(A, n);

  in.close();
  out.close();
  return 0;
}