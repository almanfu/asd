#include <utility>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <climits>
using namespace std;

ifstream in("input.txt");
ofstream out("output.txt");

int main(int argc, char const *argv[]) {
  int N;
  in >> N;

  vector<bool> sim;
  vector<pair<int,int>> intervalli;
  intervalli.resize(N);

  // valori minimo e massimo
  int m = INT_MAX;
  int M = -1;

  // lettura input
  for (int i=0; i<N; i++) {
    int start, end;
    in >> start >> end;

    // il valore più piccolo è un estremo iniziale
    if(start < m) {
      m = start;
    }

    // il valore più grande è un estremo finale
    if(end > M) {
      M = end;
    }

    intervalli[i].first = start;
    intervalli[i].second = end;
  }

  // vettore di simulazione da m a M (compresi), quindi di dimensione M-m+1
  sim.resize(M-m+1);

  for(auto intv: intervalli) {
    for(int s=intv.first; s<intv.second; s++) {
      sim[s-m] = true;
    }
  }

  int best = 0;
  int stbest=-1, enbest=-1;

  for(int j=0; j<sim.size(); j++) {
    cout << j << "\t";
  }
  cout << endl;
  for(int j=0; j<sim.size(); j++) {
    cout << sim[j] << "\t";
  }
  cout << endl;

  int count = 0;
  for(int j=0; j<sim.size(); j++) {
    if(!sim[j]) {
      count++;
    } else {
      if(count > best) {
        best = count;
        enbest = j+m;
        stbest = j+m-count;
      }
      count = 0;
    }
  }

  if(best > 0) {
    // quanto scritto su cout è ignorato da judge
    cout << "Buco più grande, di lunghezza " << best \
         << ", da " << stbest << " a " << enbest << endl;

    // scrittura risultato su output.txt
    out << stbest << " " << enbest << endl;
  } else {
    cout << "Nessun buco trovato" << endl;
    out << 0 << endl;

  }

  return 0;
}
