#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
using namespace std;


struct nodo{
  vector<int> adj;
  int dist;
  int num;
};

vector<nodo> grafo;
int N;

void bfs(int st){
  //Inizializzo le variabili
  for(int i=0;i<N;i++){
    grafo[i].dist=-1;
    grafo[i].num=0;
  }
  grafo[st].dist=0;
  grafo[st].num=1;
  queue<int> q;
  q.push(st);
  while(!q.empty()){
    int cur=q.front();
    q.pop();
    //Per ogni nodo, guarda i suoi vicini
    for(int vic:grafo[cur].adj){
      //Se non é stato ancora visto, aggiorna distanza e inserisci nella coda
      if(grafo[vic].dist==-1){
        grafo[vic].dist=grafo[cur].dist+1;
        q.push(vic);
      }
      //Se sono un predecessore di vic, aggiorno il numero di cammini
      if(grafo[vic].dist==grafo[cur].dist+1)
        grafo[vic].num+=grafo[cur].num;      
    }
  }
}

int main(void)
{
  ifstream in("input.txt");
  int M,S,T;
  in>>N>>M>>S>>T;
  //Lettura del grafo
  grafo.resize(N);
  for(int i=0;i<M;i++){
    int f,t;
    in>>f>>t;
    grafo[f].adj.push_back(t);
  }
  bfs(S);

  ofstream out("output.txt");
  out<<grafo[T].dist<<" "<<grafo[T].num<<endl;
  return 0;
}

