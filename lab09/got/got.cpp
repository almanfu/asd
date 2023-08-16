#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <unordered_set>
#include <algorithm>
#include <memory>

using namespace std;


/*

*/
class Cell;
class Paint;

int N, M;
vector<vector<Cell>> G;


class Cell
{
public:
  int i, j;
  shared_ptr<Paint> paint;
  static vector<Cell *> neighbors(Cell *);
  Cell();
  Cell(int, int);
  bool isPainted();
  void fillPaint(shared_ptr<Paint> p);
  void unpaint();
};

class Paint : public std::enable_shared_from_this<Paint>
{
public:
  int size;
  int currSize;
  int a;
  int alpha;
  bool dead;
  unordered_set<Cell *> newAdj;
  unordered_set<Cell *> adj; // inv: all cells in adj are not painted
  Paint(int size, Cell* cell) : size(size), currSize(0), a(0), alpha(1), dead(false){
    adj.insert(cell);
  }
  void pour(int r){
    a += r;
  }
  void addToNewAdj(Cell *c){
    newAdj.insert(c);
  }
  void removeFromAdj(Cell* c){
    adj.erase(c);
  }
  // uniform expansion
  void expand(){
    for(Cell* c: newAdj)
      adj.insert(c);
    newAdj.erase(newAdj.begin(), newAdj.end());
    while (adj.size() <= a && !isDead())
    {
      // fillPaint adj (can fillPaint them all)
      for(Cell* c: adj){
        a--;
        c->fillPaint(shared_from_this());
        currSize++;
        // add neighbours of c to newAdj(if they are not filled)
        // or merge them
        for (Cell *n : Cell::neighbors(c))
        {
          shared_ptr<Paint> peer = n->paint;
          if (!n->isPainted())
            addToNewAdj(n);
          else if (peer->size == size)
          { // merge
            if (peer->currSize + currSize <= size)
            { // valid, expander feeds and merges
              // feed
              alpha += peer->alpha;
              a += peer->a;
              // update adj
              for (Cell *c : peer->adj)
              {
                if (c->paint.get() != this)
                  addToNewAdj(c);
              }
              // update internal cells with bfs
              queue<Cell *> Q;
              Q.push(n);
              while(!Q.empty()){
                Cell *u = Q.front();
                Q.pop();
                if(u->isPainted() && u->paint.get() == peer.get()){
                  u->paint = shared_from_this();
                  for (Cell *v : Cell::neighbors(u))
                    Q.push(v);
                }
              }
              // peer dies
              peer->die();
            }
            else
            { // conflict, biggest feeds and cleans smallest
              if (this->currSize > peer->currSize)
                this->feedandclean(peer, n);
              else{
                peer->feedandclean(shared_from_this(), n);
                break;
              }
            }
          }
        }
        // stop growing only after merge
        if(size == currSize){
          die();
          break;
        }
      }
      adj = newAdj;
      newAdj.erase(newAdj.begin(), newAdj.end());
    }
  }
  void die(){
    dead = true;
  }
  bool isDead(){
    return dead == true;
  }
  void feedandclean(shared_ptr<Paint> smaller, Cell *n)
  {
    //feed
    alpha += smaller->alpha;
    a += smaller->a;
    //  update internal cells with bfs
    queue<Cell *> Q;
    Q.push(n);
    while (!Q.empty())
    {
      Cell *u = Q.front();
      Q.pop();
      if (u->isPainted() && u->paint.get() == smaller.get())
      {
        for (Cell *v : Cell::neighbors(u))
          Q.push(v);
      }
    }
    //update adjs
    for (Cell *c : smaller->adj)
      c->unpaint();
    // smaller dies
    smaller->die();
  }
};

// Cell
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
  bool Cell::isPainted()
  {
    return paint != nullptr;
  }
  void Cell::fillPaint(shared_ptr<Paint> p)
  {
    paint = p;
    // remove me from neighbors paint adjs
    for (Cell *n : Cell::neighbors(this))
    {
      if (n->isPainted())
      {
        n->paint->removeFromAdj(this);
      }
    }
  }
  void Cell::unpaint(){
    paint = nullptr;
    for (Cell *n : neighbors(this))
    {
      //update newAdj of painted neighbors
      if (n->isPainted())
        n->paint->newAdj.insert(this);
    }
  }
//

int main()
{
  ifstream in("input.txt");
  ofstream out("output.txt");

  // setup
  in >> N >> M;
  G = vector<vector<Cell>>(N, vector<Cell>(M));
  vector<shared_ptr<Paint>> Q;
  for (int i = 0; i < N; i++)
  {
    for (int j = 0; j < M; j++)
    {
      int size;
      in >> size;
      G[i][j] = Cell(i, j);
      if (size != 0)
      {
        shared_ptr<Paint> p = make_shared<Paint>(Paint(size, &G[i][j]));
        Q.push_back(shared_ptr<Paint>(p));
      }
     
    }
  }

  //pouring
  int delta=1;
  int r = 1;
  vector<shared_ptr<Paint>> L;
  while(!(Q.empty() && L.empty())){
    while(!Q.empty()){
      //sort Q by decreasing size
      for(shared_ptr<Paint> p: Q){
        if(!p->isDead()){
          p->pour(r);
          p->expand();
        }
      }
      // remove dead paints
      Q.erase(remove_if(Q.begin(), Q.end(), [](shared_ptr<Paint>& p)
                      { return p->isDead(); }), Q.end());
      r += delta;
    }
    if(!L.empty()){
      // sort L by decreasing currSize
      shared_ptr<Paint> p = *L.begin();

      L.erase(L.begin());
    }
  }

  //output
  for (int i = 0; i < N; i++){
    for (int j = 0; j < M; j++)
      out << (G[i][j].isPainted() ? G[i][j].paint->size : 0) << ' ';
    out << endl;
  }

    in.close();
  out.close();
  return 0;
}
