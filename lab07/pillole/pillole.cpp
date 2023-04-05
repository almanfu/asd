#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <stack>

using namespace std;

/*

*/

long long int pillole(int n){
  vector<vector<long long int>> DP(n+1, vector<long long int>(n+1));
  for(int i=0; i <= n; i++){
    for(int m=0; m <= n-i; m++){
      if(i==0)
        DP[i][m]=1;
      else if(m != 0)
        DP[i][m]=DP[i-1][m+1]+DP[i][m-1];
      else
        DP[i][m]=DP[i-1][m+1];      
    }
  }
  return DP[n][0];
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
