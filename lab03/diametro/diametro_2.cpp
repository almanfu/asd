#include <fstream>
#include <vector>
#include <queue>
using namespace std;


struct nodo{
  vector<int> adj;
  int dist;
};

vector<nodo> grafo;

int maxdist = -1;
int N, M;

void bfs(int st){
  { // reset del problema
    // all'inizio ogni nodo ha distanza -1, a parte il nodo di partenza
    for(nodo& n:grafo)
      n.dist = -1; 
    grafo[st].dist = 0;
  }

  // coda per la visita
  queue<int> q;
  q.push(st);
  int cur;
  while(!q.empty()){
    cur = q.front();
    q.pop();
    for(int vic: grafo[cur].adj){
      if(grafo[vic].dist == -1){
        // se un vicino non é ancora stato visitato, imposto la sua distanza.
        grafo[vic].dist = grafo[cur].dist+1;
        q.push(vic);
      }
    }
  }
  // aggiorno la distanza massima con la distanza dell'ultimo nodo visitato
  maxdist = max(maxdist,grafo[cur].dist);
}

int main(void) {
  ifstream in("input.txt");
  ofstream out("output.txt");

  in >> N >> M;
  grafo.resize(N);

  // lettura del grafo
  for(int i=0;i<M;i++){
    int f,t;
    in>>f>>t;
    grafo[f].adj.push_back(t);
    grafo[t].adj.push_back(f);
  }

  // bfs da ogni nodo
  for(int i=0;i<N;i++)
    bfs(i);

  out << maxdist << endl;
  return 0;
}

