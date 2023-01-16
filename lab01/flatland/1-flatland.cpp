#include <iostream>
#include <fstream>
#include <deque>
#include <vector>
#include <set>

using namespace std;

/*
O(n*n!*2^n)
Precalculating solutions for all possible permutations
*/

struct Tri{
  char o;
  int i;
  bool operator==(const Tri& t) const{
    return o == t.o;
  }
  bool operator==(const char& c) const{
    return o == c;
  }
};

typedef deque<Tri> Coda;
typedef set<int> Sol;
typedef pair<Coda, Sol> TableItem;
typedef vector<TableItem> TableRow;
typedef vector<TableRow> Table;
Sol sol;

bool sx(Tri t){
  return t.o == 's';
}
bool dx(Tri t){
  return t.o == 'd';
}
Tri next(Tri t){
  t.o = t.o == 's' ? 'd' : 's';
  return t;
}

int main(int argc, char *argv[]){
  ifstream in("input.txt");
  ofstream out("output.txt");

  int n;
  in >> n;
  Coda ci;

  for(int i=0; i < n; i++){
    char o;
    in >> o;
    ci.push_back({o, i});
  }

  // Building backtrack table====================

  vector<deque<TableItem>> table(n/2, deque<TableItem>());
  
  Coda ct;
  Sol s;
  s.insert(0);
  ct.push_back({'s', 0});
  table[0].push_back(TableItem(ct, s));
  ct[0] = {'d', 0};
  table[0].push_back(TableItem(ct, s));

  for(int k=3; k < n; k+=2){
    deque<Tri> c(k);
    deque<Tri> cl(k);

    // Init deques
    for(int i=0; i < k; i++){
      c[i] = {'s', i};
      cl[i] = {'d', i};
    }

    // Get permutations
    while(c != cl){
      // Calculate sol
      Sol sol;
      for(int i=1; i < (k-1); i++){
        Coda c1 = c;
        auto t = c1.begin()+i;
        if(sx(*t)){
          c1.erase(t-1, t+1);
        }else if(dx(*t))
          c1.erase(t, t+2);  
        // Do table lookup and set union
        for(TableItem item: table[(k-2)/2]){
          if(item.first == c1){
            for(int p: item.second)
              sol.insert(c1[p].i);
          }
        }
      }
      // Add sol to table
      table[k/2].push_back(TableItem(c, sol));

      // Get new permutation
      for(int i=0; i < k; i++){
        if(c[i] == 'd'){
          c[i] = next(c[i]);
        }else{
          c[i] = next(c[i]);
          break;
        }
      }
    }

    // Last permutation
    // Calculate sol
    Sol sol;
    for(int i=1; i < (k-1); i++){
      Coda c1 = c;
      auto t = c1.begin()+i;
      if(sx(*t)){
        c1.erase(t-1, t+1);
      }else if(dx(*t))
        c1.erase(t, t+2);  
      // Do table lookup and set union
      for(TableItem item: table[(k-2)/2]){
        if(item.first == c1){
          for(int p: item.second)
            sol.insert(c1[p].i);
        }
      }
    }
    // Add sol to table
    table[k/2].push_back(TableItem(c, sol));

    // Get new permutation
    for(int i=0; i < k; i++){
      if(c[i] == 'd'){
        c[i] = next(c[i]);
      }else{
        c[i] = next(c[i]);
        break;
      }
    }
  }//======================

  // Find sol
  Sol sol;
  for(int i=1; i < (n-1); i++){
    Coda c1 = ci;
    auto t = c1.begin()+i;
    if(sx(*t)){
      c1.erase(t-1, t+1);
    }else if(dx(*t))
      c1.erase(t, t+2);  
    // Do table lookup and set union
    for(TableItem item: table[(n-2)/2]){
      if(item.first == c1){
        for(int p: item.second)
          sol.insert(c1[p].i); 
      }
    }
  }

  out << sol.size() << endl;
  for(int i: sol)
    out << i << " ";
  
  in.close();
  out.close();
  return 0;
}