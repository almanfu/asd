#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <set>
#include <iomanip>

using namespace std;

/*
O(n!)
Backtracking on indexes
*/

struct Tri{
  char o;
  int i;
  bool operator==(const Tri& t) const{
    return t.i == i;
  }
};
typedef vector<Tri> Coda;
typedef set<int> Sol;
typedef vector<Coda> TableRow;
typedef vector<TableRow> Table;

Sol sol;
Table table;

bool sx(Tri t){
  return t.o == 's';
}
bool dx(Tri t){
  return t.o == 'd';
}
//
void print(Coda c, int depth){
  string depthString(depth, '-');
  Coda::iterator it = c.begin();
  cout << depthString;
  while(it != c.end())
    cout << (it++)->o;
  cout << endl;
  it = c.begin();
  cout << depthString;
  while(it != c.end())
    cout << (it++)->i;
  cout << endl;
}
void print(set<int> s){
  cout << s.size() << endl;
  for(int i: s)
    cout << i << " ";
  cout << endl;
}
void print(vector<int> s, int depth){
  string depthString(depth, '-');
  cout << depthString;
  for(int i: s)
    cout << i << " ";
  cout << endl;
}
vector<int> seq;
//

bool inTable(Coda c, int depth){
  bool res=false;
  auto it = table[depth].begin();
  auto end = table[depth].end();
  while(it != end && !res)
    res = *(it++) == c;
  return res;
}

void fun(Coda c, int depth=0){
  print(c, depth);//
  if(c.size() == 1){
    sol.insert(c[0].i);
    //
    cout << endl;
    print(seq, depth);
    cout << string(depth, '-') << "=========================" << endl;
    //
  }else{/*=> c.size() >= 3*/
    for(int i=1; i < (c.size()-1); i++){
      Coda c1 = c;
      auto t = c1.begin()+i;
      seq.push_back(t->i);//  
 
      if(sx(*t)){
        c1.erase(t-1, t+1);
      }else if(dx(*t))
        c1.erase(t, t+2);

      if(!inTable(c1, depth)){
        table[depth].push_back(c1);    
        fun(c1, depth+1);
      }
    }
  }

  seq.pop_back();//
}

int main(int argc, char *argv[]){
  ifstream in("input.txt");
  ofstream out("output.txt");

  int n;
  in >> n;
  Coda c;

  for(int i=0; i < n; i++){
    char o;
    in >> o;
    c.push_back({o, i});
  }

  table = Table(n, TableRow());

  fun(c);

  print(sol);//
  
  out << sol.size() << endl;
  for(int i: sol)
    out << i << " ";
  
  in.close();
  out.close();
  return 0;
}

