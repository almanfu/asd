#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <stack>

using namespace std;

/*

*/

int knapsack(vector<int> W, vector<int> V, int N, int C){
  vector<vector<int>> DP(N+1, vector<int>(C+1, 0));
  for(int n=0; n <= N; n++){
    for(int c=0; c <= C; c++){
      if(n==0 || c==0)
        DP[n][c] = 0;
      else
        DP[n][c] = max(DP[n-1][c], c>=W[n] ? DP[n-1][c-W[n]]+V[n] : 0);
    }
  }
  return DP[N][C];
}


int main(int argc, char *argv[]){
  ifstream in("input.txt");
  ofstream out("output.txt");
  int C, N;

  in >> C >> N;

  vector<int> W(N+1);
  vector<int> V(N+1);

  for(int i=1; i<=N; i++)
    in >> W[i] >> V[i];

  out << knapsack(W, V, N, C);

  in.close();
  out.close();
  return 0;
}
