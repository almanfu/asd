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

int main(int argc, char *argv[]){
  ifstream in("input.txt");
  ofstream out("output.txt");

  int n;
  in >> n;
  vector<int> v(n+1);
  for(int i=1; i<=n; i++)
    in >> v[i];

  int minMoves=0;
  int i=1;
  while(i<=n){
    if(v[i] == i){
      v[i]=0;
      i++;
    }
    else if(v[i] != 0){
      int next = v[i];
      v[i]=0;
      if(v[next] != 0)
        minMoves++;
      i=next;
    }else{
      i++;
    }
  }
  int minCost=0;
  


  out << minMoves << ' ' << minCost;
  in.close();
  out.close();
  return 0;
}

