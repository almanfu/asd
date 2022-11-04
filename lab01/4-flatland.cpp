#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <set>
#include <iomanip>

using namespace std;

/*
Find canErase sequences
with Backtracking on indexes
*/
ofstream out("out.txt");

bool sx(char t){
  return t == 's';
}
bool dx(char t){
  return t == 'd';
}
void print(vector<char> v){
  out << string(4*v.size(),'=') << endl;
  for(char e: v)
    out << setw(3) << e << " ";
  out << endl;  
}
void print(vector<int> v){
  for(int e: v)
    out << setw(3) << e << " ";
  out << endl;
}

bool canErase1(vector<char> c, int n){
  while(n!=2){
    //else
    int sumDx = 0;
    int i = n-1;//indice prima occ.(da fine) di una s
    while(i>=1 && dx(c[i--]))
      sumDx++;
    i++;

    //cout << "i=" << i << " sumDx=" << sumDx << endl;//

    if(dx(c[i]))// => i==1
      return false;
    else if(sumDx%2==0)//i>=2
      return true;
    //else
    int j=i;//indice prima occ.(da i) di una d
    while(j>=2 && sx(c[j-=2]))
      ;
    //cout << "j=" << j << endl;//
    
    if(j==0)
      return false;
    //else
    c.erase(c.begin()+j, c.end()-sumDx+1);
      /*
    for(char t: c)
      cout << setw(3) << t << " ";
    cout << endl;*/
    n = c.size();
  }
  return sx(c[1]);
}

bool canErase(vector<char> c, int n){
  if(n==2)
    return sx(c[1]);

  bool res = false;
  for(int i=1; i<n && !res && (i<n-1 || sx(c[i])); i++){

    // n >= 4
    auto c1 = c;
    auto it = c1.begin();
    if(sx(c[i])){
      c1.erase(it+i-1, it+i+1);
    }else if(dx(c[i]))
      c1.erase(it+i, it+i+2);
    res = canErase(c1, n-2);
    //
  }
  return res;
}

void print_stats(vector<char> c, int n){
  if(canErase(c, n))
    out << "\033[31m";
  
  auto sumRight = vector<int>(n, 0);
  for(int i=n-1; i >= 1; i--)
    sumRight[i]=(i==(n-1)?0:sumRight[i+1])+(c[i]=='s'?1:-1);

  print(c);
  print(sumRight);

  out << "\033[0m";
}

int main(int argc, char *argv[]){
  int n = 15;
  for(int k=2; k < n; k+=2){
    vector<char> c(k);
    vector<char> cl(k);
    // Init vectors
    for(int i=0; i < k; i++){
      c[i] = 's';
      cl[i] = 'd';
    }
    // Get permutations
    while(c != cl){
      //Current permutation
      print_stats(c, k);
      // Get new permutation
      for(int i=0; i < k; i++){
        if(c[i] == 'd'){
          c[i] = c[i]=='d'?'s':'d';
        }else{
          c[i] = c[i]=='d'?'s':'d';
          break;
        }
      }
    }
    //Last permutation
    print_stats(c, k);
  }
  out.close();

  return 0;
}

