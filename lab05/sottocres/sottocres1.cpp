#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <stack>

using namespace std;

/*

*/

int fun(vector<int>& A, vector<int>& DP, int l){
  if(l==1){
    return A[l];
  }else if(DP[l] != INT32_MIN){
    return DP[l];
  }else{
    int res = A[l];
    for(int pl=1; pl < l; pl++){
      if(A[pl] <= A[l])
        res = max(res, fun(A, DP, pl)+A[l]);
    }
    DP[l] = res;
    return DP[l];
  }
}

int sottocres(vector<int> A, int n){
  vector<int> DP(n+1, INT32_MIN);
  int res = INT32_MIN;
  for(int l=1; l <= n; l++)
    res = max(res, fun(A, DP, l));
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