#include <fstream>
#include <vector>
#include <map>
using namespace std;

/*
Si utilizza la definzione ricorsiva di albero.

Il problema è scomposto in due parti:
  1. calcolo delle distanza di ogni nodo dal proprio portale più lontano;
  2. selezione dei nodi sui quali posizionare i cavalieri e conteggio.

# Calcolo delle distanze

Definizione del sottoproblema: lo stato è dato dal nodo corrente (n) e dal nodo
padre (p):
              /
             / 0                              se si "torna indietro" a p 
  S[n, p] = <
             \ max({w(v, n) + S[v, n]}_{v})   per ogni nodo v vicino di n,
              \                               tale che v sia diverso da p

la memoization si realizza con una mappa map<pair<int,int>, int>.

Di fatto equivale ad eseguire una visita in profondità su ogni nodo, con la
memoization la visita viene eseguita una volta sola.

# Posizionamento e conteggio dei cavalieri

Definizione del sottoproblema: lo stato è dato da nodo corrente n, padre p e
distanza della radice (d_r) dal portale più lontano:

                   / 0                        se d_n < d_r
                  /
                 /   0                        se d_n - d_r > L      
 S[n, p, d_r] = <
                 \ 
                  \  1 + \sum_{v} S[v,n,d_r]  altrimenti
                   \

la somma è svolta sui vicini di n, escluso il padre. Per ogni nodo n,
si parte da S[n, n, d_n].

*/

struct nodo{
  vector<pair<int, int>> vic;
  int id;
  int dist = 0;
};

// struttura dati per la memoization
map<pair<int,int>, int> dp;
vector<nodo> G;

int dist(int node, int father){
  auto state = make_pair(node,father);
  if(dp.find(state)==dp.end()){
    int ma = 0;
    for(auto& edge:G[node].vic){
      if(edge.first!=father){
        ma = max(ma, edge.second + dist(edge.first, node));
      }
    }
    dp[state]=ma;
  }
  return dp[state];
}


int visit(int node, int father, int min, int L){
  if(G[node].dist < min || G[node].dist - min > L)
    return 0;
  int count = 1;
  for(auto& edge:G[node].vic)
    if(edge.first!=father)
      count += visit(edge.first,node,min,L);
  return count;
}

int main(){
  int N;
  in >> N;
  G.resize(N);
  for(int i=0;i<N;i++)
    G[i].id=i;
  for(int i=0;i<N-1;i++){
    int v1, v2, w;
    in>>v1>>v2>>w;
    G[v1].vic.push_back(make_pair(v2,w));
    G[v2].vic.push_back(make_pair(v1,w));
  }

  // Calcolo distanze da ogni nodo
  for(nodo& n:G){
    n.dist=max(n.dist, dist(n.id, n.id));
  }

  // posizionamento e conteggio cavalieri
  int L;
  in >> L;
  for(int i=0;i<L;i++){
    int l;
    in >> l;
    int best = 1;
    for(nodo& n:G)
      best = max(best, visit(n.id, n.id, n.dist, l));
    out<<best<<endl;
  }  
  return 0;
}
