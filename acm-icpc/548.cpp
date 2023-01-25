#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <vector>
#include <list>
#include <queue>
#include <stack>

#include <unordered_map>

using namespace std;

/*

*/

class Tree{
  public:
  int value;
  Tree* left;
  Tree* right;
  Tree(int v){
    value = v;
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
  friend void deleteTree(Tree*& t){
    t->deleteLeft();
    t->deleteRight();
    delete t;
    t = nullptr;
  }
};

void aux(Tree*& s, int u, unordered_map<int, int> io){
  if(!s){
    s = new Tree(u);
  }else{
    if(io[s->value] < io[u])
      aux(s->right, u, io);
    else
      aux(s->left, u, io);
  }
}

Tree* buildTree(vector<int> IO, vector<int> PO, int n){
  Tree* t = new Tree(PO[n-1]);
  unordered_map<int, int> io = unordered_map<int, int>(n);
  for(int i=0; i < n; i++)
    io.insert(pair<int, int>(IO[i], i));
  
  for(int i=n-2; i >= 0; i--){
    Tree* s = t;
    aux(s, PO[i], io);
  }

  return t;
}
//   sum, leaf val
pair<int, int> lvp(Tree* t, int sum){
  
  if(t == nullptr){
    return pair<int, int>(INT32_MAX, INT32_MAX);
  }else{
    pair<int, int> l = lvp(t->left, sum+t->value);
    pair<int, int> r = lvp(t->right, sum+t->value);
    if(l.first == r.first && r.first == INT32_MAX)
      return pair<int, int>(sum+t->value, t->value);
    else
      return min(l, r);
  }
}

int p548 (vector<int> IO, vector<int> PO, int n){
  Tree* t = buildTree(IO, PO, n);
  //print_visual(t);
  pair<int, int> res = lvp(t, 0);
  deleteTree(t);
  return res.second;
}

int main(int argc, char *argv[]){
  //ifstream in("input.txt");
  //ofstream out("output.txt");

  string io_s;
  string po_s;
  while (getline(cin, io_s) && getline(cin, po_s)){
    istringstream io_b(io_s);
    istringstream po_b(po_s);
    int u;
    vector<int> IO, PO;

    while (io_b >> u){
      IO.push_back(u);
    }
    while (po_b >> u){
      PO.push_back(u);
    }

    int res = p548(IO, PO, IO.size());

    cout << res << endl;
  }

  //in.close();
  //out.close();
  return 0;
}
