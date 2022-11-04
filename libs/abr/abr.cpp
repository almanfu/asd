#include <iostream>
#include <fstream>

#include <bitset>
#include <vector>
#include <list>
#include <queue>
#include <deque>
#include <map>
#include <stack>
#include <set>

#include <string>
#include <cctype>

#include <random>
#include <utility>
#include <functional>
#include <algorithm>

using namespace std;

/*

*/

class Item{
  int v;
  public:
  Item(){
    this->v = 0;
  }
  Item(int v){
    this->v = v;
  }
  int read(){
    return this->v;
  }
  void write(int v){
    this->v = v;
  }
  friend bool operator==(Item i1, Item i2){
    return i1.v == i2.v;
  }
  friend bool operator!=(Item i1, Item i2){
    return i1.v != i2.v;    
  }
  friend bool operator<=(Item i1, Item i2){
    return i1.v <= i2.v;    
  }
  friend bool operator>=(Item i1, Item i2){
    return i1.v >= i2.v;    
  }
  friend bool operator<(Item i1, Item i2){
    return i1.v < i2.v;    
  }
  friend bool operator>(Item i1, Item i2){
    return i1.v > i2.v;
  }
  friend ostream& operator<<(ostream& os, Item& i){
    return os << i.v;
  }
  friend istream& operator>>(istream& is, Item& i){
    return is >> i.v;
  }
};

class Tree{
  public:
  Item key;
  Item value;
  Tree* parent;
  Tree* left;
  Tree* right;
  Tree(Item k, Item v){
    key = k;
    value = v;
    parent = left = right = nullptr;
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
  ~Tree(){}
  friend Tree* lookupNode(Tree* t, Item k){
    Tree* u = t;
    while(u != nullptr && u->key != k)
      u = k < u->key ? u->left : u->right;
    return u;
  }
  friend Tree* min(Tree* t){
    Tree* u = t;
    while(u != nullptr)
      u = u->left;
    return u;
  }
  friend Tree* max(Tree* t){
    Tree* u = t;
    while(u != nullptr)
      u = u->right;
    return u;
  }
  friend Tree* successorNode(Tree* t){
    if(t == nullptr)
      return t;
    if(t->right != nullptr)
      return min(t->right);
    else{
      Tree* p = t->parent;
      while(p != nullptr && t == p->right){
        t = p;
        p = p->parent;
      }
      return p;
    }
  }
  friend Tree* predecessorNode(Tree* t){
    if(t == nullptr)
      return t;
    if(t->left != nullptr)
      return max(t->left);
    else{
      Tree* p = t->parent;
      while(p != nullptr && t == p->left){
        t = p;
        p = p->parent;
      }
      return p;
    }
  }
  friend void link(Tree* p, Tree* u, Item k){
    if(u != nullptr)
      u->parent = p;
    if(p != nullptr){
      if(k<p->key)
        p->left = u;
      else
        p->right = u;
    }
  }
  friend Tree* insertNode(Tree* t, Item k, Item v){
    Tree* p = nullptr;
    Tree* u = t;
    while(u != nullptr && u->key != k){
      p = u;
      u = k < u->key ? u->left : u->right;
    }
    if(u != nullptr && u->key == k)
      u->value = v;
    else{
      Tree* n = new Tree(k, v);
      link(p, n, k);
      if(p == nullptr)
        t = n;
    }
    return t;
  }
  friend Tree* removeNode(Tree* t, Item k){
    Tree* u = lookupNode(t, k);
    if(u != nullptr){// CASO 1 (nessun figlio)
      if(u->left == nullptr && u->right == nullptr){
        link(u->parent, nullptr, k);
        if(t == u)
          t = nullptr;
        delete u;
      }// CASO 2 (un solo figlio)
      else if(!(u->left == nullptr && u->right == nullptr)){
        Tree* n = u->left != nullptr ? u->left : u->right;
        if(u->parent == nullptr)
          t = n;
        else
          link(u->parent, n, k);
        delete u;
      }// CASO 3 (entrambi i figli)
      else{
        Tree* s = successorNode(u);// il succ--se in subtree--non ha figlio sx
        link(s->parent, s->right, s->key);
        u->key = s->key;
        u->value = s->value;
        delete s;
      }
    }
    return t;
  }
  friend void preorder(Tree* t){
    std::stack<Tree*> s;
    if(t != nullptr)
      s.push(t);
    while(!s.empty()){
      t = s.top();
      s.pop();
      cout << t << " ";
      if(t->right != nullptr)
        s.push(t->right);
      if(t->left != nullptr)
        s.push(t->left);      
    }
    cout << endl;
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
  friend void print_visual(Tree* t, int p=0){
    // Rotate visually by 90° right
    if (t != nullptr)
    {
      p++;
      print_visual(t->right, p);
      cout << string(p*4, ' ') << t->key << endl;
      print_visual(t->left, p);
    }
  }
  friend ostream& operator<<(ostream& os, Tree*& t){
    if (t != nullptr)
      os << t->key;
    return os;
  }
  friend istream& operator>>(istream& is, Tree*& t){
    Item k, v;
    is >> k >> v;
    t = insertNode(t, k, v);
    return is;
  }
};

pair<int, int> g(Tree* t){
  auto someMax = [](int a, int b, int c){
    if(a>b)
      return a>c?a:c;
    else
      return b>c?b:c;
  };
  if(t == nullptr)
    return pair<int, int>(0, 0);
  if(t->left == nullptr && t->right == nullptr){
    cout << t->key << "->leaf" << endl;
    return pair<int, int>(0, 1);
  }
  auto gLeft = g(t->left);
  auto gRight = g(t->right);
  int sbil = abs(gLeft.second-gRight.second);
  cout << t->key <<"[" << "(" << gLeft.first << "," << gLeft.second << ")," << "(" << gRight.first << "," << gRight.second << ")]";

  return pair<int, int>(someMax(sbil, gLeft.first, gRight.first), gLeft.second+gRight.second);
}

int f(Tree* t){
  return g(t).first;
}

int main()
{
  char option;
  Item key;
  std::random_device rd;
  Tree* t = nullptr;
  do
  {
    cout << endl
         << "r(a)ndom" << endl
         << "(i)nsert" << endl
         << "insert (k)ey" << endl
         << "(r)emove" << endl
         << "(s)earch" << endl
         << "(p)rint" << endl
         << "(c)lear" << endl
         << "(e)xit" << endl;
    cin >> option;
    switch (option)
    {
    case 'a':
      for(int i=0; i < 10; i++)
        t = insertNode(t, Item(rd()%20), Item());
      break;
    case 'i':
      cout << "Key,Value: " << endl;
      cin >> t;
      break;
    case 'k':
      cout << "Key: " << endl;
      cin >> key;
      t = insertNode(t, key, Item());
      break;
    case 'r':
      cout << "Key: ";
      cin >> key;
      t = removeNode(t, key);
      break;
    case 's':
      cout << "Key: ";
      cin >> key;
      if (lookupNode(t, key) != nullptr)
        cout << "Key Found!" << endl;
      else
        cout << "Key Not Found!" << endl;
      break;
    case 'p':
      cout << "Visual Tree: " << endl;
      print_visual(t);
      cout << "preorder: " << endl;
      preorder(t);
      cout << "inorder: " << endl;
      inorder(t);
      cout << "postorder: " << endl;
      postorder(t);
      break;
    case 'c':
      deleteTree(t);
      break;
    case 'e':
      break;
    default:
      cout << "Wrong choice!" << endl;
    }
  } while (option != 'e');
  deleteTree(t);
  return 0;
}