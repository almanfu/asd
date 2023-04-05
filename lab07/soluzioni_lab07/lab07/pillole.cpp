#include <iostream>
#include <fstream>
using namespace std;
#define lint long long int 


lint arr[31][31];

lint compute(int W,int H){
  if(W==0)
    return 1;
  if(H<0)
    return 0;
  if(arr[W][H]==0)
    arr[W][H]= compute(W-1,H+1) + compute(W,H-1);
  return arr[W][H];
}
int N;

int main(){
  ifstream in("input.txt");
  ofstream out("output.txt");
  in>>N;
  out<<compute(N,0)<<endl;
  return 0;
}
