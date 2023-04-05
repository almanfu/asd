#include <utility>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct evento {
  int time;
  bool isstart;
};

// l'operatore < definisce l'ordinamento per strutture dati di tipo evento
bool operator < (const evento ev1, const evento ev2) {
  return ev1.time < ev2.time;
}

ifstream in("input.txt");
ofstream out("output.txt");

int main(int argc, char const *argv[]) {
  int N;

  in >> N;

  vector<evento> vec;

  // lettura input
  for (int i=0; i<N; i++) {
    int start, end;
    evento evs, eve;

    in >> start >> end;

    evs.time = start;
    evs.isstart = true;

    eve.time = end;
    eve.isstart = false;

    vec.push_back(evs);
    vec.push_back(eve);
  }

  // ordinamento delle struct secondo l'operatore definito sopra
  sort(vec.begin(), vec.end());

  int active = 0;
  int R = -1;

  int best = 0;
  int stbest = -1;
  int enbest = -1;

  for (int i=0; i < vec.size(); i++) {
    struct evento cur = vec[i];

    if (cur.isstart) {
      // estremo iniziale di un intervallo

      // se non ci sono intervalli attivi abbiamo trovato un buco
      if( R != -1 && active == 0 && cur.time - R > best) {
        best = cur.time - R;
        stbest = R;
        enbest = cur.time;
      }

      active++;   // --> si è aperto un intervallo

    } else {
      // estremo finale di un intervallo

      active--;   // <-- si è chiuso un intervallo

      R = cur.time;
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
