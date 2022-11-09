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

  int minCost=0;
  int startCycle=0;
  int minCycle;
  int sumCycle;
  int lenCycle;


  int i=1;
  while(i<=n){
    if(v[i] == i){
      v[i]=0;
      i++;
    }
    else if(v[i] != 0){
      int next = v[i];
      if(startCycle == 0){
        //Init cycle parameters
        startCycle = i;
        minCycle = v[i];
        sumCycle = lenCycle = 0;
      }

      minCycle = v[i]<minCycle?v[i]:minCycle;
      sumCycle += v[i];
      lenCycle++;

      if(v[next] != 0){
        minMoves++;
      }else{
        // Formula costo del ciclo
        if(lenCycle == 2){
          minCost += sumCycle;
        }else if(lenCycle == 3){
          minCost += sumCycle-minCycle + (lenCycle-1)*minCycle;
        }else if(minCycle > (lenCycle+1)/(lenCycle-3)){
          minCost += sumCycle-minCycle + (lenCycle-1)+2*(minCycle+1);
        }else{
          minCost += sumCycle-minCycle + (lenCycle-1)*minCycle;
        }
        //cout << minCycle << ' ' << minCost << ' ' << lenCycle << endl;
        startCycle = 0;
      }
      v[i]=0;
      i=next;
    }else{
      i++;
    }
  }



  out << minMoves << ' ' << minCost;
  in.close();
  out.close();
  return 0;
}

