#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <set>
#include <iomanip>

using namespace std;

/*
O(n^3)
canErase
*/

bool sx(char t){
  return t == 's';
}
bool dx(char t){
  return t == 'd';
}
char toggle(char t){
  return (dx(t)?'s':'d');
}
bool canErase(vector<char> c){// O(n^2)
  int n = c.size();
  while(n!=2){
    //else
    int sumDx = 0;
    int i = n-1;//indice prima occ.(da fine) di una s
    while(i>=1 && dx(c[i--]))// O(n)
      sumDx++;
    i++;

    if(dx(c[i]))// => i==1
      return false;
    else if(sumDx%2==0)//i>=2
      return true;
    //else
    int j=i;//indice prima occ.(da i) di una d
    while(j>=2 && sx(c[j-=2]))// O(n)
      ;
    
    if(j==0)
      return false;
    //else
    c.erase(c.begin()+j, c.end()-sumDx+1);// O(n)
    n = c.size();
  }
  return sx(c[1]);
}
int main(int argc, char *argv[]){
  //get input
  ifstream in("input.txt");
  ofstream out("output.txt");
  int n;
  in >> n;
  vector<char> c(n), ci(n);
  for(int i=0; i<n; i++){//O(n)
    in >> c[i];
    ci[n-1-i] = toggle(c[i]);
  }
  //find solutions
  vector<int> sol;
  auto it = c.begin();
  auto iti = ci.begin();
  for(int i=0; i<n;i+=2){//O(n^3)
    if(i==0){
      if(canErase(vector<char>(iti, iti+n-1)))//O(n^2)
        sol.push_back(i);
    }else if(i==n-1){
      if(canErase(vector<char>(it, it+n-1)))
        sol.push_back(i);
    }else{
      if(canErase(vector<char>(it, it+i)) && canErase(vector<char>(iti, iti+n-1-i)))
        sol.push_back(i);
    }
  }

  //print solutions
  out << sol.size() << endl;
  for(int s: sol)
    out << s << " ";
  out.close();
  return 0;
}

