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
  else{
    int m = floor((i+j)/2.0);
    if (A[m] == v)
      return m;
    if (A[m] < v)
      return binarySearch(A, v, m+1, j);
    else
      return binarySearch(A, v, i, m-1);
  }
}

int searchFirst(int A[], int v, int i, int j){
  if(i > j)
    return -1;
  else if (i == j && A[i] == v)
    return i;
  else{
    int m = floor((i+j)/2.0);
    if (A[m] < v)
      return searchFirst(A, v, m+1, j);
    else
      return searchFirst(A, v, i, m);
  }
}

int searchLast(int A[], int v, int i, int j){
  if(i > j)
    return -1;
  else if (i == j && A[i] == v)
    return i;
  else{
    int m = ceil((i+j)/2.0);
    if (A[m] <= v)
      return searchLast(A, v, m, j);
    else
      return searchLast(A, v, i, m-1);
  }
}

int main(int argc, char *argv[]){
  int A[] = {1, 1, 1, 1, 2, 2, 4, 5, 5, 6, 7, 7, 8};

  cout << binarySearch(A, 8, 1-1, 13-1) << endl;
  cout << searchFirst(A, 8, 1-1, 13-1) << endl;
  cout << searchLast(A, 8, 1-1, 13-1) << endl;

  return 0;
}
