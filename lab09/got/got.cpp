#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <unordered_set>

using namespace std;


/*

*/
class Cell;
class Paint;

static int N, M;
static vector<vector<Cell>> G;


class Cell
{
public:
  int i, j;
  Paint *paint;
  static void init(int &, int &, vector<vector<Cell>> &);
  static vector<Cell *> neighbors(Cell *);
  Cell();
  Cell(int, int);
  void fill(Paint *p);
  bool isFilled();
};

class Paint
{
public:
  int size;
  int currSize;
  int a;
  unordered_set<Cell*> adj; // inv: all cells in adj are not painted
  Paint(int size, Cell* cell) : size(size), currSize(0), a(0){
    adj.insert(cell);
  }
  void pour(int r){
    a += r;
  }
  void removeFromAdj(Cell* c){
    adj.erase(c);
  }
  // uniform expansion
  void expand(){
    while(adj.size() <= a){
      a -= adj.size();
      unordered_set<Cell *> newAdj;
      // fill adj (can fill them all)
      for(Cell* c: adj){
        c->fill(this);
        //add neighbours of C to newAdj(if they are not filled)
        for (Cell *n : Cell::neighbors(c))
        {
          if(!n->isFilled())
            newAdj.insert(n);
        }
      }
      adj = newAdj;
    }
  }
};

// Cell
  void Cell::init(int &_N, int &_M, vector<vector<Cell>> &_G)
  {
    N = _N;
    M = _M;
    G = _G;
  }
  vector<Cell *> Cell::neighbors(Cell *c)
  {
    int i = c->i;
    int j = c->j;
    vector<Cell *> res;
    if (i + 1 <= N)
      res.push_back(&G[i + 1][j]);
    if (0 <= i - 1)
      res.push_back(&G[i - 1][j]);
    if (j + 1 <= M)
      res.push_back(&G[i][j + 1]);
    if (0 <= j - 1)
      res.push_back(&G[i][j - 1]);
    return res;
  }
  Cell::Cell() {}
  Cell::Cell(int i, int j) : i(i), j(j), paint(nullptr) {}

  void Cell::fill(Paint *p)
  {
    paint = p;
    // remove me from neighbors paint adjs
    for (Cell *n : Cell::neighbors(this))
    {
      if (n->paint != nullptr){
        n->paint->removeFromAdj(this);
      }
    }
  }
  bool Cell::isFilled()
  {
    return paint != nullptr;
  }
//

int main()
{
  ifstream in("input.txt");
  ofstream out("output.txt");

  // setup
  in >> N >> M;
  G = vector<vector<Cell>>(N, vector<Cell>(M));
  vector<Paint*> Q;
  for (int i = 0; i < N; i++)
  {
    for (int j = 0; j < M; j++)
    {
      int size;
      in >> size;
      G[i][j] = Cell(i, j);
      if (size != 0)
      {
        Paint *p = new Paint(size, &G[i][j]);
        Q.push_back(p);
      }
     
    }
  }

  //pouring
  int delta=10;

  in.close();
  out.close();
  return 0;
}
