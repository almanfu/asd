#include <iostream>
#include <fstream>

#include <bitset>
#include <vector>
#include <list>
#include <queue>
#include <deque>
#include <map>
#include <stack>
#include <set>

#include <string>
#include <cctype>

#include <utility>
#include <functional>
#include <algorithm>

using namespace std;

/*

*/

void merge(int *a, int l, int piv, int r) {
   int i, j, k, nl, nr;
   nl = piv-l+1; nr = r-piv;
   int larr[nl], rarr[nr];
   for(i = 0; i<nl; i++)
      larr[i] = a[l+i];
   for(j = 0; j<nr; j++)
      rarr[j] = a[piv+1+j];
   i = 0; j = 0; k = l;
   while(i < nl && j<nr) {
      if(larr[i] <= rarr[j]) {
         a[k] = larr[i];
         i++;
      }else{
         a[k] = rarr[j];
         j++;
      }
      k++;
   }
   while(i<nl){
      a[k] = larr[i];
      i++; k++;
   }
   while(j<nr) {
      a[k] = rarr[j];
      j++; k++;
   }
}

void merge_sort(int *a, int l, int r){
  if(l < r) {
      int piv = l+(r-l)/2;
      merge_sort(a, l, piv);
      merge_sort(a, piv+1, r);
      merge(a, l, piv, r);
   }
}

int main(int argc, char *argv[]){
  ifstream in("input.txt");
  ofstream out("output.txt");

  int n;
  in >> n;

  int* a = new int[n];

  for(int i=0; i < n; i++){
    in >> a[i];
  }

  merge_sort(a, 0, n-1);

  for(int i=0; i < n; i++){
    out << a[i] << ' ';
  }

  delete[] a;
  in.close();
  out.close();
  return 0;
}

