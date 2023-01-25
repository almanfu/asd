#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <stack>

using namespace std;

/*

*/

class Tree{
  public:
  int value;
  Tree* left;
  Tree* right;
  Tree* parent;
  Tree(int v){
    value = v;
    parent = left = right = nullptr;
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
  friend void link(Tree* p, Tree* u, int v){
    if(u != nullptr)
      u->parent = p;
    if(p != nullptr){
      if(v<p->value)
        p->left = u;
      else
        p->right = u;
    }
  }
  friend Tree* insertNode(Tree* t, int v){
    Tree* p = nullptr;
    Tree* u = t;
    while(u != nullptr && u->value != v){
      p = u;
      u = v < u->value ? u->left : u->right;
    }
    if(u != nullptr && u->value == v)
      u->value = v;
    else{
      Tree* n = new Tree(v);
      link(p, n, v);
      if(p == nullptr)
        t = n;
    }
    return t;
  }
friend void inorder(Tree* t){
    std::stack<Tree*> s;
    while(!s.empty() || t != nullptr){
      if(t != nullptr){
        s.push(t);
        t = t->left;
      }else{
        t = s.top();
        s.pop();
        cout << t << " ";
        t = t->right;
      }
    }
    cout << endl;
  }
  friend void postorder(Tree* t){
    std::stack<Tree*> s;
    Tree* prev = t;
    if(t != nullptr)
      s.push(t);
    while(!s.empty()){
      t = s.top();
      if(prev == t){// Descending
        if(t->left != nullptr){
          s.push(t->left);
          prev = t->left;
        }else if(t->right != nullptr){
          s.push(t->right);        
          prev = t->right;
        }else{
          cout << t << " ";
          prev = t;
          s.pop();
        }
      }else if(prev == t->left){// Ascending from left
        if(t->right != nullptr){
          s.push(t->right);
          prev = t->right;
        }else{
          cout << t << " ";
          prev = t;
          s.pop();
        }
      }else if(prev == t->right){// Ascending from right
          cout << t << " ";
          prev = t;
          s.pop();      
      }
    }
    cout << endl;
  }
    friend ostream& operator<<(ostream& os, Tree*& t){
    if (t != nullptr)
      os << t->value;
    return os;
  }
  friend istream& operator>>(istream& is, Tree*& t){
    int v;
    is >> v;
    t = insertNode(t, v);
    return is;
  }
};

int main(int argc, char *argv[]){
  //ofstream out("input.txt");

  Tree* t = new Tree(100);

  for(int _=0; _ < 3000; _++)
    insertNode(t, rand());
  
  inorder(t);
  postorder(t);

  deleteTree(t);

  //out.close();
  return 0;
}
