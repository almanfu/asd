#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <stack>

#include <iomanip>
#include <cmath>

using namespace std;

/*

*/

class Tree{
  public:
  int value, depth;
  bool goRight;
  Tree* left;
  Tree* right;
  Tree(int v, int d){
    value = v;
    depth = d;
    goRight = false;
    left = right = nullptr;
  }
  ~Tree(){}
  friend void print_visual(Tree* t, int p=0){
    // Rotate visually by 90° right
    if (t != nullptr)
    {
      p++;
      print_visual(t->right, p);
      cout << string(p*4, ' ') << t->value << endl;
      print_visual(t->left, p);
    }
  }
  void deleteLeft(){
    if(left != nullptr){
      left->deleteLeft();
      left->deleteRight();
      delete left;
      left = nullptr;
    }
  }
  void deleteRight(){
    if(right != nullptr){
      right->deleteLeft();
      right->deleteRight();
      delete right;
      right = nullptr;
    }
  }
  int launchBall(){
    Tree* t = this;
    Tree* p = this;
    while(t != nullptr){
      p = t;
      if(t->goRight){
        t->goRight = !t->goRight;
        t = t->right;
      }else{
        t->goRight = !t->goRight;
        t = t->left;
      }
    }
    return p->value;
  }
  static void reset(Tree* t){
    if(t == nullptr) return;
    t->goRight = false;
    reset(t->left);
    reset(t->right);
  }
  static Tree* giveComplete(int D){
    Tree* t = new Tree(1, 0);
    queue<Tree*> q = queue<Tree*>();
    int u=1;
    q.push(t);
    while(!q.empty()){
      Tree* s = q.front(); q.pop();
      if(s->depth == D) break;

      s->left = new Tree(++u, s->depth+1);
      s->right = new Tree(++u, s->depth+1);
      q.push(s->left);
      q.push(s->right);
    }
    return t;
  }
  static void deleteTree(Tree*& t){
    t->deleteLeft();
    t->deleteRight();
    delete t;
    t = nullptr;
  }
};

void simulate(int D=5){
 for(int d=1; d <= D; d++){
    int p2d = pow(2, d);
    Tree* t = Tree::giveComplete(d);

    cout << "=============" << endl;
    cout << "d=" << d << endl;
    cout << "p2d=" << p2d << endl;

    for(int i=1; i <= p2d; i++)
      cout << setw(4) << i << ' ';
    cout << endl;

    for(int i=1; i <= p2d; i++)
      cout << setw(4) << (t->launchBall()-p2d+1) << ' ';
    cout << endl;

    Tree::reset(t);

    int prev = t->launchBall()-p2d+1;
    cout << setw(4) << 'X' << ' ';
    for(int i=1; i < p2d; i++){
      int curr = t->launchBall()-p2d+1;
      cout << setw(4) << (curr - prev) << ' ';
      prev = curr;
    }
    cout << endl;

    Tree::deleteTree(t);
  }  
}

int main(int argc, char *argv[]){
  //simulate();
  //return 0;

  struct problem{
    int k;
    int i;
  };

  int h;
  cin >> h;

  int D=0;

  vector<list<problem>> v = vector<list<problem>>(20, list<problem>());
  vector<int> sol = vector<int>(h);
  
  for(int k=0; k<h; k++){
    int d, i;
    cin >> d >> i;
    v[d-1].push_front({k, i-1});
    D = max(D, d-1);
  }


  list<int> l = list<int>();
  l.push_front(1);
  int p2d = 1;
  for(int d=0; d<=D; d++){
    if(d>0){
      p2d *= 2;
      auto iter = l.begin();
      while(iter != l.end()){
        l.insert(++iter, *iter+p2d/2);
      }
    }

    vector<int> lv = vector<int>(l.begin(), l.end());

    for(problem p: v[d])
      sol[p.k] = lv[p.i]+p2d-1;

  }

  for(int s: sol)
    cout << s << endl;

  return 0;
}
