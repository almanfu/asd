#include <fstream>
using namespace std;

int main() {
  // input e output streams
  ifstream in("input.txt");
  ofstream out("output.txt");

  int N;
  int mx = 0;
  int last = 0;
  int cur;

  // la prima riga dell'input contiene la dimensione del vettore
  in >> N;

  // algoritmo di Kadane
  // intuizione: la sottosequenza di somma massima conterrà un elemento
  //             con indice massimo, sia esso "i". Chiamiamo B_{i} la
  //             sottosequenza di somma massima che ha come ultime
  //             elemento il numero in posizione "i".
  //             Se riusciamo a calcolare B_0, B_1, ..., B_{N-1} allora
  //             la soluzione S si trova con:
  //                 S = max(B_0, B_1, ..., B_{N-1})
  for(int i=0; i<N; i++){
    // leggo un elemento del vettore
    in >> cur;

    // last contiene la somma parziale fino all'elemento precedente a
    // quello corrente, i-1. Se suppongo di conosecre B_{i-1} e voglio
    // trovare B_{i} ho due possibilità:
    //   - aggiungo cur, quindi considero la sottosequenza che arriva
    //     fino all'elemento "i"
    //   - riparto da cur
    last = max(cur, cur+last);

    // il massimo dei B_{i} trovo il massimo generale
    mx = max(mx, last);
  }

  out << mx << endl;
  return 0;
}
