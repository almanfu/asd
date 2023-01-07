#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <stack>

using namespace std;

/*

*/

int fun(int A[], int i, int j){
  int m = (i+j)/2;
  if(i == j)
    return i;
  else if(A[m] < m)
    return fun(A, i, m-1);
  else if(A[m]>=m && A[m+1] >= m+1)
    return fun(A, m+1, j);
  else
    return m;
}

int hindex(int A[], int n){
  return fun(A, 0, n-1);
}

int main(int argc, char *argv[]){
  int A[] = {16, 12, 7, 5, 4, 3, 2, 1};
  
  cout << hindex(A, 8) << endl;

  return 0;
}
