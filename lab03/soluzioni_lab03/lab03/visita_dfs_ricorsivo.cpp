#include <fstream>
#include <vector>
using namespace std;


struct nodo{
  vector<int> adj;
  bool visited=false;
};

vector<nodo> grafo;
int count=0;

void visit(int n){
  //Se il nodo é stato giá visitato, ignoralo
  if(grafo[n].visited)
    return;
  grafo[n].visited=true;
  //Aumenta il contatore
  count++;
  //Visita tutti i vicini
  for(int v:grafo[n].adj)
    visit(v);
}

int main(void)
{
  ifstream in("input.txt");
  ofstream out("output.txt");
  int N,M,S;
  in>>N>>M>>S;
  grafo.resize(N);
  //Lettura del grafo
  for(int i=0;i<M;i++){
    int f,t;
    in>>f>>t;
    grafo[f].adj.push_back(t);
  }
  //DFS ricorsiva partendo da S
  visit(S);
  out<<count<<endl;
  return 0;
}

