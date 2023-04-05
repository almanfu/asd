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

int fun(vector<int> A, vector<vector<int>> DP, int i, int l){
  if(i==1){
    if(l==1)
      return A[l];
    else
      return 0;
  }else if(DP[i][l] != INT32_MIN){
    return DP[i][l];
  }else{
    int res = A[l];
    for(int pl=1; pl < l; pl++){
      if(A[pl] <= A[l])
        res = max(res, fun(A, DP, i-1, pl)+A[l]);
    }
    DP[i][l] = res;
    return DP[i][l];
  }
}

int sottocres(vector<int> A, int n){
  vector<vector<int>> DP(n+1, vector<int>(n+1, INT32_MIN));
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