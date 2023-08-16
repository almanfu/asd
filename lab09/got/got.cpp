#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <unordered_set>
#include <algorithm>
#include <memory>
//#include "got.h"

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
  unsigned int a;
  unsigned int alpha;
  bool dead;
  Cell *root;
  unordered_set<Cell *> addAdj;
  unordered_set<Cell *> delAdj;
  unordered_set<Cell *> adj; // inv: all cells in adj are not painted
  Paint(int size, Cell* cell) : size(size), currSize(0), a(0), alpha(1), dead(false), root(cell){
    adj.insert(cell);
  }
  void pour(int r){
    a += r*alpha;
  }
  void addToAdj(Cell *c){
    addAdj.insert(c);
  }
  void delFromAdj(Cell *c)
  {
    delAdj.insert(c);
  }
  void updateAdj(bool clearAdj){
    if (clearAdj)
      adj.clear();
    else{
      for (Cell *c : delAdj)
        adj.erase(adj.find(c));      
    }    for (Cell *c : addAdj)
      adj.insert(c);

    addAdj.clear();
    delAdj.clear();
  }
  // uniform expansion
  void expand(){
    updateAdj(false);
    while (adj.size() != 0 && a != 0 && adj.size() <= a && !isDead())
    {
      // fillPaint adj (can fillPaint them all)
      for(Cell* c: adj){
        a--;
        // calls a removeFromAdj on me too!
        c->fillPaint(shared_from_this());
        currSize++;
        // add neighbours of c to newAdj(if they are not filled)
        // or merge them
        for (Cell *n : Cell::neighbors(c))
        {
          shared_ptr<Paint> peer = n->paint;
          if (!n->isPainted())
            addToAdj(n);
          else if (peer->size == size && peer.get() != this)
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
                  addToAdj(c);
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
                this->feedandclean(peer);
              else{
                peer->feedandclean(shared_from_this());
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
      updateAdj(true);
    }
  }
  void die(){
    dead = true;
  }
  bool isLocked(){
    return adj.empty();
  }
  bool isDead(){
    return dead;
  }
  void feedandclean(shared_ptr<Paint> smaller)
  {
    //feed
    alpha += smaller->alpha;
    a += smaller->a;
    smaller->dieandclean();
  }
  void dieandclean(){
    die();
    // update internal cells with bfs
    queue<Cell *> Q;
    Q.push(root);
    while (!Q.empty())
    {
      Cell *u = Q.front();
      Q.pop();
      if (u->isPainted() && u->paint.get() == this)
      {
        u->paint = nullptr;
        for (Cell *v : Cell::neighbors(u))
          Q.push(v);
      }
    }
    // update adjs
    for (Cell *c : adj)
      c->unpaint();
  }
};

// Cell
  vector<Cell *> Cell::neighbors(Cell *c)
  {
    int i = c->i;
    int j = c->j;
    vector<Cell *> res;
    if (i + 1 < N)
      res.push_back(&G[i + 1][j]);
    if (0 <= i - 1)
      res.push_back(&G[i - 1][j]);
    if (j + 1 < M)
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
    for (Cell *n : Cell::neighbors(&G[i][j]))
    {
      if (n->isPainted())
      {
        n->paint->delFromAdj(&G[i][j]);
      }
    }
  }
  void Cell::unpaint(){
    paint = nullptr;
    for (Cell *n : neighbors(&G[i][j]))
    {
      //update adj of painted neighbors
      if (n->isPainted())
        n->paint->addToAdj(&G[i][j]);
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
  unsigned int delta=1;
  unsigned int r = 1;
  vector<shared_ptr<Paint>> L;
  while(!(Q.empty() && L.empty())){
    while (!Q.empty())
    {
      //sort Q by decreasing size
      sort(Q.begin(), Q.end(), [](const shared_ptr<Paint> &a, const shared_ptr<Paint> &b)
                { return a->size > b->size; });
      for(shared_ptr<Paint> p: Q){
        if(!p->isDead()){
          p->pour(r);
          p->expand();
        }
      }
      // remove dead paints
      Q.erase(remove_if(Q.begin(), Q.end(), [](shared_ptr<Paint>& p)
                      { return p->isDead(); }), Q.end());
      // update adjs
      for (shared_ptr<Paint> p : Q){
        p->updateAdj(false);
        if(p->isLocked())
          L.push_back(p);
      }
      // remove locked paints
      Q.erase(remove_if(Q.begin(), Q.end(), [](shared_ptr<Paint> &p)
                        { return p->isLocked(); }),Q.end());
      r += delta;
    }
    if(!L.empty()){
      // sort L by decreasing currSize
      sort(L.begin(), L.end(), [](const shared_ptr<Paint> &a, const shared_ptr<Paint> &b)
                { return a->currSize > b->currSize; });
      // clean largest paint
      (*L.begin())->dieandclean();
      L.erase(L.begin());
      for(shared_ptr<Paint> p : L)
        Q.push_back(p);
      L.clear();
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
