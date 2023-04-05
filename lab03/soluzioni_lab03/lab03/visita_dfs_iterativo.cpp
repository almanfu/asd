#include <fstream>
#include <vector>
#include <stack>
using namespace std;


struct nodo{
  vector<int> adj;
  bool visited;
  nodo(){
    visited=false;
  }
};

vector<nodo> grafo;
int count=0;

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

  stack<int> st;
  st.push(S);
  while(!st.empty()){
    int n=st.top();
    st.pop();
    //Se abbiamo giá visitato il nodo, ignoriamolo.
    if(grafo[n].visited)
      continue;
    grafo[n].visited=true;
    //Aumenta il contatore
    count++;
    //Visita tutti i vicini
    for(int v:grafo[n].adj)
      st.push(v);        
  }

  out<<count<<endl;
  return 0;
}

