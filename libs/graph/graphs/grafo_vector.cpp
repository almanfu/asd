#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

vector<vector<int> > adj;

int main(){
  ifstream in("input.txt");
  int N, M;
  in >> N >> M;
  adj.resize(N);

  for(int j=0; j<M; j++) {
    int from, to;
    in >> from >> to;
    adj[from].push_back(to);
  }

  for(int i=0; i<N; i++) {
    cout << "Nodo " << i << " ha " << adj[i].size() << " vicini" << endl;
    for(int j=0; j<adj[i].size(); j++) {
      cout << "  " << adj[i][j] << endl;
    }
  }

  return 0;
}
