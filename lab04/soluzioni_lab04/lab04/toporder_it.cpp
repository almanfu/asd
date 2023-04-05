#include <fstream>
#include <vector>
#include <queue>
using namespace std;

struct nodo{
  vector<int> vic;
  //Manteniamo il numero di archi entranti
  int entranti=0;
};

vector<nodo> grafo;

int main(void)
{
  ifstream in("input.txt");
  ofstream out("output.txt");
  int N,M;
  in>>N>>M;
  grafo.resize(N);
  for(int i=0;i<M;i++){
    int a,b;
    in>>a>>b;
    grafo[a].vic.push_back(b);
    grafo[b].entranti++;
  }
  //Coda con i nodi sorgenti che abbiamo trovato
  queue<int> sorgenti;
  //Se un nodo non ha archi entranti, é una sorgente.
  for(int i=0;i<N;i++)
    if(grafo[i].entranti==0)
      sorgenti.push(i);
  
  while(!sorgenti.empty()){
    int p=sorgenti.front();
    sorgenti.pop();
    //Abbiamo trovato p, ora lo rimuoviamo
    out<<p<<" ";
    //Per ogni vicino
    for(int v:grafo[p].vic){
      //Aggiorniamo il numero di archi entranti
      grafo[v].entranti--;
      //Se entranti=0, v é diventato una sorgente
      if(grafo[v].entranti==0)
        sorgenti.push(v);
    }
  }
  out<<endl;
  return 0;
}

