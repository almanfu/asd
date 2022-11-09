#include <fstream>
#include <stack>

using namespace std;

class Tree{
  public:
  int key;
  Tree* left;
  Tree* right;
  Tree(int k){
    key = k;
    left = right = nullptr;
  }
  friend void deleteTree(Tree* t, ostream& os){
    std::stack<Tree*> s;
    bool asc=false;
    if(t != nullptr)
      s.push(t);
    while(!s.empty()){
      t = s.top();
      if(!asc){// Descending
        if(t->left != nullptr){
          s.push(t->left);
        }else if(t->right != nullptr){
          os << t->key << ' ';//IN
          t->left=t->right;
          s.push(t->right);      
        }else{
          os << t->key << ' ';
          asc = true;
          s.pop();
          delete t;//POST
        }
      }else if(t->left != nullptr){
        if(t->right != nullptr && t->right != t->left){// Ascending from left descending to right
          os << t->key << ' ';//IN
          s.push(t->right);
          asc = false;
          t->left = t->right;
        }else if(t->right == t->left){// Ascending from right
          asc = true;
          s.pop();
          delete t;//POST
        }else{//Ascending from left
          os << t->key << ' ';//IN
          asc = true;
          s.pop();
          delete t;//POST
        } 
      }
    }
  }
  ~Tree(){}
  friend Tree* insertNode(Tree* t, int k){
    Tree* p = nullptr;
    Tree* u = t;
    while(u != nullptr){
      p = u;
      u = k <= u->key ? u->left : u->right;
    }
    Tree* n = new Tree(k);
    if(p != nullptr){
      if(k<=p->key)
        p->left = n;
      else
        p->right = n;
    }else{
      t = n;
    }
    return t;
  }
};

int main(int argc, char *argv[]){
  ifstream in("input.txt");
  ofstream out("output.txt");

  int n;
  in >> n;

  Tree* t = nullptr;

  int k;
  for(int i=0; i < n; i++){
    in >> k;
    t = insertNode(t, k);
  }
  in.close();

  deleteTree(t, out);
  out.close();
  return 0;
}