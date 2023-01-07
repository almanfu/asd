#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <set>

using namespace std;

/*

*/

int max(int a, int b){
  return a >= b ? a : b;
}

int longestSingle(int A[], int n){
  set<int> s = set<int>();
  int maxLength = 0;
  int thisLength = 0;

  for(int i=0; i < n; i++){
    if(s.find(A[i]) != s.end()){
      s.clear();
      thisLength = 1;
    }else{
      thisLength += 1;
    }
    maxLength = max(thisLength, maxLength);
    s.insert(A[i]);
  }

  return maxLength;
}

int main(int argc, char *argv[]){
  int A[] = {1, 2, 3, 2, 4, 1, 3, 4, 5, 6, 7, 8, 4};

  cout << longestSingle(A, 13) << endl;

  return 0;
}
