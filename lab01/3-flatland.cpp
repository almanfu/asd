#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <set>
#include <iomanip>

using namespace std;

/*
Finding sumLeft, sumRight correlations
with Backtracking on indexes
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
vector<int> sumLeft;
vector<int> sumRight;
ofstream out;
void print(Coda c, int depth=0){
  string depthString(depth, '_');
  out << depthString;

  for(Tri t: c)
    out << setw(3)<< t.i << " ";
  out << endl << depthString;

  for(Tri t: c)
    out << setw(3)<< t.o << " ";
  out << endl << depthString;

  for(Tri t: c)
    out << setw(3)<< sumLeft[t.i] << " ";
  out << endl << depthString;

  for(Tri t: c)
    out << setw(3)<< sumRight[t.i] << " ";
  out << endl;
}
void print(set<int> s){
  out << s.size() << endl;
  for(int i: s)
    out << setw(3) << i << " ";
  out << endl;
  for(int i: s)
    out << setw(3) << sumLeft[i] << " ";
  out << endl;
  for(int i: s)
    out << setw(3) << sumRight[i] << " ";
  out << endl;
}
void print(vector<int> s, int depth){
  string depthString(depth, '_');
  out << depthString;
  for(int i: s)
    out << setw(3) << i << " ";
  out << endl << depthString;
  for(int i: s)
    out << setw(3) << sumLeft[i] << " ";
  out << endl;
  for(int i: s)
    out << setw(3) << sumRight[i] << " ";
  out << endl;
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
  //print(c, depth);//
  if(c.size() == 1){
    sol.insert(c[0].i);
    //
    out << endl;
    print(seq, 0);
    out << "=========================" << endl;
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

      seq.pop_back();//
    
    }
  }

}

int main(int argc, char *argv[]){
  ifstream in(argv[1]);
  out = ofstream("myoutput/"+string(argv[1]));
  int n;
  in >> n;
  Coda c;

  for(int i=0; i < n; i++){
    char o;
    in >> o;
    c.push_back({o, i});
  }

  sumLeft = vector<int>(n, 0);
  for(int i=2; i < n; i++){
    sumLeft[i]=sumLeft[i-1] + (c[i-1].o=='s'?1:-1);
  }

  sumRight = vector<int>(n, 0);
  for(int i=n-3; i >= 0; i--){
    sumRight[i]=sumRight[i+1] + (c[i+1].o=='d'?1:-1);
  }

  table = Table(n, TableRow());

  fun(c);

  print(c);

  print(sol);//
  
  in.close();
  out.close();
  return 0;
}

