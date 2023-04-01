#include <iostream>
#include <fstream>

#include <unordered_set>

using namespace std;

/*

*/

bool four(int A[], int n, int k){
  unordered_set<int> s = unordered_set<int>(n);
  for(int i=0; i < n; i++){
    for(int j=0; j < n; j++){
      s.insert(A[i]+A[j]);
    }
  }
  for(int i=0; i < n; i++){
    for(int j=0; j < n; j++){
      if(s.find(k-A[i]-A[j]) != s.end()){
        return true;
      }
    }
  }
  return false;
}

int main(int argc, char *argv[]){
  const int n = 1000;
  int *A = new int[n];
  int k = rand();

  for(int i=0; i < n; i++){
    A[i] = rand();
  }

  cout << (four(A, n, k) ? "true" : "false") << endl;  

  delete[] A;
  return 0;
}
