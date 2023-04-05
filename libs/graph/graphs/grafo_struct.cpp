#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

struct nodo{
  vector<int> vic;
  bool visited;
};

vector<nodo> grafo;

int main(){
  ifstream in("input.txt");
  int N, M;
  in >> N >> M;
  grafo.resize(N);

  for(int j=0; j<M; j++) {
    int from, to;
    in >> from >> to;
    grafo[from].vic.push_back(to);
  }

  for(int i=0; i<N; i++) {
    cout << "Nodo " << i << " ha " << grafo[i].vic.size() <<" vicini" << endl;
    for(int v: grafo[i].vic) {
      cout << "  " << v << endl;
    }
  }

  return 0;
}
