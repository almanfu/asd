#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

vector<vector<int>> adjmat;

int main(){
  ifstream in("input.txt");
  int N, M;
  in >> N >> M;

  adjmat.resize(N);
  for(int i=0; i<N; i++) {
    adjmat[i].resize(N);
  }

  for(int j=0; j<M; j++) {
    int from, to;
    in >> from >> to;
    adjmat[from][to] = 1;
  }

  for(int i=0; i<N; i++) {
    for(int j=0; j<N; j++) {
      cout << adjmat[i][j] << "\t";
    }
    cout << endl;
  }

  return 0;
}
