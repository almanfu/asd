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

int max(int a, int b){
  return a > b ? a : b;
}

int main(int argc, char *argv[]){
  ifstream in("input.txt");
  ofstream out("output.txt");

  // Load Array
  int n;
  in >> n;
  vector<int> a(n);
  for(int i=0; i < n; i++)
    in >> a[i];
  // Find arrMax
  int arrMax=0, sum=0;
  for(int j=0; j < n; j++){
    sum = max(sum+a[j], 0);
    arrMax = max(sum, arrMax);
  }
  // Write it out
  out << arrMax;
  in.close();
  out.close();
  return 0;
}
