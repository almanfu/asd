#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <cmath>

using namespace std;

/*

*/

int shift(int A[], int i, int j){
  if(i==j)
    return i;
  else{
    int m = floor((i+j)/2);
    if(A[m]>A[j])
      return shift(A, m+1, j);
    else
      return shift(A, i, m);
  }
}

int shiftlin(int A[], int n){
  int min=INT32_MAX;
  int res=0;
  for(int i=0; i < n; i++){
    if(A[i]<min){
      min=A[i];
      res=i;
    }
  }
  return res;
}

int compare(const void* a, const void* b){
  return (*(int*)a-*(int*)b);
}

void test(){
  for(int _=0; _<1000;_++){
    int n = rand()%10000;
    int* A = new int[n];
    for(int i=0; i < n; i++)
      A[i] = rand();
    qsort(A, n, sizeof(int), compare);
    if(shift(A, 1-1, n-1)!=shiftlin(A, n)){
      cout << "wrong" << endl;
    }
    delete[] A;
  }  
}

int main(int argc, char *argv[]){

  test();

  return 0;
}
