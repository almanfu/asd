#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <set>

using namespace std;

/*
O(n!)
Backtracking on orientations
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

typedef vector<Tri> Coda;
typedef set<int> Sol;
typedef pair<Coda, Sol> TableItem;
typedef vector<TableItem> TableRow;
typedef vector<TableRow> Table;

Table table;

bool sx(Tri t){
  return t.o == 's';
}
bool dx(Tri t){
  return t.o == 'd';
}

Sol fun(Coda c, int depth=0){
  Sol sol;

  if(c.size() == 1){
    sol.insert(0);
  }else{/*=> c.size() >= 3*/
    for(int i=1; i < (c.size()-1); i++){
      Coda c1 = c;
      auto t = c1.begin()+i;
 
      if(sx(*t)){
        c1.erase(t-1, t+1);
      }else if(dx(*t))
        c1.erase(t, t+2);

      // Table lookup
      bool match = false;
      for(TableItem ti: table[depth]){
        if(ti.first == c1){
          match = true;
          // Add to sol
          for(int p: ti.second)
            sol.insert(c1[p].i);
          break;
        }
      }
      if(!match){
        Coda c2 = c1;
        int p=0;
        for(Tri& t: c2)
          t.i = p++;
        Sol sol2 = fun(c2, depth+1);
        table[depth].push_back(TableItem(c2, sol2));
        for(int p: sol2)
          sol.insert(c1[p].i);
      }

    }
  }
  return sol;
}
/*====
void print(Coda c){
  for(Tri t: c)
    cout << t.o;
  cout << endl;
  for(Tri t: c)
    cout << t.i;
  cout << endl;
}
void print(Sol s){
  for(int i: s)
    cout << i << " ";
  cout << endl;
}
void print(TableItem ti){
  print(ti.first);
  print(ti.second);
}
void print(Table table, int row=-1){
  if(row != -1){
    for(TableItem ti: table[row])
      print(ti);
    return;
  }
  int i=0;
  for(TableRow tr: table){
    cout << "========" << i << "========" << endl;
    for(TableItem ti: tr)
      print(ti);
    i++;
  }
}
//====*/
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

  table = Table(n, vector<TableItem>());

  Sol sol = fun(c);

  //print(table);
  
  out << sol.size() << endl;
  for(int i: sol)
    out << i << " ";
  
  in.close();
  out.close();
  return 0;
}

