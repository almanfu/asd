#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <queue>
#include <stack>

using namespace std;

/*

*/

void print(vector<vector<int>> a){
  for(vector<int> row: a){
    for(int item: row)
      cout << item << " ";
    cout << endl;
  }
}

int max(int a, int b){
  return a > b ? a : b;
}

int main(int argc, char *argv[]){
  ifstream in("input.txt");
  ofstream out("output.txt");

  int r, c;
  in >> r >> c;
  vector<vector<int>> a(r, vector<int>(c));
  for(int i=0; i < r; i++){
    for(int j=0; j < c; j++)
      in >> a[i][j];
  }
  /*O(n^4)
  int maxSoFar = 0;
  // For each cell
  for(int i=0; i < r; i++){
    for(int j=0; j < c; j++){
      vector<int> colSum(c,0);
      // Find all possible sub-matrix      
      for(int i1=i; i1 < r; i1++){
        int sum = 0;
        for(int j1=j; j1 < c; j1++){
          // Sum all cells in column j1
          // from row i to row i1
          colSum[j1] += a[i1][j1];
          sum += colSum[j1];
          maxSoFar = max(maxSoFar, sum);
        }
      }
    }
  }
  */
  // Init partial sums vector
  vector<vector<int>> colSums(r, vector<int>(c, 0));
  for(int j=0; j < c; j++){
    int partialSum = 0;
    for(int i=0; i < r; i++){
      partialSum += a[i][j];
      colSums[i][j] = partialSum;
    }
  }
  int maxSoFar = 0;
  for(int i=0; i < r; i++){
    for(int i1=i; i1<r; i1++){
        // Find arrMax
        int arrMax=0, sum=0;
        for(int j=0; j < c; j++){
          int item = colSums[i1][j]-( i > 0 ? colSums[i-1][j] : 0); 
          sum = max(sum+item, 0);
          arrMax = max(sum, arrMax);
        }
      maxSoFar = max(maxSoFar, arrMax);
    }
  }


  out << maxSoFar;
  in.close();
  out.close();
  return 0;
}

