#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <stack>

using namespace std;

/*

*/

int main(int argc, char *argv[]){
  ifstream in("input.txt");
  ofstream out("output.txt");

  string t, u;
  in >> t >> u;

  int n = t.length();
  int m = u.length();

  vector<vector<int>> dp = vector<vector<int>>(n+1, vector<int>(m+1, 0));

  for (int i = 1; i <= n; i++){
    for (int j = 1; j <= m; j++){
      dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
      if(t[i-1] ==u[j-1])
        dp[i][j] = max(dp[i][j], dp[i - 1][j - 1]+1);
    }
  }

  out << dp[n][m];

  in.close();
  out.close();
  return 0;
}
