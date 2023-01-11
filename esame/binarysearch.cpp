#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <cmath>
#include <algorithm>

using namespace std;

/*

*/

int binarySearch(int A[], int v, int i, int j){
  if(i > j)
    return -1;
  else if (i == j && A[i] == v)
    return i;
  else{
    int m = floor((i+j)/2);
    if (A[m] < v)
      return binarySearch(A, v, m+1, j);
    else
      return binarySearch(A, v, i, m);
  }
}

int main(int argc, char *argv[]){
  int A[] = {1, 1, 1, 1, 2, 2, 4, 5, 5, 6, 7, 7, 8};

  cout << binarySearch(A, 9, 1-1, 13-1) << endl;

  return 0;
}
