#include <fstream>
#include <vector>
#include <map>
#include <tuple>
#include <algorithm>
using namespace std;

/*
Il problema è scomposto in due parti:
  1. calcolo delle distanza di ogni nodo dal proprio portale più lontano;
  2. selezione dei nodi sui quali posizionare i cavalieri e conteggio.

# Calcolo delle distanze

* si visita l’albero con una BFS da ogni nodo, definendo il sottoproblema
sugli archi, gli archi sono orientati nella direzione indicata nell’input;
* con la memoization l’albero viene visitato una volta sola;

# Posizionamento e conteggio dei cavalieri

* Si radica l'albero nel nodo la cui distanza dal portale più lontano
  è minima e si orientano gli archi dai padri ai figli;
* Inizialmente:
  --  ogni nodo è in un gruppo a sè stante;
  --  si mette un cavaliere su ogni nodo;
* Si analizza ogni nodo in ordine da quello a distanza massima dalla
  sorgente più lontana. Dato il nodo corrente (n):
  --  si uniscono nello stesso gruppo ogni nodo con i suoi figli (escludendo
      il nodo padre da cui si proviene). 
      I gruppi con nodi in comune si contano insieme;
  --  si eliminano i cavalieri dai nodi qualora d_v - d_n > L;

C è il conteggio del numero massimo di cavalieri presenti in un gruppo in un
dato momento;

*/

struct nodo{
  int id;
  int dist = 0;
  int start_edge;
  int end_edge;
  int father = -1;
};

// struttura dati union-find
struct UF{
  vector<int> f;
  vector<int> c;
  UF(int N){
    f.resize(N, -1);
    c.resize(N, 1);
  }
  int find(int el){
    if(f[el]==-1)
      return el;
    return f[el]=find(f[el]);
  }
  void uni(int a, int b){
    int fa = find(a);
    int fb = find(b);
    if(fa==fb)
      return;
    if(c[fa]>c[fb]){
      f[fb]=fa;
      c[fa]+=c[fb];
    }else{
      f[fa]=fb;
      c[fb]+=c[fa];
    }
  }
};

// edge definiti come triple (source, target, weight)
vector<tuple<int, int, int>> edges;
// memoization
vector<int> DP;
vector<nodo> G;

int dist(int edge, bool has_father){
  int node, father;
  if(has_father){
    if(DP[edge]!=-1)
      return DP[edge];
    // gli elementi della tuple sono numerato da 0
    // dato e = (source, target, weight)
    // get<1>(e) -> nodo di arrivo
    // get<0>(e) -> nodo di partenza
    node = get<1>(edges[edge]);
    father = get<0>(edges[edge]);
  }else{
    node = edge;
    father = node;
  }
  int ma = 0;
  for(int e=G[node].start_edge;e<G[node].end_edge;e++){
    int dest = get<1>(edges[e]);
    int w = get<2>(edges[e]);
    if(dest!=father){
      ma = max(ma, w + dist(e, true));
    }
  }
  if(has_father)
    DP[edge]=ma;
  return ma;
}

int dfs(int node, int father){
  for(int e=G[node].start_edge;e<G[node].end_edge;e++){
    int dest = get<1>(edges[e]);
    if(dest!=father){
      G[dest].father = node;
      dfs(dest, node);
    }
  }
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
    edges.push_back(make_tuple(v1,v2,w));
    edges.push_back(make_tuple(v2,v1,w));
  }

  // ordinando gli archi in ordine lessigrafico posso individuare per
  // ogni nodo quali sono i suoi figli
  sort(edges.begin(), edges.end());
  DP.resize(edges.size(), -1);
  
  int last = -1;
  for(int i=0;i<edges.size();i++){
    int source = get<0>(edges[i]);
    if(source!= last){
      G[source].start_edge = i;
      if(last>-1)
        G[last].end_edge = i;
    }
    last = source;
  }
  G[last].end_edge = edges.size();

  vector<pair<int, int>> nodes;
  for(nodo& n:G){
    n.dist=max(n.dist, dist(n.id, false));
    nodes.push_back(make_pair(n.dist, n.id));
  }
  
  // ordino i nodi per distanza, radico l'albero nel nodo a distanza minima
  // faccio partire una DFS da questo nodo per orientare tutti gli archi
  // dai padri ai figli.
  sort(nodes.rbegin(), nodes.rend());
  dfs(nodes[nodes.size()-1].second, -1);
  
  int L;
  in >> L;
  for(int i=0;i<L;i++){
    int l;
    in >> l;
    UF uf(N);
    int it = 0;
    int ans = 0;
    for(int i=0;i<N;i++){
      int node = nodes[i].second;
      for(int e=G[node].start_edge;e<G[node].end_edge;e++){
        int dest = get<1>(edges[e]);
        if(dest != G[node].father){
          uf.uni(node, dest);
        }
      }
      while(G[nodes[it].second].dist - G[node].dist > l){
        uf.c[uf.find(nodes[it].second)]--;
        it++;
      }
      ans = max(ans, uf.c[uf.find(node)]);
    }
    out<<ans<<endl;
  }  
  return 0;
}
