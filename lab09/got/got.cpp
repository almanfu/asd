#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <unordered_set>
#include <algorithm>
#include <memory>
#include "got.h"

using namespace std;


/*

*/
class Cell;
class Paint;

int N, M;
vector<vector<unique_ptr<Cell>>> G;


class Cell
{
public:
  int i, j;
  shared_ptr<Paint> paint;
  static vector<Cell *> adj(Cell *);
  Cell();
  Cell(int, int);
  bool isFull();
  bool isClean();
  void fillPaint(shared_ptr<Paint> p);
  void cleanPaint();
  /*friend ostream &operator<<(ostream &os, const Cell &cell)
  {
    return os << cell.i << ' ' << cell.j;
  }*/
};

class Paint : public std::enable_shared_from_this<Paint>
{
public:
  int size;
  int currSize;
  unsigned int a;
  unsigned int alpha;
  bool dead;
  //bool someOldFull;
  int kills;
  Cell *root;
  unordered_set<Cell *> newClean;
  unordered_set<Cell *> oldFull;
  unordered_set<Cell *> cleanAdj; // inv: all cells in cleanAdj are not painted
  Paint(int size, Cell *cell) : size(size), currSize(0), a(0), alpha(1), dead(false), root(cell){
    cleanAdj.insert(cell);
    kills = 0;
  }
  void pourMore(int r){
    a += r*alpha;
  }
  void foundNewClean(Cell *c){
    newClean.insert(c);
  }
  void foundOldFull(Cell *c)
  {
    //someOldFull = true;
    oldFull.insert(c);
  }
  void updateCleanAdj(bool clearAdj){
    if (clearAdj)
      cleanAdj.clear();
    else{
      for (Cell *c : oldFull){
        // remove cells that have become full
        // might have been cleaned
        if(c->isFull() && cleanAdj.find(c) != cleanAdj.end())
          cleanAdj.erase(cleanAdj.find(c));
      }
      //someOldFull = false;
    }

    // add new clean adjacent cells
    for (Cell *c : newClean){
      // might have been filled
      if(c->isClean()){
        cleanAdj.insert(c);
      }
    }

    newClean.clear();
    oldFull.clear();
  }
  // uniform expansion
  void expand(){
    updateCleanAdj(false);
    while (!(isDead() || isComplete()) && cleanAdj.size() != 0 && a >= cleanAdj.size())
    {
      // fillPaint cleanAdj (can fillPaint them all)
      unordered_set<Cell*>::iterator it;
      for (it = cleanAdj.begin(); it != cleanAdj.end() && !(isDead() || isComplete()); it++)
      {
        Cell *fillCell = *it;
        if(fillCell->isFull())
          continue;
        a--;
        fillCell->fillPaint(shared_from_this());
        currSize++;
        // add neighbours of c to newAdj (if they are not filled)
        // or merge them
        for (Cell *newAdjCell : Cell::adj(fillCell))
        {
          shared_ptr<Paint> newAdjPaint = newAdjCell->paint;
          if (newAdjCell->isClean())
            foundNewClean(newAdjCell);
          else if (newAdjPaint->size == size && newAdjPaint.get() != this)
          { // merge with new adjacent paint
            // but first update it so that the INV holds
            newAdjPaint->updateCleanAdj(false);
            if (newAdjPaint->currSize + this->currSize <= size)
            { // valid, expander feeds and merges
              // feed
              alpha += newAdjPaint->alpha;
              a += newAdjPaint->a;
              currSize += newAdjPaint->currSize;
              // peer dies
              newAdjPaint->die();
              /// first we update the internal cells, then we explore new adj cells
              // update internal cells with bfs
              queue<Cell *> Q;
              Q.push(newAdjPaint->root);
              while(!Q.empty()){
                Cell *u = Q.front();Q.pop();
                if (u->isFull() && u->paint.get() == newAdjPaint.get())
                {
                  u->paint = this->shared_from_this();
                  /*
                  if(cleanAdj.find(u) != cleanAdj.end()){//this never holds
                    this->foundOldFull(u);
                    cout << "this never holds" << endl;
                  }*/
                  // explore adjacent cells
                  for (Cell *v : Cell::adj(u))
                    Q.push(v);
                }
              }
              // explore newAdjPaint->cleanAdj
              for (Cell *c : newAdjPaint->cleanAdj)
              {
                if (c->paint.get() != this) // this always holds? should, since we updated the adjPaint
                  foundNewClean(c);
                /*else
                  cout << "near you" << endl;*/
              }
            }
            else
            { // conflict, biggest feeds, smallest die and cleans
              if (this->currSize >= newAdjPaint->currSize)
                this->feedandclean(newAdjPaint);
              else{
                newAdjPaint->feedandclean(shared_from_this());
                //
                break;
              }
            }
          }
        }
      }
      // erase visited adjacent + update without erase
      cleanAdj.erase(cleanAdj.begin(), it);
      updateCleanAdj(false);
    }
  }
  void die(){
    dead = true;
  }
  bool isLocked(){
    return cleanAdj.empty() && !isComplete();
  }
  bool isDead(){
    return dead;
  }
  bool isComplete(){
    return currSize == size;
  }
  void feedandclean(shared_ptr<Paint> smaller)
  {
    kills++;
    // feed
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
      if (u->isFull() && u->paint.get() == this)
      {
        u->paint = nullptr;
        for (Cell *v : Cell::adj(u))
          Q.push(v);
      }
    }
    // update adjs
    for (Cell *c : cleanAdj)
      c->cleanPaint();
  }
};

