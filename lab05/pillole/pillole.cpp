#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <stack>

using namespace std;

/*

*/

int fun(vector<vector<int>> DP, int i, int m){
  if(i==0)
    return 1;
  else if(DP[i][m] != INT32_MIN)
    return DP[i][m];
  else if(m != 0)
    return fun(DP, i-1, m+1)+fun(DP, i, m-1);
  else
    return fun(DP, i-1, m+1);
}

int pillole(int n){
  vector<vector<int>> DP(n+1, vector<int>(n+1, INT32_MIN));
  return fun(DP, n, 0);
}

int main(int argc, char *argv[]){
  ifstream in("input.txt");
  ofstream out("output.txt");
  int n;
  in >> n;

  out << pillole(n);

  in.close();
  out.close();
  return 0;
}
