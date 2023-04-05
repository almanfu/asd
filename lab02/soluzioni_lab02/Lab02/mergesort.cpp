#include <fstream>
#include <vector>
#include <climits>
using namespace std;

vector<int> merge_sort(vector<int>& arr){
  if(arr.size()<=1) {
    return arr;
  }

  // spezzo l'array in due parti
  int mid=arr.size()/2;

  vector<int> left(&arr[0],&arr[mid]);
  vector<int> right(&arr[mid],&arr[arr.size()]);

  // ordino le due parti
  left=merge_sort(left);
  right=merge_sort(right);

  // inserisco guardie (valori fittizi limite)
  left.push_back(INT_MAX);
  right.push_back(INT_MAX);

  vector<int> result;

  int l=0, r=0;

  // merge
  for(unsigned int i=0; i<arr.size(); i++) {
    if(right[r]<left[l]) {
      result.push_back(right[r]);
      r++;
    } else {
      result.push_back(left[l]);
      l++;
    }
  }
  return result;
}

int main(void) {
  ifstream in("input.txt");
  int N;
  in >> N;
  vector<int> vec(N);

  // lettura input
  for(int i=0;i<N;i++) {
    in>>vec[i];  
  }

  vec = merge_sort(vec);

  ofstream out("output.txt");
  for(int el:vec) {
    out << el << " ";
  }
  out << endl;

  return 0;
}
