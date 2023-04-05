#include <fstream>
#include <iostream>
#include <vector>
#include <climits>
using namespace std;

ifstream in("input.txt");
ofstream out("output.txt");

vector<int> vec;
vector<bool> vis;

void printVector(vector<int> &v);

int main(void) {
  int N;
  in >> N;
  vec.resize(N + 1);
  vis.resize(N + 1, false);
  for (int i = 1; i <= N; i++) {
    in >> vec[i];
  }

  int mosse = 0;
  int prezzo = 0;
  for (int i = 1; i <= N; i++) {
    if (!vis[i]) {  // se non l'ho visitato
      // se un numero non è al suo posto devo fare un ciclo di sistemazioni
      if (vec[i] != i) {
        int cur = vec[i];
        vis[cur] = true;
        cout << "C'è un numero fuori posto! " \
             << "Ho trovato: " << vec[i] << " al posto: " << i << endl;

        int mosseCiclo = 0;
        int sum = cur;
        int m = cur;
        do {
          cout << "\t Scambio " << cur;

          // dico di aver visitato l'elemento estraneo
          // (come se lo mettessi al posto giusto)
          vis[vec[cur]] = true;

          // mettendolo al suo posto vuol dire che qualcun altro era
          // fuori posto... finchè non trovo i
          cur = vec[cur];

          m = min(m, cur);

          sum += cur;
          cout <<" con "<< cur << endl;
          mosseCiclo++;
        } while (!vis[vec[cur]]);

        // mosse conta il numero totale di mosse necessario come la somma del
        // numero di mosse necessario per sistemare ogni gruppo di numeri fuori
        // posto
        mosse += mosseCiclo;

        // nelle slide m := m, n := mosseCiclo, S := sum
        cout << "m = " << m << ", n = " << mosseCiclo+1 << ", S = " << sum << endl;

        // calcoliamo il costo minimo usando l'elemento minimo o 1
        int Cm = (mosseCiclo * m) + sum - m;
        int C1 = 2 * (1 + m) + (mosseCiclo) + sum - m;

        cout << "Costi ordinamento ciclo: " \
             << "Cm: " << Cm << ", oppure C1: " << C1 << endl;

        // scelgo il minimo e lo sommo a prezzo che contiene il totale
        prezzo += min(Cm, C1);
      }
      vis[i] = true;
    }
  }

  cout << mosse << " " << prezzo << endl;

  // scrivo il risultato su output.txt
  out << mosse << " " << prezzo << endl;

  return 0;
}
