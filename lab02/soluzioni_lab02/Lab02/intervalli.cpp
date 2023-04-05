#include <utility>
#include <fstream>
#include <algorithm>
using namespace std;

// array di coppie di interi
vector<pair<int,int>> arr;

int main(void) {
  ifstream in("input.txt");

  int N;
  in >> N;
  arr.resize(N);

  for(int i=0; i<N; i++) {
    in >> arr[i].first >> arr[i].second;  
  }

  // sort ordina prima per first, poi per second
  sort(arr.begin(),arr.end());

  // l'istante più grande coperto dagli intervalli
  int R = arr[0].second;

  // la migliore soluzione trovata fino a questo punto
  int stbest = -1;
  int enbest = -1;

  for(int i=1; i<N; i++) {
    // abbiamo trovato un nuovo buco di lunghezza massima?
    // nota: controllare che arr[i].first > R è ridondante, perché in caso
    // contrario arr[i].first-R < 0 che è sicuramente minore di enbest-stbest
    if( arr[i].first-R > enbest-stbest ) {
      stbest = R;
      enbest = arr[i].first;
    }

    // aggiorna la variabile
    R = max(R, arr[i].second);
  }

  ofstream out("output.txt");
  if(stbest == -1) {
    out << 0 << endl;
  }
  else {
    out << stbest << " " << enbest << endl;
  }
  return 0;
}