// Cell
  vector<Cell *> Cell::adj(Cell *c)
  {
    int i = c->i;
    int j = c->j;
    vector<Cell *> res;
    if (i + 1 < N)
      res.push_back(G[i + 1][j].get());
    if (0 <= i - 1)
      res.push_back(G[i - 1][j].get());
    if (j + 1 < M)
      res.push_back(G[i][j + 1].get());
    if (0 <= j - 1)
      res.push_back(G[i][j - 1].get());
    return res;
  }
  Cell::Cell() {}
  Cell::Cell(int i, int j) : i(i), j(j), paint(nullptr) {}
  bool Cell::isFull()
  {
    return paint != nullptr;
  }
  bool Cell::isClean(){
    return paint == nullptr;
  }
  void Cell::fillPaint(shared_ptr<Paint> p)
  {
    paint = p;
    // remove me from adj paint adjs
    for (Cell *n : Cell::adj(G[i][j].get()))
    {
      if (n->isFull())
      {
        n->paint->foundOldFull(G[i][j].get());
      }
    }
  }
  void Cell::cleanPaint(){
    paint = nullptr;
    for (Cell *ac : adj(G[i][j].get()))
    {
      //update cleanAdj of painted adj
      if (ac->isFull())
        ac->paint->foundNewClean(G[i][j].get());
    }
  }
//

int main()
{
  ifstream in("input.txt");
  ofstream out("output.txt");

  // setup
  in >> N >> M;
  G.resize(N);
  vector<shared_ptr<Paint>> Q;
  for (int i = 0; i < N; i++)
  {
      G[i].resize(M);
      for (int j = 0; j < M; j++)
      {
        int size;
        in >> size;
        G[i][j] = move(unique_ptr<Cell>(new Cell(i, j)));
        if (size != 0)
        {
        shared_ptr<Paint> p = make_shared<Paint>(Paint(size, G[i][j].get()));
        Q.push_back(shared_ptr<Paint>(p));
      }
     
    }
  }

  //pouring
  unsigned int delta=1;
  unsigned int r = 1;
  //p in Q => not complete, not dead
  vector<shared_ptr<Paint>> L;
  while(!(Q.empty() && L.empty())){
    while (!Q.empty())
    {
      // sort Q by decreasing size
      sort(Q.begin(), Q.end(), [](const shared_ptr<Paint> &a, const shared_ptr<Paint> &b)
                { return a->size > b->size; });
      for(shared_ptr<Paint> p: Q){
        // keep pouring on locked paints
        if(! (p->isDead())){
          p->pourMore(r);
          p->expand();
        }
      }
      // remove dead/complete paints
      Q.erase(remove_if(Q.begin(), Q.end(), [](shared_ptr<Paint> &p)
                        { return p->isDead() || p->isComplete(); }),
              Q.end());
      // update adjs
      unsigned int countLocked = 0;
      for (shared_ptr<Paint> p : Q)
      {
        p->updateCleanAdj(false);
        if(p->isLocked())
          L.push_back(p);
      }
      // remove locked paints at the end of the iteration
      Q.erase(remove_if(Q.begin(), Q.end(), [](shared_ptr<Paint> &p)
                        { return p->isLocked(); }),
              Q.end());
      /* before removing largest locked, wait all to be locked
      if (countLocked == Q.size())
      {
        L = Q;
        Q.clear();
      }*/
      r += delta;
    }
    // if some are locked, remove largest locked
    if(!L.empty()){
      //sort L by decreasing currSize
      sort(L.begin(), L.end(), [](const shared_ptr<Paint> &a, const shared_ptr<Paint> &b)
                { return a->currSize > b->currSize; });
      // clean largest locked paint; cleanAdj is updated
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
      out << (G[i][j]->isFull() ? G[i][j]->paint->size : 0) << ' ';
    out << endl;
  }
  out << "***";

  /*do a bfs on the graph to find errors
  for (int i = 0; i < N; i++)
  {
    for (int j = 0; j < M; j++){
      if(G[i][j]->paint != nullptr){
        //start bfs
        shared_ptr<Paint> p = G[i][j]->paint;
        queue<Cell*> Q;
        Q.push(G[i][j].get());
        int effSize = 0;
        while (!Q.empty())
        {
          Cell *u = Q.front();Q.pop();
          if (u->isFull() && u->paint.get() == p.get())
          {
            effSize++;
            //cout << (u->paint->size) << (u->paint->root == p->root ? 'y' : 'n');
            u->paint = nullptr;
            for (Cell *v : Cell::adj(u))
              Q.push(v);
          }
        }
        cout << endl;
        if ((effSize - (p->size) != 0) || ((p->currSize) - (p->size) != 0))
        {
          cout << "== " << (p->dead ? 'd' : 'a') << " a=" << p->alpha << " k=" << p->kills << " r=" << *(p->root) << endl;
          if (effSize - (p->size) != 0)
          {
            cout << "-- effSize=" << effSize << (effSize - (p->size) > 0 ? ">" : "<=") << (p->size) << "=size " << endl;
            cout << "--- currSize " << ((p->currSize) - (p->size) == 0 ? '=' : '!') << " size " << endl;
          }
          if((p->currSize)-(p->size) != 0){
            cout << "-- currSize " << ((p->currSize) - (p->size) > 0 ? ">" : "<=") << " size " << endl;
          }
        }
        else
        {
          cout << "## " << (p->dead ? 'd' : 'a') << " a=" << p->alpha << " k=" << p->kills << " r=" << *(p->root) << endl;
        }
      }
    }
  }*/

  in.close();
  out.close();
  return 0;
  }
